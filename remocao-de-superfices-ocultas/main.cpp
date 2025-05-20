#include <GL/glut.h>
#include <iostream>
#include <string>

enum Technique { BACKFACE, ZBUFFER, PAINTER };
Technique currentTechnique = BACKFACE;

float angleY = 0.0f;

void drawText(const char* text, int x, int y) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glRasterPos2i(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *text++);
    }
    glEnable(GL_DEPTH_TEST);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void drawObjects() {
    // Cubo 1
    glPushMatrix();
    glTranslatef(-1.5f, 0, -6);
    glutSolidCube(1.0);
    glPopMatrix();

    // Cubo 2
    glPushMatrix();
    glTranslatef(0.0f, 0, -8);
    glutSolidCube(1.0);
    glPopMatrix();

    // Cubo 3
    glPushMatrix();
    glTranslatef(1.5f, 0, -10);
    glutSolidCube(1.0);
    glPopMatrix();

    // Cubo 4
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, -7);
    glutSolidCube(1.0);
    glPopMatrix();
}

void painterDraw() {
    // Manualmente ordenando os objetos de trás para frente
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

    glPushMatrix();
    glTranslatef(-1.5f, 0, -6);
    glutSolidCube(1.0);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glRotatef(angleY, 0.0f, 1.0f, 0.0f);

    switch (currentTechnique) {
        case BACKFACE:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glDisable(GL_DEPTH_TEST);
            drawObjects();
            glDisable(GL_CULL_FACE);
            drawText("Técnica: Back-Face Culling", 10, 10);
            break;

        case ZBUFFER:
            glEnable(GL_DEPTH_TEST);
            drawObjects();
            glDisable(GL_DEPTH_TEST);
            drawText("Técnica: Z-Buffer", 10, 10);
            break;

        case PAINTER:
            glDisable(GL_DEPTH_TEST);
            painterDraw();
            drawText("Técnica: Painter’s Algorithm", 10, 10);
            break;
    }

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w/h, 1, 100);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int, int) {
    if (key == '1') currentTechnique = BACKFACE;
    if (key == '2') currentTechnique = ZBUFFER;
    if (key == '3') currentTechnique = PAINTER;
    glutPostRedisplay();
}

void specialKeys(int key, int, int) {
    if (key == GLUT_KEY_LEFT) angleY -= 5.0f;
    if (key == GLUT_KEY_RIGHT) angleY += 5.0f;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Remoção de Faces Ocultas");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    glutMainLoop();
    return 0;
}
