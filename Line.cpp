//
// Created by janek on 19.04.19.
//

#include "Line.h"

Line::Line(int x, int y, std::list<std::shared_ptr<Ball>> &balls) : moveDirection(false), balls_(balls), maxX(x),
                                                                    maxY(y) {
    for (int i = 0; i < x + 1; ++i) {
        points.emplace_back(Point(x - i, y + i));
    }
}

void Line::move() {
    if (moveDirection) {
        for (auto &point : points) {
            --point.coordY_;
        }
        for (auto &ball : balls_) {
            if (ball->isItInQueue()) {
                ball->decrementCoordY();
            }
        }
    } else {
        for (auto &point : points) {
            ++point.coordY_;
        }
        for (auto &ball : balls_) {
            if (ball->isItInQueue()) {
                ball->incrementCoordY();
            }
        }
    }
    if (not queue_balls.empty()) {
        if (not checkIfBallAtBottom(queue_balls.front()->getBall())) {
            queue_balls.front()->getBall()->moveByLine(1, -1);
        } else {
            queue_balls.pop();
        }
    }
}

void Line::changeDirectionIfNecessary(int max_y) {
    if ((points.begin())->coordY_ <= 0) {
        moveDirection = false;
    } else if ((points.end() - 1)->coordY_ >= max_y) {
        moveDirection = true;
    }
}

bool Line::isMoveDirection() const {
    return moveDirection;
}

Point::Point(int coordX, int coordY) : coordX_(coordX), coordY_(coordY) {}


void Line::manageCollisions() {
    for (const auto &ball : balls_) {
        if (!ball->isItInQueue()) {
            if (checkIfHitLine(ball)) {
                queue_balls.emplace(std::make_unique<BallHolder>(ball));
            }
        }
    }
}

void Line::moveLine() {
    changeDirectionIfNecessary(maxY - 1);
    move();
    manageCollisions();
}

bool Line::checkIfHitLine(const std::shared_ptr<Ball> &ball_ptr) {
    for (auto &point: points) {
        if (ball_ptr->getCoordinateX() - 1 <= point.coordX_ and ball_ptr->getCoordinateX() + 1 >= point.coordX_
            and ball_ptr->getCoordinateY() - 1 <= point.coordY_ and ball_ptr->getCoordinateY() + 1 >= point.coordY_) {
            return true;
        }
    }
    return false;
}

const std::vector<Point> &Line::getPoints() const {
    return points;
}

bool Line::checkIfBallAtBottom(const std::shared_ptr<Ball> &ball_ptr) {
    return ball_ptr->getCoordinateX() +1 >= maxX;
}
