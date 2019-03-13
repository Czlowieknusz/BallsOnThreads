#include <iostream>
#include "Ball.h"
#include <thread>
#include <ncurses.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include "DirectionGenerator.h"

/*
 * Aplikacja tworząca co 5 sekund obiektu Ball. Każdy ma ustalony początkowy wektor ruchu
 * Może być ustawiony pod kątem 45, 90, 135 stopni. Jego prędkość ma stopniowo maleć.
 * Wykorzystana ma być biblioteka ncurses w celu wizualizacji.
 *
 */

/*
 * TODO: Grawitacja i kursor koło kólki ma zniknąć
 */

int maxX, maxY, initX, initY;
std::vector<Ball> balls;
std::mutex ncurses_mutex;
DirectionGenerator directionGenerator;

void checkIfHitEdge(Ball &ball) {
    std::lock_guard<std::mutex> lock_guard(ncurses_mutex);
    if (ball.getCoordinateX() == 0 or ball.getCoordinateX() == maxX) {
        ball.turnVelX();
    }

    if (ball.getCoordinateY() == 0 or ball.getCoordinateY() == maxY) {
        ball.turnVelY();
    }
}

void animateBalls() {
    std::lock_guard<std::mutex> lock_guard(ncurses_mutex);
    erase();
    for (auto &ball : balls) {
        if (ball.getVelocityX() != 0 or ball.getVelocityY() != 0) {
            mvprintw(ball.getCoordinateX(), ball.getCoordinateY(), "O");
            refresh();
        }
    }
}

void animationLoop(unsigned index) {
    while (balls[index].getVelocityX() != 0 or balls[index].getVelocityY() != 0) {
        for (unsigned i = 0; i < 50; ++i) {
            checkIfHitEdge(balls[index]);
            balls[index].move();
            animateBalls();
            usleep(100000 / abs(balls[index].getVelocityX()));
        }
        balls[index].decrementVelX();
        balls[index].decrementVelY();
    }
}

void generateBall() {
    Ball buf(initX, initY, directionGenerator.getRandom());
    std::lock_guard<std::mutex> lock_guard(ncurses_mutex);
    balls.push_back(buf);
}

void calculateXYVals() {
    getmaxyx(stdscr, maxX, maxY);
    initX = maxX-1;
    initY = maxY / 2;
}

int main() {
    initscr();
    calculateXYVals();
    std::vector<std::thread> threadBalls;
    unsigned numberOfIteration = 0;
    while (numberOfIteration < 100) {
        usleep(1500000);
        //
        generateBall();
        std::thread threadBall(animationLoop, balls.size() - 1);
        threadBalls.push_back(std::move(threadBall));
        ++numberOfIteration;
    }
    getch();
    endwin();
    return 0;
}