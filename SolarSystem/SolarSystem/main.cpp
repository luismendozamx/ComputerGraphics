//
//  main.cpp
//  SolarSystem
//
//  Created by Luis Mendoza on 10/6/14.
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

static GLfloat year = 0, day = 0;
static GLfloat pitch = 0, rot = 90.0;
static GLfloat xdist = 0, ydist = 0, zdist = 10.0;
static GLfloat xlook = 0.0, ylook = 0.0, zlook = 0.0;
static GLfloat xup = 0.0, yup = 0.0, zup= 0.0;
static int lastx = 0, lasty = 0;

// Lighting values
GLfloat ambientLight0[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat ambientLight1[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };

GLfloat	lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat emissionSun[] = { 0.9f, 0.0f, 0.0f, 0.0f};
GLfloat nullv[] = { 0.0f, 0.0f, 0.0f, 1.0f};

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
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING); //Enable lighting
    glEnable(GL_LIGHT0); //Enable light #0
    glEnable(GL_NORMALIZE); //Automatically normalize normals
    glShadeModel(GL_SMOOTH); //Enable smooth shading
}

//Called when the window is resized
void handleResize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}

void BluePlanet(void)
{
    
    glRotatef((GLfloat) year*6, 0.0, 1.0, 0.0); //rotate for the planet
    glTranslatef(3.0, 0.0, 0.0);
    glColor3f(0.0, 0.3, 1.0);
    glutSolidSphere(0.2, 10, 8); //draw planet
    
    glRotatef((GLfloat) day*2, 0.0, 1.0, 0.0); //rotate for the moon
    glTranslatef(0.5, 0.0, 0.0);
    glColor3f(0.5, 0.3, 0.0);
    glutSolidSphere(0.05, 5, 4); //draw moon
    
}

float _angle = -70.0f;
float _angle1 = -100.0f;
float _angle2 = -360.0f;

//Draws the 3D scene
void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glPushMatrix(); //save state
    
    glTranslatef(0.0f, 0.0f, -9.0f);
    
    //set specular reflectivity with low shine
    glColor4f(1.0, 1.0, 0.0, 1.0);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
    glMateriali(GL_FRONT, GL_SHININESS, 3);
    glMaterialfv(GL_FRONT, GL_EMISSION, emissionSun); //make sun glow
    
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos); //move light 1 under sun
    glRotatef(_angle, 0.0f, 1.0f, 0.0f);
    glutSolidSphere(0.8, 40, 32); //draw sun
    
    //save lighting
    glPushAttrib(GL_LIGHTING_BIT);
    //glDisable(GL_LIGHT0);
    glEnable(GL_LIGHT1); //turn on the sun
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, nullv);
    glMaterialfv(GL_FRONT, GL_EMISSION, nullv);
    
    glPushMatrix(); //save state
    
    glRotatef(_angle, 0.0, 1.0, 0.0); //rotate for the planet
    glTranslatef(3.0, 0.0, 0.0);
    glColor3f(0.0, 0.3, 1.0);
    glutSolidSphere(0.2, 12, 10); //draw planet
    
    glRotatef(_angle1, 0.0, 1.0, 0.0); //rotate for the moon
    glTranslatef(0.3, 0.0, 0.0);
    glColor3f(0.5, 0.3, 0.0);
    glutSolidSphere(0.05, 5, 4); //draw moon
    
    glPopMatrix(); //restore state¡
    
    glPopAttrib();
    glPopMatrix(); //restore stat
    
    glutSwapBuffers();
}



void SetupLighting(void)
{
    // Enable lighting
    glEnable(GL_LIGHTING);
    
    // Setup and enable light0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    
    // Setup light1
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
    
    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);
    
    // Set Material properties to follow glColor values
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
}

void SetupRC(void)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    SetupLighting();
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    glShadeModel(GL_SMOOTH);
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
    //SetupRC();
    glutDisplayFunc(drawScene);
    glutKeyboardFunc(handleKeypress);
    glutReshapeFunc(handleResize);
    
    glutTimerFunc(25, update, 0); //Add a timer
    
    glutMainLoop();
    return 0;
}
