//
// Created by janek on 02.03.19.
//

#ifndef BALLSONTHREADS_BALL_H
#define BALLSONTHREADS_BALL_H


class Ball {
public:
    void moveX(int);
    void moveY(int);
    void move(int, int);

    int getCoordinateX() const;

    void setCoordinateX(int  coordinateX);

    int getCoordinateY() const;

    void setCoordinateY(int coordinateY);

    Ball(int coordinateX, int coordinateY);

private:
    int coordinateX;
    int coordinateY;
    int velocityX;
    int velocityY;
public:
    int getVelocityX() const;

    void setVelocityX(int velocityX);

    int getVelocityY() const;

    void setVelocityY(int velocityY);

    void turnVelX();

    void turnVelY();

    void decrementateVelX();

    void decrementateVelY();
};


#endif //BALLSONTHREADS_BALL_H
