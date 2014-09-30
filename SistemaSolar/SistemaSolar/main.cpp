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


int main(int argc, char ** argv) {
    
    //Initializa GLUT and OpenGL
    glutInit(&argc, argv);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGTH);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    
    //Create Window
    glutCreateWindow(WINDOW_TITLE);
    
    //GLUT Callback functions
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
    glVertex2f(-1, -1);
    glColor3f(0.0, 0.0, 1.0);
    glVertex2f(0, 1);
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(1, -1);
    
    glEnd();
    
    glutSwapBuffers();
}
