/*
 */



#include <cstdlib>
#include <ctime>
#include <iostream>
#include <math.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "gamedrawer.h"

using namespace std;

const float PI = 3.1415926535f;

//Tiempo en milisegundos para actualizar
const int TIMER_MS = 25;

GameDrawer* gameDrawer;
bool isLeftKeyPressed = false;
bool isRightKeyPressed = false;

float rotationVar = 0;

void cleanup() {
    delete gameDrawer;
    cleanupGameDrawer();
}

void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
        case '\r': //Enter
            if (gameDrawer->isGameOver()) {
                gameDrawer->startNewGame(2.2f, 10);
            }
            break;
        case 27: //Escape
            cleanup();
            exit(0);
    }
}

void handleSpecialKeypress(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            isLeftKeyPressed = true;
            if (isRightKeyPressed) {
                gameDrawer->setPlayerPaddleDir(0);
            }
            else {
                gameDrawer->setPlayerPaddleDir(1);
            }
            break;
        case GLUT_KEY_RIGHT:
            isRightKeyPressed = true;
            if (isLeftKeyPressed) {
                gameDrawer->setPlayerPaddleDir(0);
            }
            else {
                gameDrawer->setPlayerPaddleDir(-1);
            }
            break;
    }
}

void handleSpecialKeyReleased(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            isLeftKeyPressed = false;
            if (isRightKeyPressed) {
                gameDrawer->setPlayerPaddleDir(-1);
            }
            else {
                gameDrawer->setPlayerPaddleDir(0);
            }
            break;
        case GLUT_KEY_RIGHT:
            isRightKeyPressed = false;
            if (isLeftKeyPressed) {
                gameDrawer->setPlayerPaddleDir(1);
            }
            else {
                gameDrawer->setPlayerPaddleDir(0);
            }
            break;
    }
}

void initRendering() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    initGameDrawer();
}

void handleResize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 0.02, 5.0);
}

void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.5f, -0.3f, -1.8f);
    glRotatef(50, 1, 0, 0);
    glRotatef(180, 0, 1, 0);
    
    gameDrawer->draw();
    
    glutSwapBuffers();
}

void update(int value) {
    gameDrawer->advance((float)TIMER_MS / 1000);
    
    rotationVar += 0.2f * (float)TIMER_MS / 1000;
    while (rotationVar > 2 * PI) {
        rotationVar -= 2 * PI;
    }
    
    glutPostRedisplay();
    glutTimerFunc(TIMER_MS, update, 0);
}

int main(int argc, char** argv) {
    srand((unsigned int)time(0)); //Semilla del generador aleatorio
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    
    glutCreateWindow("Proyecto Final - Space Pong");
    initRendering();
    
    gameDrawer = new GameDrawer();
    
    glutDisplayFunc(drawScene);
    glutKeyboardFunc(handleKeypress);
    glutSpecialFunc(handleSpecialKeypress);
    glutSpecialUpFunc(handleSpecialKeyReleased);
    glutReshapeFunc(handleResize);
    glutTimerFunc(TIMER_MS, update, 0);
    
    glutMainLoop();
    return 0;
}









