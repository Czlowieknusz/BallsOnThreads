#include <cmath>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include <menu.h>
#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#include "Ball.h"
#include "BallHolder.h"
#include "DirectionGenerator.h"
#include "Line.h"

/*
 * Aplikacja tworząca co 5 sekund obiektu Ball. Każdy ma ustalony początkowy wektor ruchu
 * Może być ustawiony pod kątem 45, 90, 135 stopni. Jego prędkość ma stopniowo maleć.
 * Wykorzystana ma być biblioteka ncurses w celu wizualizacji.
 *
 * Etap 2:
 *
 * Po ekranie wędruje linia.
 * Gdy jakaś piłeczka w nią uderzy kolejkuje się wewnątrz lini i porusza wraz z nią.
 *
 */

bool isEndOfProgram = false;
int maxX, maxY, initX, initY;
std::list<std::shared_ptr<Ball>> balls;
std::mutex ncurses_mutex;
std::unique_ptr<Line> line;

bool reachedTop(const std::shared_ptr<Ball> &ball_ptr) {
    return ball_ptr->getCoordinateX() + 1 <= 0
           and ball_ptr->getVelocityX() < 0;
}

bool reachedBottom(const std::shared_ptr<Ball> &ball_ptr) {
    return ball_ptr->getCoordinateX() - 1 >= maxX
           and ball_ptr->getVelocityX() > 0;
}

bool reachedLeft(const std::shared_ptr<Ball> &ball_ptr) {
    return ball_ptr->getCoordinateY() + ball_ptr->getVelocityY() <= 0;
}

bool reachedRight(const std::shared_ptr<Ball> &ball_ptr) {
    return ball_ptr->getCoordinateY() + ball_ptr->getVelocityY() >= maxY;
}

void checkIfHitEdge(const std::shared_ptr<Ball> &ball_ptr) {
    std::lock_guard<std::mutex> lock_guard(ncurses_mutex);
    if (reachedBottom(ball_ptr) or reachedTop(ball_ptr)) {
        ball_ptr->turnVelX();
    }

    if (reachedLeft(ball_ptr) or reachedRight(ball_ptr)) {
        ball_ptr->turnVelY();
    }
}

void animateBalls() {
    while (!isEndOfProgram) {
        usleep(50000);
        std::lock_guard<std::mutex> lock_guard(ncurses_mutex);
        erase();
        for (const auto &point : line->getPoints()) {
            mvprintw(point.coordX_, point.coordY_, "/");
        }
        for (const auto &ball_ptr : balls) {
            mvprintw(ball_ptr->getCoordinateX(), ball_ptr->getCoordinateY(), "O");
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

void simulateGravity(std::shared_ptr<Ball> &ball) {
    std::lock_guard<std::mutex> lock_guard(ncurses_mutex);
    ball->decreaseVelX(0.6);
}

// Change it to store ptr not index HURRAY
// std::list ftw!
void animationLoop(std::shared_ptr<Ball> &ball) {
    int numberOfMoveXBeforeY = 2;
    while (!isEndOfProgram) {
        for (unsigned i = 0; i < 3; ++i) {
            if (not ball->isItInQueue()) {
                checkIfHitEdge(ball);
                if (numberOfMoveXBeforeY == 0) {
                    numberOfMoveXBeforeY = std::abs(ball->getVelocityX());
                    std::lock_guard<std::mutex> lock_guard(ncurses_mutex);
                    ball->move();
                } else {
                    std::lock_guard<std::mutex> lock_guard(ncurses_mutex);
                    ball->moveX();
                    --numberOfMoveXBeforeY;
                }
            }
            usleep(50000 / std::abs(ball->getVelocityX()));
        }
        if (ball->getCoordinateX() <= maxX) {
            simulateGravity(ball);
        }
    }
}

void generateBall(DirectionGenerator &directionGenerator) {
    auto buf(std::make_shared<Ball>(initX, initY, directionGenerator.getRandom()));
    std::lock_guard<std::mutex> lock_guard(ncurses_mutex);
    balls.push_back(std::move(buf));
}

void calculateXYVals() {
    getmaxyx(stdscr, maxX, maxY);
    initX = maxX - 1;
    initY = maxY / 2;
}

// probably mutex will be needed
void lineThreadFunction() {
    while (!isEndOfProgram) {
        usleep(50000);
        line->moveLine();
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

    line = std::make_unique<Line>(maxX, maxY, balls);

//    std::queue<std::shared_ptr<BallHolder>> queue_balls;
    //std::thread collisionManager(manageCollisions, std::ref(queue_balls));

    std::thread lineThread(lineThreadFunction);

    while (!isEndOfProgram) {
        usleep(1500000);
        generateBall(directionGenerator);
        std::thread threadBall(animationLoop, std::ref(*balls.rbegin()));
        threadBalls.push_back(std::move(threadBall));
    }
    for (auto &thread : threadBalls) {
        thread.join();
    }
    worldEnder.join();
    animator.join();
    lineThread.join();
//    collisionManager.join();
}