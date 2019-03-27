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

void checkIfHitEdge(Ball &ball) {
    std::lock_guard<std::mutex> lock_guard(ncurses_mutex);
    if (ball.getCoordinateX() <= 0 or ball.getCoordinateX() >= maxX) {
        ball.turnVelX();
    }

    if (ball.getCoordinateY() <= 0 or ball.getCoordinateY() >= maxY) {
        ball.turnVelY();
    }
}

void animateBalls() {
    std::lock_guard<std::mutex> lock_guard(ncurses_mutex);
    erase();
    for (auto &ball : balls) {
        if (ball.getVelocityX() != 0 or ball.getVelocityY() != 0) {
            mvprintw(ball.getCoordinateX(), ball.getCoordinateY(), "O");
        }
    }
    refresh();
}

// TODO: NOWE ZADANIE: po ekranie wedruje rownia pochyla. pilka uderza w nia i zsuwa sie z hardcoded predkoscia zawsze w dol. Jesli jedna uderzy to czeka az wszystkie przed nia skoncza

// TODO: poczekac na koniec watkow, zabic je
void checkIfEnd() {
    while (true) {
        if (getch()) {
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
    while (balls[index].getVelocityX() != 0 or balls[index].getVelocityY() != 0) {
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
            animateBalls();
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

/*
 * TODO: Add thread ending program; Kulki maja sie generowac i nie znikac koniec na watku przycisk
 * */

int main() {
    initscr();
    curs_set(0);
    //nodelay(stdscr, TRUE);
    calculateXYVals();
    std::thread worldEnder(checkIfEnd);
    DirectionGenerator directionGenerator;
    std::vector<std::thread> threadBalls;
//    std::thread gravitation(simulateGravity);
    while (true) {
        usleep(1500000);
        //
        generateBall(directionGenerator);
        std::thread threadBall(animationLoop, balls.size() - 1);
        threadBalls.push_back(std::move(threadBall));
    }
}