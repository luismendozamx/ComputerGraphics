/*
 * Implementación de la clase de Juego.
 *
 *
 *
 */

#include <cmath>
#include <cstdlib>
#include <math.h>
#include <vector>

#include "game.h"

using namespace std;

namespace {
    const float PI = 3.1415926535f;
    
    const float PADDLE_STEP_TIME = 0.01f;//Tiempo que dura el movimiento de la paleta
    const float TIME_TO_MAXIMUM_SPEED = 0.18f;//Tiempo que toma acelerar la paleta de 0 a su velocidad máxima.
    //The maximum angle formed by the direction at which a ball is hit off of a
    //paddle and the normal direction for the paddle
    const float MAX_PADDLE_BOUNCE_ANGLE_OFFSET = 0.85f * PI / 2;//
    const float PLAYER_MAXIMUM_SPEED = 2.2f;//Velocidad máxima de la paleta
    const float BALL_FADE_IN_TIME = 0.5f;//Tiempo para aparecer una pelota
    const float BALL_FADE_OUT_TIME = 0.5f;//Tiempo para desaparecer una pelota
    const float BALL_RADIUS = 0.04f;//Radio de las pelotas
    const float BALL_SPEED = 1.0f;//velocidad de las pelotas
    const int NUM_BALLS = 2;//Numero ideal de pelotas en juego
    const float GAME_STEP_TIME = 0.01f;//Tiempo en el que se avanza el juego
    
    //Método que regresa un número aleatorio de 0 a <1
    float randomFloat() {
        return (float)rand() / ((float)RAND_MAX + 1);
    }
}

//Inicializar paleta
Paddle::Paddle(float maximumSpeed1) {
    maximumSpeed = maximumSpeed1;
    pos0 = 0.5f;
    dir0 = 0;
    speed0 = 0;
    timeUntilNextStep = 0;
}

//Método que mueve una paleta
void Paddle::step() {
    //Acelera la paleta
    float ds = PADDLE_STEP_TIME * acceleration();
    if (dir0 != 0) {
        speed0 += dir0 * ds;
        if (speed0 > maximumSpeed) {
            speed0 = maximumSpeed;
        }
        else if (speed0 < -maximumSpeed) {
            speed0 = -maximumSpeed;
        }
    }
    else {
        float s = abs(speed0);
        s -= ds;
        if (s < 0) {
            s = 0;
        }
        if (speed0 > 0) {
            speed0 = s;
        }
        else {
            speed0 = -s;
        }
    }
    
    //Mueve la paleta sin salirse de las barreras del cuadrado
    pos0 += PADDLE_STEP_TIME * speed0;
    if (pos0 < BARRIER_SIZE + PADDLE_LENGTH / 2) {
        pos0 = BARRIER_SIZE + PADDLE_LENGTH / 2;
        speed0 = 0;
    }
    else if (pos0 > 1 - BARRIER_SIZE - PADDLE_LENGTH / 2) {
        pos0 = 1 - BARRIER_SIZE - PADDLE_LENGTH / 2;
        speed0 = 0;
    }
}

float Paddle::pos() {
    return pos0;
}

int Paddle::dir() {
    return dir0;
}

float Paddle::speed() {
    return speed0;
}

float Paddle::acceleration() {
    return maximumSpeed / TIME_TO_MAXIMUM_SPEED;
}

void Paddle::setDir(int dir1) {
    if (dir1 < 0) {
        dir0 = -1;
    }
    else if (dir1 > 0) {
        dir0 = 1;
    }
    else {
        dir0 = 0;
    }
}

//Llama al método step() para acelerar la paleta un número determinado de tiempo.
void Paddle::advance(float dt) {
    while (dt > 0) {
        if (timeUntilNextStep < dt) {
            dt -= timeUntilNextStep;
            step();
            timeUntilNextStep = PADDLE_STEP_TIME;
        }
        else {
            timeUntilNextStep -= dt;
            dt = 0;
        }
    }
}

//Constructor de las pelotas.
Ball::Ball(float radius1, float x1, float z1, float angle1) {
    r = radius1;
    x0 = x1;
    z0 = z1;
    angle0 = angle1;
    fadeAmount0 = 0;
    isFadingOut0 = false;
}

float Ball::radius() {
    return r;
}

float Ball::x() {
    return x0;
}

float Ball::z() {
    return z0;
}

