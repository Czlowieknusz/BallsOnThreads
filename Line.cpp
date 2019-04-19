//
// Created by janek on 19.04.19.
//

#include "Line.h"

Line::Line(int x, int y) : moveDirection(false) {
    for (int i = 0; i < x + 1; ++i) {
        points.emplace_back(Point(x - i, y + i));
    }
}

void Line::move() {
    if (moveDirection) {
        for (auto &point : points) {
            --point.coordY_;
        }
    } else {
        for (auto &point : points) {
            ++point.coordY_;
        }
    }
}

const std::vector<Point> &Line::getPoints() const {
    return points;
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
