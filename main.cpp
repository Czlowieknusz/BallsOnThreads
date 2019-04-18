#include <iostream>
#include "Ball.h"
#include <thread>
#include <ncurses.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include "DirectionGenerator.h"
#include <cmath>
#include <menu.h>
#include <stdio.h>
#include "Line.h"
#include <memory>

/*
 * Aplikacja tworząca co 5 sekund obiektu Ball. Każdy ma ustalony początkowy wektor ruchu
 * Może być ustawiony pod kątem 45, 90, 135 stopni. Jego prędkość ma stopniowo maleć.
 * Wykorzystana ma być biblioteka ncurses w celu wizualizacji.
 *
 */

/*
 * TODO: Grawitacja i kursor koło kólki ma zniknąć
 */

bool isEndOfProgram = false;
int maxX, maxY, initX, initY;
std::vector<Ball> balls;
std::mutex ncurses_mutex;
std::unique_ptr<Line> line;

void checkIfHitEdge(Ball &ball) {
    std::lock_guard<std::mutex> lock_guard(ncurses_mutex);
    if (ball.getCoordinateX() + ball.getVelocityX() <= 0 or ball.getCoordinateX() + ball.getVelocityX() >= maxX) {
        ball.turnVelX();
    }

    if (ball.getCoordinateY() + ball.getVelocityY() <= 0 or ball.getCoordinateY() + ball.getVelocityY() >= maxY) {
        ball.turnVelY();
    }
}

void animateBalls() {
    while (!isEndOfProgram) {
        usleep(50000);
        std::lock_guard<std::mutex> lock_guard(ncurses_mutex);
        erase();
        for (auto &ball : balls) {
            if (ball.getVelocityX() != 0 or ball.getVelocityY() != 0) {
                mvprintw(ball.getCoordinateX(), ball.getCoordinateY(), "O");
            }
        }
        for (auto &point : line->getPoints()) {
            mvprintw(point.coordX_, point.coordY_, "/");
        }
        refresh();
    }
}

// TODO: NOWE ZADANIE: po ekranie wedruje rownia pochyla. pilka uderza w nia i zsuwa sie z hardcoded predkoscia zawsze w dol. Jesli jedna uderzy to czeka az wszystkie przed nia skoncza

// TODO: poczekac na koniec watkow, zabic je
void checkIfEnd() {
    while (true) {
        if (getch()) {
            isEndOfProgram = true;
            endwin();
            exit(1);
        }
    }
}

void simulateGravity(int index) {
    std::lock_guard<std::mutex> lock_guard(ncurses_mutex);
    balls[index].decreaseVelX(0.6);
}

void animationLoop(unsigned index) {
    int zmiennaDoZmiany = 2;
    while (!isEndOfProgram) {
        for (unsigned i = 0; i < 3; ++i) {
            checkIfHitEdge(balls[index]);
            if (zmiennaDoZmiany == 0) {
                zmiennaDoZmiany = std::abs(balls[index].getVelocityX());
                std::lock_guard<std::mutex> lock_guard(ncurses_mutex);
                balls[index].move();
            } else {
                std::lock_guard<std::mutex> lock_guard(ncurses_mutex);
                balls[index].moveX();
                --zmiennaDoZmiany;
            }
            //animateBalls();
            usleep(50000 / std::abs(balls[index].getVelocityX()));
        }
        simulateGravity(index);
    }
}

void generateBall(DirectionGenerator &directionGenerator) {
    Ball buf(initX, initY, directionGenerator.getRandom());
    std::lock_guard<std::mutex> lock_guard(ncurses_mutex);
    balls.push_back(buf);
}

void calculateXYVals() {
    getmaxyx(stdscr, maxX, maxY);
    initX = maxX - 1;
    initY = maxY / 2;
}

void moveLine() {
    while (!isEndOfProgram) {
        usleep(50000);
        line->changeDirectionIfNecessary(maxX, maxY);
        line->move();
    }
}

int main() {
    initscr();
    curs_set(0);
    calculateXYVals();
    std::thread worldEnder(checkIfEnd);
    DirectionGenerator directionGenerator;
    std::vector<std::thread> threadBalls;
    std::thread animator(animateBalls);
    line = std::make_unique<Line>(initX, initY);
    std::thread lineThread(moveLine);
    while (!isEndOfProgram) {
        usleep(1500000);
        generateBall(directionGenerator);
        std::thread threadBall(animationLoop, balls.size() - 1);
        threadBalls.push_back(std::move(threadBall));
        //
    }
    for (auto &thread : threadBalls) {
        thread.join();
    }
}