float Ball::angle() {
    return angle0;
}

void Ball::setAngle(float angle1) {
    angle0 = angle1;
}

float Ball::fadeAmount() {
    return fadeAmount0;
}

void Ball::fadeOut() {
    isFadingOut0 = true;
}

bool Ball::isFadingOut() {
    return isFadingOut0;
}

void Ball::advance(float dt) {
    if (isFadingOut0) {
        //Fade out
        fadeAmount0 -= dt / BALL_FADE_OUT_TIME;
        if (fadeAmount0 < 0) {
            fadeAmount0 = 0;
        }
    }
    else if (fadeAmount0 < 1) {
        //Fade in
        fadeAmount0 += dt / BALL_FADE_IN_TIME;
        if (fadeAmount0 > 1) {
            dt = (fadeAmount0 - 1) * BALL_FADE_IN_TIME;
            fadeAmount0 = 1;
        }
        else {
            dt = 0;
        }
    }
    
    if (dt <= 0) {
        return;
    }
    
    //Avanza la posición de la pelota
    x0 += dt * cos(angle0) * BALL_SPEED;
    z0 += dt * sin(angle0) * BALL_SPEED;
}





Game::Game(float maximumSpeedForOpponents, int startingScore) {
    if (startingScore > 0) {
        paddles0[0] = new Paddle(PLAYER_MAXIMUM_SPEED);
        for(int i = 1; i < 4; i++) {
            paddles0[i] = new Paddle(maximumSpeedForOpponents);
        }
    }
    else {
        for(int i = 0; i < 4; i++) {
            paddles0[i] = NULL;
        }
    }
    
    for(int i = 0; i < 4; i++) {
        scores[i] = startingScore;
    }
    timeUntilNextStep = 0;
}

Game::~Game() {
    for(int i = 0; i < 4; i++) {
        if (paddles0[i] != NULL) {
            delete paddles0[i];
        }
    }
    for(unsigned int i = 0; i < balls0.size(); i++) {
        delete balls0[i];
    }
}

namespace {
    //Returns whether the point (dx, dz) lies within r units of (0, 0)
    bool intersectsCircle(float dx, float dz, float r) {
        return dx * dx + dz * dz < r * r;
    }
    
    //Returns whether a ball is colliding with a circle that is dx units to the
    //right and dz units inward from it, where r is the sum of the radius of the
    //ball and the radius of the circle and (vx, vz) is the velocity of the ball
    bool collisionWithCircle(float dx, float dz, float r, float vx, float vz) {
        return intersectsCircle(dx, dz, r) && vx * dx + vz * dz > 0;
    }
    
    //Returns the resultant angle when an object traveling at an angle of angle
    //bounces off of a wall whose normal is at an angle of normal.  The returned
    //angle will be between 0 and 2 * PI.  An angle of 0 indicates the positive
    //x direction, and an angle of PI / 2 indicates the positive z direction.
    float reflect(float angle, float normal) {
        angle = 2 * normal - PI - angle;
        while (angle < 0) {
            angle += 2 * PI;
        }
        while (angle > 2 * PI) {
            angle -= 2 * PI;
        }
        return angle;
    }
    
    //Adjusts the ball's angle in response to a collision with a circle at the
    //specified position
    void collideWithCircle(Ball* ball, float x, float z) {
        if (ball->fadeAmount() < 1) {
            return;
        }
        
        float dx = x - ball->x();
        float dz = z - ball->z();
        float normal = atan2(-dz, -dx);
        float newBallAngle = reflect(ball->angle(), normal);
        if (newBallAngle < 0) {
            newBallAngle += 2 * PI;
        }
        else if (newBallAngle > 2 * PI) {
            newBallAngle -= 2 * PI;
        }
        ball->setAngle(newBallAngle);
    }
    
    //Returns whether a paddle at the indicated position has intercepted a ball at
    //the indicated position, where the positions are measured parallel to the
    //direction in which the paddle moves
    bool collisionWithPaddle(float paddlePos, float ballPos) {
        return abs(paddlePos - ballPos) < PADDLE_LENGTH / 2;
    }
    
