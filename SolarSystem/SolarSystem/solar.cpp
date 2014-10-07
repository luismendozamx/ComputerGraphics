//solar.cpp
// solar system...

#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

/*
 
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

void GetKey();

void Rotate()
{
	day += 2;
	if(day > 360)
		day = day - 360;
	year += .1;
	if(year > 360)
		year = year - 360;
	glutPostRedisplay();
}

void DrawOrbit(GLfloat rad, GLfloat inc)
{
	glPushMatrix();

	GLfloat y = 0.0;
	glColor3f(0.5, 0.5, 0.5);

	glBegin(GL_POINTS);
	for(GLfloat angle = 0; angle <= 360; angle+=inc)
	{
		GLfloat x = rad*sin((angle)*3.1415926535/180);
		GLfloat z = rad*cos((angle)*3.1415926535/180);

		glVertex3f(x, y, z);
	}
	glEnd();

	glPopMatrix();
}
void BluePlanet(void)
{
	DrawOrbit(5.5, 1);

	glRotatef((GLfloat) year*6, 0.0, 1.0, 0.0); //rotate for the planet
	glTranslatef(5.5, 0.0, 0.0);
	glColor3f(0.0, 0.3, 1.0);
	glutSolidSphere(0.2, 10, 8); //draw planet

	glRotatef((GLfloat) day*2, 0.0, 1.0, 0.0); //rotate for the moon
	glTranslatef(0.5, 0.0, 0.0);
	glColor3f(0.5, 0.3, 0.0);
	glutSolidSphere(0.05, 5, 4); //draw moon

}

void GreenPlanet(void)
{
	DrawOrbit(8.5, 1);

	glRotatef((GLfloat) year*5, 0.0, 1.0, 0.0); //rotate for the planet
	glTranslatef(8.5, 0.0, 0.0);
	glColor3f(0.0, 1.0, 0.0);
	glutSolidSphere(0.3, 20, 16); //draw planet

	glRotatef((GLfloat) day*2, 0.0, 1.0, 0.0); //rotate for the moon
	glTranslatef(0.5, 0.0, 0.0);
	glColor3f(0.5, 0.3, 0.0);
	glutSolidSphere(0.1, 5, 4); //draw moon
}

void PurplePlanet(void)
{
	DrawOrbit(10.5, 1);

	glRotatef((GLfloat) year*3, 0.0, 1.0, 0.0); //rotate for the planet
	glTranslatef(10.5, 0.0, 0.0);
	glColor3f(1.0, 0.01, 1.0);
	glutSolidSphere(0.37, 20, 16); //draw planet

	glPushMatrix();
	glRotatef((GLfloat) day*2, 0.0, 1.0, 0.0); //rotate for the moon
	glTranslatef(0.6, 0.0, 0.0);
	glColor3f(0.5, 0.3, 0.0);
	glutSolidSphere(0.1, 5, 4); //draw moon

	glPopMatrix();
	glRotatef((GLfloat) day*3, 0.0, 1.0, 0.0); //rotate for the moon
	glTranslatef(1.2, 0.0, 0.0);
	glColor3f(0.5, 0.3, 0.0);
	glutSolidSphere(0.1, 5, 4); //draw moon
}

void RedPlanet(void)
{
	DrawOrbit(16.5, .5);

	glRotatef((GLfloat) year*2, 0.0, 1.0, 0.0); //rotate for the planet
	glTranslatef(16.5, 0.0, 0.0);
	glColor3f(1.0, 0.01, 0.0);
	glutSolidSphere(0.7, 20, 16); //draw planet

	glPushMatrix();
	glRotatef((GLfloat) day*3, 0.0, 1.0, 0.0); //rotate for the moon
	glTranslatef(1.5, 0.0, 0.0);
	glColor3f(0.5, 0.3, 0.0);
	glutSolidSphere(0.15, 5, 4); //draw moon

	glPopMatrix();
	glPushMatrix();
	glRotatef((GLfloat) day*2, 0.0, 1.0, 0.0); //rotate for the moon
	glTranslatef(2.1, 0.0, 0.0);
	glColor3f(0.5, 0.3, 0.0);
	glutSolidSphere(0.15, 5, 4); //draw moon

	glPopMatrix();
	glRotatef((GLfloat) day, 0.0, 1.0, 0.0); //rotate for the moon
	glTranslatef(2.7, 0.0, 0.0);
	glColor3f(0.5, 0.3, 0.0);
	glutSolidSphere(0.2, 5, 4); //draw moon
}

void WhitePlanet(void)
{
	DrawOrbit(20.5, .3);

	glRotatef((GLfloat) year, 0.0, 1.0, 0.0); //rotate for the planet
	glTranslatef(20.5, 0.0, 0.0);
	glColor3f(0.9, 0.9, 0.9);
	glutSolidSphere(0.6, 20, 16); //draw planet


	glRotatef(15.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat) day*4, 0.0, 1.0, 0.0); //rotate for the moon
	glTranslatef(1.2, 0.0, 0.0);
	glColor3f(0.5, 0.3, 0.1);
	glutSolidSphere(0.1, 5, 4); //draw moon
}

void RenderScene(void)
{

	Rotate();
	GetKey();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//enable light 0
	glEnable(GL_LIGHT0);

	//set the viewing transformation
	gluLookAt(xdist, ydist, zdist,  xlook, ylook, zlook,  xup, yup, zup);

	glPushMatrix(); //save state
	glTranslatef(0.0, 0.0, -5.0); //translate out to sun

	//set specular reflectivity with low shine
	glColor4f(1.0, 0.8, 0.0, 1.0);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, 3);
	glMaterialfv(GL_FRONT, GL_EMISSION, emissionSun); //make sun glow

	glLightfv(GL_LIGHT1, GL_POSITION, lightPos); //move light 1 under sun
	glutSolidSphere(1.1, 40, 32); //draw sun

	//save lighting
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHT0);
	glEnable(GL_LIGHT1); //turn on the sun

	glMaterialfv(GL_FRONT, GL_SPECULAR, nullv);
	glMaterialfv(GL_FRONT, GL_EMISSION, nullv);

	glutSolidSphere(0.05, 5, 4); //draw moon this fixes the lit orbit problem....I don't know why

	glPushMatrix(); //save state
	BluePlanet();
	glPopMatrix(); //restore state

	glPushMatrix(); //save state
	GreenPlanet();
	glPopMatrix(); //restore state

	glPushMatrix(); //save state
	PurplePlanet();
	glPopMatrix(); //restore state

	glPushMatrix(); //save state
	RedPlanet();
	glPopMatrix(); //restore state

	glPushMatrix(); //save state
	WhitePlanet();
	glPopMatrix(); //restore state

//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


//	glColor4f(1.0, 1.0, 0.0, .8);
//	glMaterialfv(GL_FRONT, GL_EMISSION, emissionSun);
//	glutSolidSphere(1.1, 40, 32); //draw sun
//	glDisable(GL_BLEND);

	glPopAttrib();
	glPopMatrix(); //restore state

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

void Reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//a very interesting function, much easier than doing aspect ratios by hand
	gluPerspective(40.0, (GLfloat)w/(GLfloat)h, 1.0, 400.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GetKey(void)
{
 
	if(GetAsyncKeyState(VK_LEFT))
	{
		xdist += (cos((rot+90)*3.1415926535/180))/2;
		zdist += -(sin((rot+90)*3.1415926535/180))/2;
	}

	if(GetAsyncKeyState(VK_RIGHT))
	{
		xdist += (cos((rot-90)*3.1415926535/180))/2;
		zdist += -(sin((rot-90)*3.1415926535/180))/2;
	}

	if(GetAsyncKeyState(VK_UP))
	{
		xdist += (cos(rot*3.1415926535/180)* cos(pitch*3.1415926535/180))/2;
		zdist += -(sin(rot*3.1415926535/180) * cos(pitch*3.1415926535/180))/2;
		ydist += sin(pitch*3.1415926535/180);
	}

	if(GetAsyncKeyState(VK_DOWN))
	{
		xdist -= (cos(rot*3.1415926535/180)* cos(pitch*3.1415926535/180))/2;
		zdist -= -(sin(rot*3.1415926535/180) * cos(pitch*3.1415926535/180))/2;
		ydist -= sin(pitch*3.1415926535/180);
	}

	if(GetAsyncKeyState(0x41)) //a
	{
		xdist += (cos(rot*3.1415926535/180)* cos((pitch+90)*3.1415926535/180));
		zdist += -(sin(rot*3.1415926535/180) * cos((pitch+90)*3.1415926535/180));
		ydist += sin((pitch+90)*3.1415926535/180);
	}
	if(GetAsyncKeyState(0x5a)) //z
	{
		xdist -= (cos(rot*3.1415926535/180)* cos((pitch+90)*3.1415926535/180));
		zdist -= -(sin(rot*3.1415926535/180) * cos((pitch+90)*3.1415926535/180));
		ydist -= sin((pitch+90)*3.1415926535/180);
	}

	xlook = xdist + (cos(rot*3.1415926535/180) * cos(pitch*3.1415926535/180));
	zlook = zdist - (sin(rot*3.1415926535/180) * cos(pitch*3.1415926535/180));
	ylook = ydist + sin(pitch*3.1415926535/180);

	xup = cos(rot*3.1415926535/180) * cos((pitch+90)*3.1415926535/180);
	zup = -sin(rot*3.1415926535/180) * cos((pitch+90)*3.1415926535/180);
	yup = sin((pitch+90)*3.1415926535/180);

	glutPostRedisplay();
 
}

void PassiveMouse(int x, int y)
{
	if( (lastx - x) >50 || (lastx - x) <-50 ||
		(lasty - y) >50 || (lasty - y) <-50 )
	{
		lastx = x;
		lasty = y;
	}

	rot += (((GLfloat)lastx - (GLfloat)x)/2);
	lastx = x;

	if(rot>360)
		rot-=360;
	if(rot<0)
		rot+=360;

	pitch += (((GLfloat)lasty - (GLfloat)y))/2;
	lasty = y;

	if(pitch > 90)
		pitch = 90.0;
	if(pitch<-90)
		pitch = -90.0;

	glutPostRedisplay();
}

/*
void main(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("solar system");

	SetupRC();
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(Reshape);
	glutPassiveMotionFunc(PassiveMouse);

	glutMainLoop();
}
*/