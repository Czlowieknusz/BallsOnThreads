//
// Created by janek on 19.04.19.
//

#ifndef BALLSONTHREADS_LINE_H
#define BALLSONTHREADS_LINE_H

#include <vector>

struct Point {
    Point(int coordX, int coordY);

    int coordX_;
    int coordY_;
};

class Line {
public:
    Line(int x, int y);

    void move();

    const std::vector<Point> &getPoints() const;

    void changeDirectionIfNecessary(int max_y);

private:
    std::vector<Point> points;
    bool moveDirection;
};


#endif //BALLSONTHREADS_LINE_H
