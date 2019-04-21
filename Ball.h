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

    Ball(int coordinateX, int coordinateY, int random);

    void incrementCoordY();
    void decrementCoordY();
private:
    double velocityX;
    double velocityY;
    int coordinateX;
    int coordinateY;
    bool isInQueue;
public:
    bool isItInQueue() const;

    void setIsInQueue(bool isInQueue);

    double getVelocityX() const;

    void setVelocityX(double velocityX);

    double getVelocityY() const;

    void setVelocityY(double velocityY);

    void turnVelX();

    void turnVelY();

    void decrementVelX();

    void decrementVelY();

    void decreaseVelX(double);
};


#endif //BALLSONTHREADS_BALL_H
