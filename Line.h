//
// Created by janek on 19.04.19.
//

#ifndef BALLSONTHREADS_LINE_H
#define BALLSONTHREADS_LINE_H

#include <list>
#include <memory>
#include <queue>
#include <vector>

#include "Ball.h"
#include "BallHolder.h"


struct Point {
    Point(int coordX, int coordY);

    int coordX_;
    int coordY_;
};

class Line {
public:
    Line(int x, int y, std::list<std::shared_ptr<Ball>> &balls);

    void move();

    const std::vector<Point> &getPoints() const;

    void changeDirectionIfNecessary(int max_y);

    bool isMoveDirection() const;

    void moveLine();

private:
    bool moveDirection;
    int maxX;
    int maxY;

    std::list<std::shared_ptr<Ball>> &balls_;
    std::queue<std::unique_ptr<BallHolder>> queue_balls;
    std::vector<Point> points;

    void manageCollisions();

    bool checkIfHitLine(const std::shared_ptr<Ball> &ball_ptr);

    bool checkIfBallAtBottom(const std::shared_ptr<Ball> &ball_ptr);
};


#endif //BALLSONTHREADS_LINE_H