    //Adjusts the ball's angle in response to a collision with a paddle.  The
    //positions are measured parallel to the direction in which the paddle moves,
    //and the paddle's position is its distance from its center to the corner to
    //its right.
    void collideWithPaddle(Ball* ball,
                         int paddleIndex,
                         float paddlePos,
                         float ballPos) {
        float angle = (1 - paddleIndex) * PI / 2 +
        MAX_PADDLE_BOUNCE_ANGLE_OFFSET *
        (paddlePos - ballPos) / (PADDLE_LENGTH / 2);
        while (angle < 0) {
            angle += 2 * PI;
        }
        while (angle >= 2 * PI) {
            angle -= 2 * PI;
        }
        ball->setAngle(angle);
    }
}

void Game::handleCollisions() {
    for(unsigned int i = 0; i < balls0.size(); i++) {
        Ball* ball = balls0[i];
        
        if (ball->fadeAmount() < 1 || ball->isFadingOut()) {
            continue;
        }
        
        //Ball-barrier collisions
        for(float z = 0; z < 2; z += 1) {
            for(float x = 0; x < 2; x += 1) {
                if (collisionWithCircle(x - ball->x(), z - ball->z(),
                                        ball->radius() + BARRIER_SIZE,
                                        BALL_SPEED * cos(ball->angle()),
                                        BALL_SPEED * sin(ball->angle()))) {
                    collideWithCircle(ball, x, z);
                }
            }
        }
        
        //Ball-ball collisions
        for(unsigned int j = i + 1; j < balls0.size(); j++) {
            Ball* ball2 = balls0[j];
            if (collisionWithCircle(ball2->x() - ball->x(),
                                    ball2->z() - ball->z(),
                                    ball->radius() + ball2->radius(),
                                    BALL_SPEED * (cos(ball->angle()) -
                                                  cos(ball2->angle())),
                                    BALL_SPEED * (sin(ball->angle()) -
                                                  sin(ball2->angle())))) {
                                        collideWithCircle(ball, ball2->x(), ball2->z());
                                        collideWithCircle(ball2, ball->x(), ball->z());
                                    }
        }
        
        //Ball-paddle (and ball-pole) collisions
        int paddleIndex;
        float ballPos;
        if (ball->z() < ball->radius()) {
            paddleIndex = 0;
            ballPos = ball->x();
        }
        else if (ball->x() < ball->radius()) {
            paddleIndex = 1;
            ballPos = 1 - ball->z();
        }
        else if (ball->z() > 1 - ball->radius()) {
            paddleIndex = 2;
            ballPos = 1 - ball->x();
        }
        else if (ball->x() > 1 - ball->radius()) {
            paddleIndex = 3;
            ballPos = ball->z();
        }
        else {
            paddleIndex = -1;
            ballPos = 0;
        }
        
        if (paddleIndex >= 0) {
            if (paddles0[paddleIndex] != NULL) {
                float paddlePos = paddles0[paddleIndex]->pos();
                if (collisionWithPaddle(paddlePos, ballPos)) {
                    collideWithPaddle(ball, paddleIndex, paddlePos, ballPos);
                }
            }
            else {
                float normal = (1 - paddleIndex) * PI / 2;
                float newAngle = reflect(ball->angle(), normal);
                ball->setAngle(newAngle);
            }
        }
    }
}

namespace {
    //Returns the position at which the specified paddle will stop if it
    //immediately starts decelerating
    float stopPos(Paddle* paddle) {
        float d = paddle->speed() * paddle->speed() / paddle->acceleration();
        if (paddle->speed() > 0) {
            return paddle->pos() + d;
        }
        else {
            return paddle->pos() - d;
        }
    }
}

/*
void Game::doAI() {
    for(int i = 1; i < 4; i++) {
        Paddle* paddle = paddles0[i];
        if (paddle == NULL) {
            continue;
        }
        
        //Find the position of the ball that is nearest the paddle's side, and
        //store the result in targetPos
        float closestBallDist = 100;
        float targetPos = 0.5f;
        for(unsigned int j = 0; j < balls0.size(); j++) {
            Ball* ball = balls0[j];
            
            float ballDist;
            float ballPos;
            switch(i) {
                case 1:
                    ballDist = ball->x() - ball->radius();
                    ballPos = 1 - ball->z();
                    break;
                case 2:
                    ballDist = 1 - ball->z() - ball->radius();
                    ballPos = 1 - ball->x();
                    break;
                case 3:
                    ballDist = 1 - ball->x() - ball->radius();
                    ballPos = ball->z();
                    break;
            }
            
            if (ballDist < closestBallDist) {
                targetPos = ballPos;
                closestBallDist = ballDist;
            }
        }
        
        //Move toward targetPos.  Stop so that the ball is in the middle 70% of
        //the paddle.
        if (abs(stopPos(paddle) - targetPos) < 0.7f * (PADDLE_LENGTH / 2)) {
            paddle->setDir(0);
        }
        else if (targetPos < paddle->pos()) {
            paddle->setDir(-1);
        }
        else {
            paddle->setDir(1);
        }
    }
}*/

