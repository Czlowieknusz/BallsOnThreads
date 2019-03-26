//
// Created by janek on 02.03.19.
//

#include "Ball.h"
#include <iostream>

int Ball::getCoordinateX() const {
    return coordinateX;
}

void Ball::setCoordinateX(int coordinateX) {
    Ball::coordinateX = coordinateX;
}

//7 i 6
int Ball::getCoordinateY() const {
    return coordinateY;
}

void Ball::setCoordinateY(int coordinateY) {
    Ball::coordinateY = coordinateY;
}

void Ball::moveX() {
    if (velocityX == 0) {}
    else if (velocityX < 0) {
        --coordinateX;
    } else {
        ++coordinateX;
    }
}

void Ball::moveY() {
    coordinateY += static_cast<int>(velocityY);
}

void Ball::move() {
    moveX();
    moveY();
}

double Ball::getVelocityX() const {
    return velocityX;
}

void Ball::setVelocityX(double velocityX) {
    Ball::velocityX = velocityX;
}

double Ball::getVelocityY() const {
    return velocityY;
}

void Ball::setVelocityY(double velocityY) {
    Ball::velocityY = velocityY;
}

void Ball::turnVelX() {
    velocityX *= (-1);
}

void Ball::turnVelY() {
    velocityY *= (-1);
}

void Ball::decrementVelX() {
    if (velocityX == 0) {}
    else if (velocityX < 0) {
        ++velocityX;
    } else {
        --velocityX;
    }
}

void Ball::decrementVelY() {
    if (velocityY == 0) {}
    else if (velocityY < 0) {
        ++velocityY;
    } else {
        --velocityY;
    }
}

Ball::Ball(int coordX, int coordY, int random)
        : coordinateX(coordX), coordinateY(coordY) {
    if (random == 0) {
        velocityX = -1;
        velocityY = -1;
    } else if (random == 1) {
        velocityX = -1;
        velocityY = 0;
    } else {
        velocityX = -1;
        velocityY = 1;
    }
}

void Ball::multiplyVelY(double multiplier) {
    if (velocityY == 0) {}
    else if (velocityY < 0) {
        velocityY *= multiplier;
    } else {
        velocityY /= multiplier;
    }
}
