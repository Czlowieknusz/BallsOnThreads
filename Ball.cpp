//
// Created by janek on 02.03.19.
//

#include "Ball.h"

int Ball::getCoordinateX() const {
    return coordinateX;
}

void Ball::setCoordinateX(int coordinateX) {
    Ball::coordinateX = coordinateX;
}

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
    if (velocityY == 0) {}
    else if (velocityY < 0) {
        --coordinateY;
    } else {
        ++coordinateY;
    }
}

void Ball::move() {
    moveX();
    moveY();
}

int Ball::getVelocityX() const {
    return velocityX;
}

void Ball::setVelocityX(int velocityX) {
    Ball::velocityX = velocityX;
}

int Ball::getVelocityY() const {
    return velocityY;
}

void Ball::setVelocityY(int velocityY) {
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
        velocityX = -2;
        velocityY = -2;
    } else if (random == 1) {
        velocityX = -2;
        velocityY = 0;
    } else {
        velocityX = -2;
        velocityY = 2;
    }
}