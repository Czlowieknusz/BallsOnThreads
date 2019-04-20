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

void checkIfHitEdge(const std::shared_ptr<Ball> &ball_ptr) {
    std::lock_guard<std::mutex> lock_guard(ncurses_mutex);
    if (ball_ptr->getCoordinateX() + ball_ptr->getVelocityX() <= 0 or
        ball_ptr->getCoordinateX() + ball_ptr->getVelocityX() >= maxX) {
        ball_ptr->turnVelX();
    }

    if (ball_ptr->getCoordinateY() + ball_ptr->getVelocityY() <= 0 or
        ball_ptr->getCoordinateY() + ball_ptr->getVelocityY() >= maxY) {
        ball_ptr->turnVelY();
    }
}

void animateBalls() {
    while (!isEndOfProgram) {
        usleep(50000);
        std::lock_guard<std::mutex> lock_guard(ncurses_mutex);
        erase();
        for (const auto &ball_ptr : balls) {
            mvprintw(ball_ptr->getCoordinateX(), ball_ptr->getCoordinateY(), "O");
        }
        for (const auto &point : line->getPoints()) {
            mvprintw(point.coordX_, point.coordY_, "/");
        }/*
        mvprintw(0, 0, "1");
        mvprintw(0, 1, "2");
        mvprintw(1, 0, "3");
        mvprintw(1, 1, "4");*/
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
            usleep(50000 / std::abs(ball->getVelocityX()));
        }
        simulateGravity(ball);
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

void moveLine() {
    while (!isEndOfProgram) {
        usleep(50000);
        line->changeDirectionIfNecessary(maxY - 1);
        line->move();
    }
}

// mutex in manageCollisions
bool checkIfHitLine(const std::shared_ptr<Ball> &ball_ptr) {
    for (auto &point: line->getPoints()) {
        if (ball_ptr->getCoordinateX() - 1 <= point.coordX_ and ball_ptr->getCoordinateX() + 1 >= point.coordX_
            and ball_ptr->getCoordinateY() - 1 <= point.coordY_ and ball_ptr->getCoordinateY() + 1 >= point.coordY_) {
            return true;
        }
    }
    return false;
}

void manageCollisions(std::queue<std::shared_ptr<BallHolder>> &queue_balls) {
    while (!isEndOfProgram) {
        usleep(5000);
        std::lock_guard<std::mutex> lock_guard(ncurses_mutex);
        for (const auto &ball: balls) {
            if (!ball->isItInQueue()) {
                if (checkIfHitLine(ball)) {
                    /*               ball->setCoordinateY(0);
                                   ball->setCoordinateX(0);
               */
                    ball->setIsInQueue(true);
                    queue_balls.emplace(std::make_shared<BallHolder>(ball));
                }
            }
        }
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

    std::queue<std::shared_ptr<BallHolder>> queue_balls;
    std::thread collisionManager(manageCollisions, std::ref(queue_balls));

    std::thread lineThread(moveLine);

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
    collisionManager.join();
}