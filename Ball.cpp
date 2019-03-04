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

void Ball::moveX(int val) {
    Ball::coordinateX += val;
}

void Ball::moveY(int val) {
    Ball::coordinateY += val;
}

void Ball::move(int valX, int valY) {
    moveX(valX);
    moveY(valY);
}

Ball::Ball(int coordinateX, int coordinateY) : coordinateX(coordinateX), coordinateY(coordinateY), velocityX(3),
                                               velocityY(3) {}

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

void Ball::decrementateVelX() {
    --velocityX;
}

void Ball::decrementateVelY() {
    --velocityY;
}
