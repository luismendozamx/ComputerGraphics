/*
 * Implementación de la clase que dibuja el juego. Utiliza la clase Game para mantener la lógica de juego.
 *
 * Luis Mendoza
 * Clase: Gráficas por Computadora
 * Fecha: 12/12/2014
 */


#include <math.h>
#include <sstream>
#include <vector>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "game.h"
#include "gamedrawer.h"
#include "imageloader.h"

using namespace std;

namespace {
    const float PI = 3.1415926535f;
    
    const float STEP_TIME = 0.01f;//Cantidad de tiempo que avanza el juego con cada ciclo
    const float PADDLE_FADE_OUT_TIME = 0.0f;
    const int NUM_BARRIER_POINTS = 30;//Numero de puntos usados para aproximar los cilindros
    const int NUM_POLE_POINTS = 6;
    const float BARRIER_HEIGHT = BARRIER_SIZE;
    const float POLE_RADIUS = 0.02f;
    const float POLE_HEIGHT = 0.07f;
    const float PADDLE_OFFSET = -0.032f;//Número de unidades por movimiento
    const float FIELD_TEXTURE_TIME = 8.0f;
    const float FIELD_TEXTURE_SIZE = 0.7f;
    const float FIELD_ALPHA = 0.8f;
    
    //Converrtir la imagen en textura
    GLuint loadTexture(Image *image) {
        GLuint textureId;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGB,
                     image->width, image->height,
                     0,
                     GL_RGB,
                     GL_UNSIGNED_BYTE,
                     image->pixels);
        return textureId;
    }
}

GameDrawer::GameDrawer() {
    game = NULL;
    playerPaddleDir = 0;
    
    //Empieza un nuevo juego
    startNewGame(0, 0);
    
    fieldTextureOffset = 0;
    
    setupBarriers();
    setupPole();
    
    Image* image = loadBMP("/Users/luismendoza/Sandbox/ComputerGraphics/proyectofinal/proyectofinal/court.bmp");
    courtTextureId = loadTexture(image);
    delete image;
    
    image = loadBMP("/Users/luismendoza/Sandbox/ComputerGraphics/proyectofinal/proyectofinal/field.bmp");
    fieldTextureId = loadTexture(image);
    delete image;
}

GameDrawer::~GameDrawer() {
    delete game;
}

void GameDrawer::setGame(Game* game1) {
    if (game != NULL) {
        delete game;
    }
    
    game = game1;
    game->setPlayerPaddleDir(playerPaddleDir);
    timeUntilNextStep = 0;
    
    isGameOver0 = (game->score(0) == 0);
    waitingForFirstGame = isGameOver0;
    
    for(int i = 0; i < 4; i++) {
        if(i != 0 || i != 2){
            animTimes[i] = 0;
            
            if (!isGameOver0) {
                paddleFadeAmounts[i] = 1;
            }
            else {
                paddleFadeAmounts[i] = 0;
            }
            
            Paddle* paddle = game->paddles()[i];
            if (paddle != NULL) {
                oldPaddlePos[i] = game->paddles()[i]->pos();
            }
            else {
                oldPaddlePos[i] = 0.5f;
            }
        }else{
            animTimes[i] = 0;
            paddleFadeAmounts[i] = 0;
        }
    }
}

