//
//  main.cpp
//  Window
//
//  Created by Luis Mendoza on 9/9/14.
//  Copyright (c) 2014 LuisMendoza. All rights reserved.
//

#include <iostream>

#include <OpenGL/gl.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <GLUT/glut.h>

using namespace std;

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const char* WINDOW_TITLE = "Gráficas";

void display();

int main(int argc, char ** argv)
{
    //Initialize GLUT and OpenGL
    glutInit(&argc, argv);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    
    //Create Window
    glutCreateWindow(WINDOW_TITLE);
    
    //Set up Glut callback function
    glutDisplayFunc(display);
    
    //enter main loop
    glutMainLoop();
    
    return 0;
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    glutSwapBuffers();
}

