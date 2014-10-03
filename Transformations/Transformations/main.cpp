//
//  main.cpp
//  Transformations
//
//  Created by Luis Mendoza on 10/3/14.
//  Copyright (c) 2014 LuisMendoza. All rights reserved.
//



#include <iostream>
#include <stdlib.h>

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
}

//Called when the window is resized
void handleResize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}

float _angle = 30.0f;
float _cameraAngle = 0.0f;

//Draws the 3D scene
void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
    glLoadIdentity(); //Reset the drawing perspective
    glRotatef(-_cameraAngle, 0.0f, 1.0f, 0.0f); //Rotate the camera
    glTranslatef(0.0f, 0.0f, -5.0f); //Move forward 5 units
    
    glPushMatrix(); //Save the transformations performed thus far
    glTranslatef(0.0f, -1.0f, 0.0f); //Move to the center of the trapezoid
    glRotatef(_angle, 0.0f, 0.0f, 1.0f); //Rotate about the z-axis
    
    glBegin(GL_QUADS);
    
    //Trapezoid
    glVertex3f(-0.7f, -0.5f, 0.0f);
    glVertex3f(0.7f, -0.5f, 0.0f);
    glVertex3f(0.4f, 0.5f, 0.0f);
    glVertex3f(-0.4f, 0.5f, 0.0f);
    
    glEnd();
    
    glPopMatrix(); //Undo the move to the center of the trapezoid
    glPushMatrix(); //Save the current state of transformations
    glTranslatef(1.0f, 1.0f, 0.0f); //Move to the center of the pentagon
    glRotatef(_angle, 0.0f, 1.0f, 0.0f); //Rotate about the y-axis
    glScalef(0.7f, 0.7f, 0.7f); //Scale by 0.7 in the x, y, and z directions
    
    glBegin(GL_TRIANGLES);
    
    //Pentagon
    glVertex3f(-0.5f, -0.5f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.0f);
    glVertex3f(-0.5f, 0.0f, 0.0f);
    
    glVertex3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);
    
    glVertex3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.5f, 0.0f);
    
    glEnd();
    
    glPopMatrix(); //Undo the move to the center of the pentagon
    glPushMatrix(); //Save the current state of transformations
    glTranslatef(-1.0f, 1.0f, 0.0f); //Move to the center of the triangle
    glRotatef(_angle, 1.0f, 2.0f, 3.0f); //Rotate about the the vector (1, 2, 3)
    
    glBegin(GL_TRIANGLES);
    
    //Triangle
    glVertex3f(0.5f, -0.5f, 0.0f);
    glVertex3f(0.0f, 0.5f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.0f);
    
    glEnd();
    
    glPopMatrix(); //Undo the move to the center of the triangle
    
    glutSwapBuffers();
}

void update(int value) {
    _angle += 2.0f;
    if (_angle > 360) {
        _angle -= 360;
    }
    
    glutPostRedisplay(); //Tell GLUT that the display has changed
    
    //Tell GLUT to call update again in 25 milliseconds
    glutTimerFunc(25, update, 0);
}

int main(int argc, char** argv) {
    //Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(400, 400);
    
    //Create the window
    glutCreateWindow("Transformations and Timers - videotutorialsrock.com");
    initRendering();
    
    //Set handler functions
    glutDisplayFunc(drawScene);
    glutKeyboardFunc(handleKeypress);
    glutReshapeFunc(handleResize);
    
    //glutTimerFunc(25, update, 0); //Add a timer
    
    glutMainLoop();
    return 0;
}

