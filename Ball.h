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
    double velocityX;
    double velocityY;

public:
    double getVelocityX() const;

    void setVelocityX(double velocityX);

    double getVelocityY() const;

    void setVelocityY(double velocityY);

    void turnVelX();

    void turnVelY();

    void decrementVelX();

    void decrementVelY();

    void multiplyVelY(double multiplier);
};


#endif //BALLSONTHREADS_BALL_H
