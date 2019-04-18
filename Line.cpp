//
// Created by janek on 19.04.19.
//

#include "Line.h"

Line::Line(int x, int y) : moveDirection(false) {
    for (int i = 0; i < x; ++i) {
        points.emplace_back(Point(x + i, y + i));
    }
}

void Line::move() {

}

Point::Point(int coordX, int coordY) : coordX_(coordX), coordY_(coordY) {}
