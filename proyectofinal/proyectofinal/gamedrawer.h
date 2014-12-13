/*
 * Definición de la clase que dibuja el juego. Utiliza la clase Game para mantener la lógica de juego.
 *
 * Luis Mendoza
 * Clase: Gráficas por Computadora
 * Fecha: 12/12/2014
 */



#ifndef GAME_DRAWER_H_INCLUDED
#define GAME_DRAWER_H_INCLUDED

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class Game;
class MD2Model;

//Mantiene el estado del juego mediante un objeto Game
class GameDrawer {
private:
    
    Game* game;//Objeto game
    GLuint barriersDisplayListId;//Display list para las esquinas
    GLuint poleDisplayListId;//Display list para las paredes
    GLuint courtTextureId;//Textura del piso
    GLuint fieldTextureId;//Textura del terreno
    float paddleFadeAmounts[4];
    float animTimes[4];//Tiempos de animación
    float oldPaddlePos[4];//Última posición de una paleta
    float fieldTextureOffset;//Distancia que se mueve el terreno
    bool isGameOver0;
    bool waitingForFirstGame;
    int playerPaddleDir;//Dirección en que se mueve la paleta del jugador
    float timeUntilNextStep;
    
    void setGame(Game* game);
    void setupBarriers();
    void setupPole();
    void step();
    
    void setupLighting();//Luces
    void drawPaddlesAndPoles(bool isReflected);//Dibuja paletas y paredes
    void drawBarriers(bool isReflected);//Dibuja esquinas
    void drawBalls(bool isReflected);//Dibuja pelotas
    void drawObjects(bool isReflected);
    void drawCourt();
    void drawField();
    
    void drawWinner();
public:
    GameDrawer();
    ~GameDrawer();
    
    void draw();//Dibuja el juego
    void advance(float dt);//Avanza el tiempo
    void setPlayerPaddleDir(int dir);
    bool isGameOver();
    void startNewGame(float maximumSpeedForOpponents, int startingScore);
};

//Inicialización
void initGameDrawer();
//Libera la memoria al terminar el juego
void cleanupGameDrawer();










#endif