void GameDrawer::setupBarriers() {
    Image* image = loadBMP("/Users/luismendoza/Sandbox/ComputerGraphics/proyectofinal/proyectofinal/vtr.bmp");
    GLuint textureId = loadTexture(image);
    delete image;
    
    GLuint barrierDisplayListId = glGenLists(1);
    glNewList(barrierDisplayListId, GL_COMPILE);
    
    //Dibuja el círculo de arriba
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    GLfloat materialColor[] = {1, 1, 1, 1};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);
    
    glNormal3f(0, 1, 0);
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0, BARRIER_HEIGHT, 0);
    for(int i = NUM_BARRIER_POINTS; i >= 0; i--) {
        float angle = 2 * PI * (float)i / (float)NUM_BARRIER_POINTS;
        glTexCoord2f(-cos(angle) / 2 + 0.5f, sin(angle) / 2 + 0.5f);
        glVertex3f(BARRIER_SIZE * cos(angle),
                   BARRIER_HEIGHT,
                   BARRIER_SIZE * sin(angle));
    }
    glEnd();
    
    //Dibuja el de abajo
    GLfloat materialColor2[] = {1.0f, 0.0f, 0.0f, 1};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor2);
    
    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0f, 0.0f, 0.0f);
    glNormal3f(0, -1, 0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, 0);
    for(int i = 0; i <= NUM_BARRIER_POINTS; i++) {
        float angle = 2 * PI * (float)i / (float)NUM_BARRIER_POINTS;
        glVertex3f(BARRIER_SIZE * cos(angle), 0, BARRIER_SIZE * sin(angle));
    }
    glEnd();
    
    //Dibuja cilindro
    glBegin(GL_QUAD_STRIP);
    for(int i = 0; i <= NUM_BARRIER_POINTS; i++) {
        float angle = 2 * PI * ((float)i - 0.5f) / (float)NUM_BARRIER_POINTS;
        glNormal3f(cos(angle), 0, sin(angle));
        float angle2 = 2 * PI * (float)i / (float)NUM_BARRIER_POINTS;
        glVertex3f(BARRIER_SIZE * cos(angle2), 0, BARRIER_SIZE * sin(angle2));
        glVertex3f(BARRIER_SIZE * cos(angle2),
                   BARRIER_HEIGHT,
                   BARRIER_SIZE * sin(angle2));
    }
    glEnd();
    glEndList();
    
    //Lista con las cuatro esquinas
    barriersDisplayListId = glGenLists(1);
    glNewList(barriersDisplayListId, GL_COMPILE);
    glDisable(GL_COLOR_MATERIAL);
    
    //Material
    GLfloat materialSpecular[] = {1, 1, 1, 1};
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, 15.0f);
    
    for(float z = 0; z < 2; z++) {
        for(float x = 0; x < 2; x++) {
            glPushMatrix();
            glTranslatef(x, 0, z);
            glCallList(barrierDisplayListId);
            glPopMatrix();
        }
    }
    glEnable(GL_COLOR_MATERIAL);
    
    //Deshabilita Material
    GLfloat materialColor3[] = {1, 1, 1, 1};
    GLfloat materialSpecular2[] = {0, 0, 0, 1};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor3);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular2);
    
    glEndList();
}

