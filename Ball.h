//
// Created by janek on 02.03.19.
//

#ifndef BALLSONTHREADS_BALL_H
#define BALLSONTHREADS_BALL_H

#include "DirectionGenerator.h"

class Ball {
public:
    void moveX();

    void moveY();

    void move();

    int getCoordinateX() const;

    void setCoordinateX(int coordinateX);

    int getCoordinateY() const;

    void setCoordinateY(int coordinateY);

//    Ball(int coordinateX, int coordinateY, int velocity);

    Ball(int coordinateX, int coordinateY, int random);

private:
    int coordinateX;
    int coordinateY;
    int velocityX;
    int velocityY;
    char ballSign;

public:
    int getVelocityX() const;

    void setVelocityX(int velocityX);

    int getVelocityY() const;

    void setVelocityY(int velocityY);

    void turnVelX();

    void turnVelY();

    void decrementVelX();

    void decrementVelY();
};


#endif //BALLSONTHREADS_BALL_H