void Game::step() {
    //Advance the paddles
    for(int i = 0; i < 4; i++) {
        Paddle* paddle = paddles0[i];
        if (paddle != NULL) {
            paddle->advance(GAME_STEP_TIME);
        }
    }
    
    //Advance the balls
    for(unsigned int i = 0; i < balls0.size(); i++) {
        balls0[i]->advance(GAME_STEP_TIME);
    }
    
    //Handle collisions
    handleCollisions();
    
    //Check for balls that have scored on a player
    vector<Ball*> newBalls;
    for(unsigned int i = 0; i < balls0.size(); i++) {
        Ball* ball = balls0[i];
        if (ball->fadeAmount() == 1 && !ball->isFadingOut()) {
            newBalls.push_back(ball);
            
            int scoredOn;
            if (ball->z() < ball->radius() && (ball->angle() > PI)) {
                scoredOn = 0;
            }
            else if (ball->x() < ball->radius() &&
                     (ball->angle() > PI / 2 && ball->angle() < 3 * PI / 2)) {
                scoredOn = 1;
            }
            else if (ball->z() > 1 - ball->radius() && (ball->angle() < PI)) {
                scoredOn = 2;
            }
            else if (ball->x() > 1 - ball->radius() &&
                     (ball->angle() < PI / 2 || ball->angle() > 3 * PI / 2)) {
                scoredOn = 3;
            }
            else {
                scoredOn = -1;
            }
            
            if (scoredOn >= 0 && paddles0[scoredOn] != NULL) {
                scores[scoredOn]--;
                if (scores[scoredOn] == 0) {
                    delete paddles0[scoredOn];
                    paddles0[scoredOn] = NULL;
                }
                
                ball->fadeOut();
            }
        }
        else if (ball->fadeAmount() > 0 || !ball->isFadingOut()) {
            newBalls.push_back(ball);
        }
        else {
            delete ball;
        }
    }
    balls0 = newBalls;
    
    //Check whether the game is over
    bool isGameOver;
    if (paddles0[0] != NULL) {
        isGameOver = true;
        for(int i = 1; i < 4; i++) {
            if (paddles0[i] != NULL) {
                isGameOver = false;
            }
        }
    }
    else {
        isGameOver = true;
    }
    
    if (!isGameOver) {
        //Try to get to NUM_BALLS balls
        while (balls0.size() < (unsigned int)NUM_BALLS) {
            //Try to place a ball at the center of the board
            bool ballFits = true;
            for(unsigned int i = 0; i < balls0.size(); i++) {
                Ball* ball = balls0[i];
                if (intersectsCircle(ball->x() - 0.5f, ball->z() - 0.5f,
                                     2 * BALL_RADIUS)) {
                    ballFits = false;
                    break;
                }
            }
            
            if (ballFits) {
                Ball* ball =
                new Ball(BALL_RADIUS, 0.5f, 0.5f, 2 * PI * randomFloat());
                balls0.push_back(ball);
            }
            else {
                break;
            }
        }
    }
    else {
        for(unsigned int i = 0; i < balls0.size(); i++) {
            balls0[i]->fadeOut();
        }
    }
    
    //Run the AI for the computer-controlled paddles
    //doAI();
}

void Game::setPlayerPaddleDir(int dir) {
    if (paddles0[0] != NULL) {
        paddles0[0]->setDir(dir);
    }
}

int Game::score(int paddleNum) {
    return scores[paddleNum];
}

void Game::advance(float dt) {
    while (dt > 0) {
        if (timeUntilNextStep < dt) {
            dt -= timeUntilNextStep;
            step();
            timeUntilNextStep = PADDLE_STEP_TIME;
        }
        else {
            timeUntilNextStep -= dt;
            dt = 0;
        }
    }
}

Paddle** Game::paddles() {
    return paddles0;
}

vector<Ball*> Game::balls() {
    return balls0;
}