void GameDrawer::setupPole() {
    poleDisplayListId = glGenLists(1);
    glNewList(poleDisplayListId, GL_COMPILE);
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.0f, 0.8f, 0.0f);
    
    //Círculo izquierdo
    glNormal3f(-1, 0, 0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(BARRIER_SIZE, POLE_HEIGHT, -POLE_RADIUS);
    for(int i = NUM_POLE_POINTS; i >= 0; i--) {
        float angle = 2 * PI * (float)i / (float)NUM_POLE_POINTS;
        glVertex3f(BARRIER_SIZE,
                   POLE_HEIGHT + POLE_RADIUS * cos(angle),
                   POLE_RADIUS * (sin(angle) - 1));
    }
    glEnd();
    
    //Círculo derecho
    glNormal3f(1, 0, 0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(1 - BARRIER_SIZE, POLE_HEIGHT, -POLE_RADIUS);
    for(int i = 0; i <= NUM_POLE_POINTS; i++) {
        float angle = 2 * PI * (float)i / (float)NUM_POLE_POINTS;
        glVertex3f(1 - BARRIER_SIZE,
                   POLE_HEIGHT + POLE_RADIUS * cos(angle),
                   POLE_RADIUS * (sin(angle) - 1));
    }
    glEnd();
    
    //Dibuja Cilindro
    glBegin(GL_QUAD_STRIP);
    for(int i = 0; i <= NUM_POLE_POINTS; i++) {
        float angle = 2 * PI * ((float)i - 0.5f) / (float)NUM_POLE_POINTS;
        glNormal3f(0, cos(angle), sin(angle));
        float angle2 = 2 * PI * (float)i / (float)NUM_POLE_POINTS;
        glVertex3f(1 - BARRIER_SIZE,
                   POLE_HEIGHT + POLE_RADIUS * cos(angle2),
                   POLE_RADIUS * (sin(angle2) - 1));
        glVertex3f(BARRIER_SIZE,
                   POLE_HEIGHT + POLE_RADIUS * cos(angle2),
                   POLE_RADIUS * (sin(angle2) - 1));
    }
    glEnd();
    
    glEndList();
}

void GameDrawer::step() {
    //Avanza el juego
    game->advance(STEP_TIME);
    
    //Mueve el terreno
    fieldTextureOffset += STEP_TIME / FIELD_TEXTURE_TIME;
    while (fieldTextureOffset > FIELD_TEXTURE_SIZE) {
        fieldTextureOffset -= FIELD_TEXTURE_SIZE;
    }
    
    bool opponentAlive = false;
    for(int i = 0; i < 4; i++) {
        Paddle* paddle = game->paddles()[i];
        
        if (paddle != NULL) {
            oldPaddlePos[i] = paddle->pos();
        }
        
        //Actualiza opacidad
        if (paddle == NULL) {
            paddleFadeAmounts[i] -= STEP_TIME / PADDLE_FADE_OUT_TIME;
            if (paddleFadeAmounts[i] < 0) {
                if (i == 0) {
                    isGameOver0 = true;
                }
                paddleFadeAmounts[i] = 0;
            }
            else if (i != 0) {
                opponentAlive = true;
            }
        }
        else if (i != 0) {
            opponentAlive = true;
        }
    }
    
    if (!opponentAlive) {
        isGameOver0 = true;
    }
}

void GameDrawer::setupLighting() {
    GLfloat ambientLight[] = {0.2f, 0.2f, 0.2f, 1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
    
    //Una luz sobre cada esquina
    int index = 0;
    for(float z = 0; z < 2; z += 1) {
        for(float x = 0; x < 2; x += 1) {
            glEnable(GL_LIGHT0 + index);
            GLfloat lightColor[] = {0.2f, 0.2f, 0.2f, 1};
            GLfloat lightPos[] = {x, 1.5f, z, 1};
            glLightfv(GL_LIGHT0 + index, GL_DIFFUSE, lightColor);
            glLightfv(GL_LIGHT0 + index, GL_SPECULAR, lightColor);
            glLightfv(GL_LIGHT0 + index, GL_POSITION, lightPos);
            index++;
        }
    }
}

void GameDrawer::drawPaddlesAndPoles(bool isReflected) {

        glEnable(GL_NORMALIZE);
        for(int i = 0; i < 4; i++) {
            Paddle* paddle = game->paddles()[i];
            
            //Rotar
            glPushMatrix();
            switch(i) {
                case 1:
                    glTranslatef(0, 0, 1);
                    glRotatef(90, 0, 1, 0);
                    break;
                case 2:
                    glTranslatef(1, 0, 1);
                    glRotatef(180, 0, 1, 0);
                    break;
                case 3:
                    glTranslatef(1, 0, 0);
                    glRotatef(270, 0, 1, 0);
                    break;
            }
            
            if (paddle != NULL || paddleFadeAmounts[i] > 0) {
                //Draw the paddle
                glPushMatrix();
                
                float paddlePos;
                if (paddle != NULL) {
                    paddlePos = paddle->pos();
                }
                else {
                    paddlePos = oldPaddlePos[i];
                }
                glTranslatef(paddlePos, 0.055f, PADDLE_OFFSET);
                if (paddle == NULL) {
                    //Used for the shrinking effect, whereby paddles shrink
                    //until they disappear when they are eliminated from play
                    glTranslatef(0, -0.055f * (1 - paddleFadeAmounts[i]), 0);
                    glScalef(paddleFadeAmounts[i],
                             paddleFadeAmounts[i],
                             paddleFadeAmounts[i]);
                }
                
                glRotatef(-90, 0, 1, 0);
                glRotatef(-90, 1, 0, 0);
                glScalef(0.05f, 0.05f, 0.05f);
                
                glColor3f(1, 1, 1);
                glutSolidSphere(1.5, 10, 6);
                glPopMatrix();
            }
            
            if (paddle == NULL) {
                //Dibuja pared
                if (isReflected) {
                    glDisable(GL_NORMALIZE);
                }
                glCallList(poleDisplayListId);
                if (isReflected) {
                    glEnable(GL_NORMALIZE);
                }
            }
            
            glPopMatrix();
        }
}

void GameDrawer::drawBarriers(bool isReflected) {
    if (isReflected) {
        glEnable(GL_NORMALIZE);
    }
    else {
        glDisable(GL_NORMALIZE);
    }
    glCallList(barriersDisplayListId);
}

void GameDrawer::drawBalls(bool isReflected) {
    if (isReflected) {
        glEnable(GL_NORMALIZE);
    }
    else {
        glDisable(GL_NORMALIZE);
    }
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    
    vector<Ball*> balls = game->balls();
    for(unsigned int i = 0; i < balls.size(); i++) {
        Ball* ball = balls[i];
        
        if (ball->fadeAmount() < 1) {
            glEnable(GL_BLEND);
            glColor4f(0.75f, 0.75f, 0.75f, ball->fadeAmount());
        }
        else {
            glColor3f(0.75f, 0.75f, 0.75f);
        }
        
        glPushMatrix();
        glTranslatef(ball->x(), ball->radius() + 0.01f, ball->z());
        glutSolidSphere(ball->radius(), 10, 6);
        glPopMatrix();
        
        if (ball->fadeAmount() < 1) {
            glDisable(GL_BLEND);
        }
    }
}

void GameDrawer::drawObjects(bool isReflected) {
    drawPaddlesAndPoles(isReflected);
    drawBarriers(isReflected);
    drawBalls(isReflected);
}

void GameDrawer::drawCourt() {
    //Altura sobre el terreno
    float height = 0.01f;
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, courtTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glDisable(GL_NORMALIZE);
    glColor3f(1, 1, 1);
    glNormal3f(0, 1, 0);
    glBegin(GL_QUADS);
    glTexCoord2f(1, 0);
    glVertex3f(0, height, 0);
    glTexCoord2f(1, 1);
    glVertex3f(0, height, 1);
    glTexCoord2f(0, 1);
    glVertex3f(1, height, 1);
    glTexCoord2f(0, 0);
    glVertex3f(1, height, 0);
    glEnd();
}

void GameDrawer::drawField() {
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, fieldTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glDisable(GL_NORMALIZE);
    glEnable(GL_BLEND);
    glColor4f(1, 1, 1, FIELD_ALPHA);
    glNormal3f(0, 1, 0);
    
    glBegin(GL_QUADS);
    glTexCoord2f(200 / FIELD_TEXTURE_SIZE,
                 -fieldTextureOffset / FIELD_TEXTURE_SIZE);
    glVertex3f(-100, 0, -100);
    glTexCoord2f(200 / FIELD_TEXTURE_SIZE,
                 (200 - fieldTextureOffset) / FIELD_TEXTURE_SIZE);
    glVertex3f(-100, 0, 100);
    glTexCoord2f(0, (200 - fieldTextureOffset) / FIELD_TEXTURE_SIZE);
    glVertex3f(100, 0, 100);
    glTexCoord2f(0, -fieldTextureOffset / FIELD_TEXTURE_SIZE);
    glVertex3f(100, 0, -100);
    glEnd();
    
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}


void GameDrawer::draw() {
    //Dibujar
    glCullFace(GL_BACK);
    setupLighting();
    drawField();
    drawCourt();
    drawObjects(false);
}

void GameDrawer::advance(float dt) {
    while (dt > 0) {
        if (timeUntilNextStep < dt) {
            dt -= timeUntilNextStep;
            step();
            timeUntilNextStep = STEP_TIME;
        }
        else {
            timeUntilNextStep -= dt;
            dt = 0;
        }
    }
}

void GameDrawer::setPlayerPaddleDir(int dir) {
    playerPaddleDir = dir;
    game->setPlayerPaddleDir(dir);
}

bool GameDrawer::isGameOver() {
    return isGameOver0;
}

void GameDrawer::startNewGame(float maximumSpeedForOpponents,
                              int startingScore) {
    setGame(new Game(maximumSpeedForOpponents, startingScore));
}

void initGameDrawer() {
}

void cleanupGameDrawer() {
}










