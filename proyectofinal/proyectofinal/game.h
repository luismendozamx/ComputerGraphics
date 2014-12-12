
/* 
 * El juego es un cuadrado con esquinas en (0, 0), (0, 1), (1, 0), y (1, 1) sobre el plano x-z.
 * Hay pelotas y una paleta de longitud PADDLE_LENGTH. En 3 de los lados del cuadrado hay paredes,
 * de éstas rebotan las pelotas que aparecen y el jugador debe evitar que salgan del cuadrado.
 * Para moverse utiliza las flechas izquierda y derecha.
 *
 *
 */

#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <vector>

//Longitud de paleta
const float PADDLE_LENGTH = 0.2f;
//Radio de las esquinas del cuadrado
const float BARRIER_SIZE = 0.06f;

//Clase que representa una paleta
class Paddle {
private:
    float maximumSpeed; //Velocidad de movimiento
    float pos0; //Distancia del centro de la paleta a la esquina a su derecha.
    int dir0;//-1 acelera a la derecha, 1 a la izquierda, 0 está frenando
    float speed0; //Velocidad
    float timeUntilNextStep; //The amount of game time until the next call to step()
    void step();//Cambia el estado de la paleta durante un periodo corto
    
public:
    Paddle(float maximumSpeed1);//Constructor que recibe la velocidad máxima
    
    float pos();//Regresa la distancia a la esquina derecha
    int dir();//Regresa negativo si está acelerando a la izquierda, positivo a la derecha y 0 si está frenando
    float speed();//Regresa la velocidad de la paleta
    float acceleration();//Regresa la aceleración
    void setDir(int dir1);//Define la dirección con las reglas antes mencionadas.
    void advance(float dt);//Advances the state of the paddle by the specified amount of time
};

//Clase que representa una pelota
class Ball {
private:
    float r;//Radio de la pelota
    float x0;//Coordenada x
    float z0;//Coordenada Z
    //The angle at which the ball is traveling.  An angle of 0 indicates the
    //positive x direction, while an angle of PI / 2 indicates the positive
    //z direction.  The angle is always between 0 and 2 * PI.
    float angle0;//Ángulo al que viaja la pelota.
    float fadeAmount0;//Valor de 0 a 1 para su opacidad al aparecer. =! 1 cuando está apareciendo o desapareciendo.
    bool isFadingOut0;
    
public:
    //Constructor que recibe el radio, la posición y el ángulo en que se moverá
    Ball(float radius1, float x1, float z1, float angle1);
    
    float radius();//Regresa el radio
    float x();//regresa la coordenada x
    float z();//regresa la coordenada z
    float angle();//regrea el ángulo de movimiento
    void setAngle(float angle1);//Define el ángulo de movimiento
    float fadeAmount();//Regresa la opacidad
    void fadeOut();//Hace que la bola desaparezca
    bool isFadingOut();
    void advance(float dt);//Mueve la pelota por un tiempo determinado
};

//Clase que controla el estado del juego.
class Game {
private:
    Paddle* paddles0[4];//Arreglo que contiene las paletas. En este caso solo usamos una, pero podrían ser una en cada lado.
    std::vector<Ball*> balls0;//Un vector de todas las pelotas que están en el cuadrado.
    int scores[4];//Puntaje de los jugadores, otra vez, podría haber más de uno.
    float timeUntilNextStep;//Tiempo hasta el siguiente step.
    void handleCollisions();//Manejador de colisiones
    //void doAI();
    void step();//Avanza el estado del juego.
public:
    //Constructor del juego que resive la velocidad máxima y el puntaje inicial
    Game(float maximumSpeedForOpponents, int startingScore);
    ~Game();//Desctructor de la Clase.
    
    void setPlayerPaddleDir(int dir);//Cambia la dirección el la que se mueve la paleta
    int score(int paddleNum);//Regresa el puntaje
    void advance(float dt);//Avanza el estado del juego
    Paddle** paddles();//Regresa las paletas existentes
    std::vector<Ball*> balls();//Regresa las pelotas existentes
};


#endif
