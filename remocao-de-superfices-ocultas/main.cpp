#include <GL/glut.h>
#include <iostream>
#include <string>

enum Technique { BACKFACE, ZBUFFER, PAINTER };
// Técnica inicial
Technique currentTechnique = BACKFACE;

// Ângulo de rotação no eixo Y
float angleY = 0.0f;

// Função para desenhar texto na tela
void drawText(const char* text, int x, int y) {
    // Muda para modo de projeção para desenhar o texto
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Desativa o teste de profundidade para o texto aparecer por cima
    glDisable(GL_DEPTH_TEST);
    // Posição do texto
    glRasterPos2i(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *text++);
    }
    // Reativa o teste de profundidade
    glEnable(GL_DEPTH_TEST);

    // Restaura as matrizes
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// Função que desenha os cubos na cena
void drawObjects() {
    glPushMatrix();
    glTranslatef(-1.5f, 0, -6);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0, -8);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.5f, 0, -10);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 1.5f, -7);
    glutSolidCube(1.0);
    glPopMatrix();
}

// Versão do desenho manualmente ordenado (Painter's Algorithm)
void painterDraw() {
    // Desenha de trás para frente
    // Mais ao fundo
    glPushMatrix();
    glTranslatef(1.5f, 0, -10);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0, -8);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 1.5f, -7);
    glutSolidCube(1.0);
    glPopMatrix();

    // Mais próximo
    glPushMatrix();
    glTranslatef(-1.5f, 0, -6);
    glutSolidCube(1.0);
    glPopMatrix();
}

// Função principal de renderização
void display() {
    // Limpa os buffers de cor e profundidade
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Aplica rotação com base na tecla pressionada
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);

    // Define o comportamento de acordo com a técnica selecionada
    switch (currentTechnique) {
        case BACKFACE:
            glEnable(GL_CULL_FACE);       // Habilita remoção de faces
            glCullFace(GL_BACK);          // Remove as faces de trás
            glDisable(GL_DEPTH_TEST);     // Desativa z-buffer
            drawObjects();                // Desenha os cubos
            glDisable(GL_CULL_FACE);      // Desativa culling
            drawText("Técnica: Back-Face Culling", 10, 10);
            break;

        case ZBUFFER:
            glEnable(GL_DEPTH_TEST);      // Ativa z-buffer
            drawObjects();                // Desenha os cubos normalmente
            glDisable(GL_DEPTH_TEST);     // Desativa para não afetar o texto
            drawText("Técnica: Z-Buffer", 10, 10);
            break;

        case PAINTER:
            glDisable(GL_DEPTH_TEST);     // Desativa z-buffer
            painterDraw();                // Desenha manualmente de trás pra frente
            drawText("Técnica: Painter’s Algorithm", 10, 10);
            break;
    }

    // Troca os buffers
    glutSwapBuffers();
}

// Função chamada quando a janela é redimensionada
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w/h, 1, 100);
    glMatrixMode(GL_MODELVIEW);
}

// Função para ativar técnica quando número é clicado
void keyboard(unsigned char key, int, int) {
    if (key == '1') currentTechnique = BACKFACE;  // Ativa técnica Back-Face
    if (key == '2') currentTechnique = ZBUFFER;   // Ativa Z-Buffer
    if (key == '3') currentTechnique = PAINTER;   // Ativa Painter's Algorithm
    glutPostRedisplay();
}

// Função para as setas
void specialKeys(int key, int, int) {
    if (key == GLUT_KEY_LEFT) angleY -= 5.0f;     // Rotaciona à esquerda
    if (key == GLUT_KEY_RIGHT) angleY += 5.0f;    // Rotaciona à direita
    glutPostRedisplay();
}

// Função principal
int main(int argc, char** argv) {
    // Inicialização
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Remoção de Faces Ocultas");

    // Ativa recursos do OpenGL
    glEnable(GL_DEPTH_TEST);                      // Z-buffer
    glEnable(GL_LIGHTING);                        // Iluminação
    glEnable(GL_LIGHT0);                          // Luz padrão
    glEnable(GL_COLOR_MATERIAL);                  // Material com cor

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    glutMainLoop();
    return 0;
}
