#include <GL/glut.h>
#include <cmath>

bool perspectiva = true;
float angle = 0.0f;
float camX = 0.0f, camY = 5.0f, camZ = 20.0f;

void setProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (perspectiva) {
        gluPerspective(60.0, 1.0, 1.0, 100.0);
    } else {
        glOrtho(-20, 20, -20, 20, -20, 100);
    }
    glMatrixMode(GL_MODELVIEW);
}

void drawGround() {
    glColor3f(0.3f, 0.9f, 0.3f);
    glBegin(GL_QUADS);
        glVertex3f(-20.0f, 0.0f, -20.0f);
        glVertex3f(-20.0f, 0.0f, 20.0f);
        glVertex3f(20.0f, 0.0f, 20.0f);
        glVertex3f(20.0f, 0.0f, -20.0f);
    glEnd();
}

void drawTree(float x, float z) {
    float trunkHeight = 2.0f;
    float trunkRadius = 0.2f;
    float foliageRadius = 1.0f;

    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(-90, 1, 0, 0);
    glColor3f(0.55f, 0.27f, 0.07f);
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, trunkRadius, trunkRadius, trunkHeight, 16, 16);
    glColor3f(0.0f, 0.5f, 0.00f);
    gluDeleteQuadric(quad);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x, trunkHeight + foliageRadius, z);
    glutSolidSphere(foliageRadius, 20, 20);
    glPopMatrix();
}


void drawHouse(float x, float z) {
    glPushMatrix();
        glTranslatef(x, 1.0f, z);
        glScalef(2.0f, 2.0f, 2.0f);
        glColor3f(0.8f, 0.2f, 0.2f);
        glutSolidCube(1.0f);
        glTranslatef(0.0f, 0.5f, 0.0f);
        glRotatef(-90, 1, 0, 0);
        glColor3f(0.6f, 0.3f, 0.1f);
        glutSolidCone(0.8f, 1.0f, 10, 2);
    glPopMatrix();
}

void drawMailbox(float x, float z) {
    glPushMatrix();
        glTranslatef(x, 0.0f, z);

        glPushMatrix();
            glColor3f(0.4f, 0.4f, 0.4f);
            glTranslatef(0.0f, 0.0f, 0.0f);
            glRotatef(-90, 1.0f, 0.0f, 0.0f);
            GLUquadric* quad = gluNewQuadric();
            gluCylinder(quad, 0.1f, 0.1f, 2.0f, 16, 16);
            gluDeleteQuadric(quad);
        glPopMatrix();

        glPushMatrix();
            glColor3f(1.0f, 0.0f, 0.0f);
            glTranslatef(0.0f, 2.0f, 0.0f);
            glScalef(1.0f, 0.6f, 0.6f);
            glutSolidCube(1.0f);
        glPopMatrix();

    glPopMatrix();
}

void drawSun() {
    glPushMatrix();
        glTranslatef(10.0f * cos(angle * 3.14 / 180), 10.0f, 10.0f * sin(angle * 3.14 / 180));
        glColor3f(1.0f, 1.0f, 0.0f);
        glutSolidSphere(1.0f, 20, 20);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    drawGround();
    drawTree(-5.0f, -5.0f);
    drawTree(5.0f, -5.0f);
    drawHouse(-5.0f, 5.0f);
    drawHouse(5.0f, 5.0f);
    drawMailbox(0.0f, -2.0f);
    drawSun();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'p' || key == 'P') {
        perspectiva = !perspectiva;
        setProjection();
        glutPostRedisplay();
    }
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT: camX -= 1.0f; break;
        case GLUT_KEY_RIGHT: camX += 1.0f; break;
        case GLUT_KEY_UP: camZ -= 1.0f; break;
        case GLUT_KEY_DOWN: camZ += 1.0f; break;
    }
    glutPostRedisplay();
}

void idle() {
    angle += 0.1f;
    if (angle > 360.0f) angle -= 360.0f;
    glutPostRedisplay();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
    setProjection();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Cena 3D com OpenGL");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
