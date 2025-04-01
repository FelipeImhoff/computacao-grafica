#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 transform;
void main()
{
    gl_Position = transform * vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);  // Cor do quadrado (vermelho)
}
)";

// Função para compilar o shader
unsigned int compileShader(unsigned int type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "Erro ao compilar o shader: " << infoLog << std::endl;
    }
    return shader;
}

// Função para criar o programa de shaders
unsigned int createShaderProgram() {
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "Erro ao vincular o programa de shaders: " << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int main() {
    // Inicializa o GLFW
    if (!glfwInit()) {
        std::cout << "Falha ao inicializar GLFW" << std::endl;
        return -1;
    }

    // Cria a janela do GLFW
    GLFWwindow* window = glfwCreateWindow(800, 600, "Quadrado com OpenGL", nullptr, nullptr);
    if (!window) {
        std::cout << "Falha ao criar a janela do GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Falha ao inicializar o GLAD" << std::endl;
        return -1;
    }

    // Definir os vértices de um quadrado
    float vertices[] = {
        // Posição do quadrado
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    // Criar e vincular o VAO e VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Compilar o programa de shaders
    unsigned int shaderProgram = createShaderProgram();

    // Definir a matriz de transformação (escala e rotação)
    float scaleFactor = 1.5f;
    float angle = 45.0f;  // Ângulo de rotação

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Ativar o programa de shaders
        glUseProgram(shaderProgram);

        // Calculando a matriz de transformação (rotação + escala)
        float radians = angle * (3.14159265359f / 180.0f);
        float transform[16] = {
            static_cast<float>(scaleFactor * cos(radians)), 
            static_cast<float>(-scaleFactor * sin(radians)), 
            0.0f, 0.0f,
            static_cast<float>(scaleFactor * sin(radians)), 
            static_cast<float>(scaleFactor * cos(radians)), 
            0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        

        // Passar a matriz para o shader
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_TRUE, transform);

        // Desenhar o quadrado
        glBindVertexArray(VAO);
        glDrawArrays(GL_QUADS, 0, 4);
        glBindVertexArray(0);

        // Trocar os buffers e processar eventos
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Limpeza e término do GLFW
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
