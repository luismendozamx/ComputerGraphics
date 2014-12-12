//
//  main.cpp
//  Tren
//
//  Created by Luis Mendoza on 11/13/14.
//  Copyright (c) 2014 LuisMendoza. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <math.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

//Called when a key is pressed
void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
        case 27: //Escape key
            exit(0);
    }
}

//Initializes 3D rendering
void initRendering() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING); //Enable lighting
    glEnable(GL_LIGHT0); //Enable light #0
    glEnable(GL_NORMALIZE); //Automatically normalize normals
    glShadeModel(GL_FLAT); //Enable flat shading
    //glShadeModel(GL_SMOOTH); //Enable smooth shading
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

//Called when the window is resized
void handleResize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}

double acamarasave, dcamara; //Guarda el ultimo angulo de la camara ** incremento de angulo que he movido
int mouseY;
double trayectoria;
float TrenX, TrenY, TrenA, AnguloTan;
double PendienteTan;
int  Elipsea, Elipseb, plano_quads;
float planoX, planoY, plano_sizequads;

double poscamara = 20;
double acamara = 0 * PI / 180; //Lo convierte a radianes
double dcamara = 0;
double trayectoria = 0 * Math.PI / 180;
float TrenX =  0.0f;
TrenY =
TrenA =
AnguloTan = 0.0f;
PendienteTan = 0;
Elipsea = 8;
Elipseb = 6;
planoX = planoY = 0.0f;
plano_quads = 150;
plano_sizequads = 0.3f;

//Draws the 3D scene
void drawScene() {
    
    // Lighting values
    float lightPos[] = { 0.0, 0.0, 0.0, 1.0 };
    float emissionSun[] = { 0.9, 0.0, 0.0, 0.0};
    float nullv[] = { 0.0, 0.0, 0.0, 1.0};
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glPushMatrix(); //save state
    
    glTranslatef(0.0, 0.0, -9.0);
    
    //set specular reflectivity with low shine
    glColor4f(1.0, 1.0, 0.0, 1.0);
    glMateriali(GL_FRONT, GL_SHININESS, 3.0);
    glMaterialfv(GL_FRONT, GL_EMISSION, emissionSun); //make sun glow
    
    glRotatef(_angle, 0.0, 1.0, 0.0);
    glutSolidSphere(0.8, 20, 32); //draw sun
    
    //save lighting
    glPushAttrib(GL_LIGHTING_BIT);
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, nullv);
    glMaterialfv(GL_FRONT, GL_EMISSION, nullv);
    
    glPushMatrix(); //save state
    
    glRotatef(_angle1, 0.0, 1.0, 0.0);
    glTranslatef(3.0, 0.0, 0.0);
    glPushMatrix();
    glRotatef(_angle1, 0.0, 1.0, 0.0);
    glPopMatrix();
    glColor3f(0.0, 0.3, 1.0);
    glutSolidSphere(0.2, 12, 10); //draw planet
    
    glRotatef(_angle2, 0.0, 1.0, 0.0);
    glTranslatef(0.3, 0.0, 0.0);
    glPushMatrix();
    glRotatef(_angle2, 0.0, 1.0, 0.0);
    glPopMatrix();
    glColor3f(0.5, 0.3, 0.0);
    glutSolidSphere(0.05, 5, 4); //draw moon
    
    glPopMatrix(); //restore state
    
    glPopAttrib();
    glPopMatrix();
    
    glutSwapBuffers();
}

void update(int value) {
    _angle += 1.5f;
    if (_angle > 360) {
        _angle -= 360;
    }
    
    _angle1 += 1.5f;
    if (_angle1 > 360) {
        _angle1 -= 360;
    }
    
    _angle2 += 1.5f;
    if (_angle2 > 360) {
        _angle2 -= 360;
    }
    
    glutPostRedisplay();
    glutTimerFunc(25, update, 0);
}

int main(int argc, char** argv) {
    //Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 800);
    
    //Create the window
    glutCreateWindow("Sistema Solar");
    initRendering();
    
    //Set handler functions
    glutDisplayFunc(drawScene);
    glutKeyboardFunc(handleKeypress);
    glutReshapeFunc(handleResize);
    
    glutTimerFunc(15, update, 0); //Add a timer
    
    glutMainLoop();
    return 0;
}
