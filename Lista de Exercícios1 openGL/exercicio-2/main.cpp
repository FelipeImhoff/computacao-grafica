#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath> // Para funções trigonométricas

// Função para verificar erros do OpenGL
void checkShaderCompile(GLuint shader) {
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Erro de compilação no shader: " << infoLog << std::endl;
    }
}

void checkProgramLink(GLuint program) {
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "Erro de link no programa: " << infoLog << std::endl;
    }
}

int main() {
    // Inicializar o GLFW
    if (!glfwInit()) {
        std::cerr << "Falha ao inicializar o GLFW!" << std::endl;
        return -1;
    }

    // Criar uma janela com GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Triângulo OpenGL com Rotação", nullptr, nullptr);
    if (!window) {
        std::cerr << "Falha ao criar a janela GLFW!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Tornar o contexto da janela atual
    glfwMakeContextCurrent(window);

    // Inicializar o GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Falha ao inicializar o GLAD!" << std::endl;
        return -1;
    }

    // Definir os vértices para o triângulo
    float vertices[] = {
        // Posições       // Cores
        0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // Vértice 1 (Verde)
       -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // Vértice 2 (Azul)
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // Vértice 3 (Vermelho)
    };

    // Criar um buffer de vértices
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Vincular o VAO e o VBO
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Definir os layouts de atributos para os vértices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Desvincular o VAO
    glBindVertexArray(0);

    // Vertex Shader
    const char* vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aColor;
        out vec3 ourColor;
        uniform mat4 rotation;

        void main() {
            gl_Position = rotation * vec4(aPos, 1.0);
            ourColor = aColor;
        })";

    // Fragment Shader
    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        in vec3 ourColor;
        void main() {
            FragColor = vec4(ourColor, 1.0f);
        })";

    // Compilar o Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    checkShaderCompile(vertexShader);

    // Compilar o Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    checkShaderCompile(fragmentShader);

    // Criar o programa de shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkProgramLink(shaderProgram);

    // Deletar os shaders, pois já foram linkados no programa
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Obter o local do uniforme da matriz de rotação
    GLint rotationLoc = glGetUniformLocation(shaderProgram, "rotation");

    // Loop de renderização
    while (!glfwWindowShouldClose(window)) {
        // Processar eventos
        glfwPollEvents();

        // Limpar a tela
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Calcular a matriz de rotação (rotação em torno do eixo Z)
        float angle = (float)glfwGetTime(); // Ângulo de rotação com base no tempo
        float cosAngle = cos(angle);
        float sinAngle = sin(angle);

        // Matriz de rotação 4x4 (em torno do eixo Z)
        float rotationMatrix[] = {
            cosAngle, -sinAngle, 0.0f, 0.0f, // Primeira coluna
            sinAngle,  cosAngle, 0.0f, 0.0f, // Segunda coluna
            0.0f,      0.0f,      1.0f, 0.0f, // Terceira coluna
            0.0f,      0.0f,      0.0f, 1.0f  // Quarta coluna
        };

        // Usar o programa de shaders
        glUseProgram(shaderProgram);

        // Enviar a matriz de rotação para o shader
        glUniformMatrix4fv(rotationLoc, 1, GL_TRUE, rotationMatrix);

        // Desenhar o triângulo
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Trocar os buffers
        glfwSwapBuffers(window);
    }

    // Limpeza de recursos
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // Terminar o GLFW
    glfwTerminate();

    return 0;
}
