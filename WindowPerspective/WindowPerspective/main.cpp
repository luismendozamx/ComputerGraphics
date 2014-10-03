//
//  main.cpp
//  SistemaSolar
//
//  Created by Luis Mendoza on 9/30/14.
//  Copyright (c) 2014 LuisMendoza. All rights reserved.
//

#include <iostream>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>

using namespace std;

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGTH = 720;
const char* WINDOW_TITLE = "OpenGL Window";

//Functions to implement
void display();
void reshape(int width, int height);


int main(int argc, char ** argv) {
    
    //Initializa GLUT and OpenGL
    glutInit(&argc, argv);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGTH);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    
    //Create Window
    glutCreateWindow(WINDOW_TITLE);
    
    //GLUT Callback functions
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    
    //Enter Main Loop
    glutMainLoop();
    
    return 0;
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    glBegin(GL_TRIANGLES);
    
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-1, -1, -10);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0, 1, -10);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(1, -1, -10);
    
    glEnd();
    
    glutSwapBuffers();
}

void reshape(int width, int height){
    
    //Evitar división entre 0.
    if(height == 0){
        height = 1;
    }
    
    float ratio = (float) width / height;
    
    //Cambiar modo de proyección
    glMatrixMode(GL_PROJECTION);
    
    glLoadIdentity();
    //Ajustar el viewport
    glViewport(0, 0, width, height);
    gluPerspective(45, ratio, 0.001, 1000);
    
    glMatrixMode(GL_MODELVIEW);
    
}
