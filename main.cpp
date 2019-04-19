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
#include <queue>

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
std::vector<std::shared_ptr<Ball>> balls;
std::mutex ncurses_mutex;
std::unique_ptr<Line> line;
std::queue<std::shared_ptr<Ball>> queue_balls;

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
        for (auto ball_ptr : balls) {
            if (ball_ptr->getVelocityX() != 0 or ball_ptr->getVelocityY() != 0) {
                mvprintw(ball_ptr->getCoordinateX(), ball_ptr->getCoordinateY(), "O");
            }
        }
        for (auto &point : line->getPoints()) {
            mvprintw(point.coordX_, point.coordY_, "/");
        }
        mvprintw(0, 0, "1");
        mvprintw(0, 1, "2");
        mvprintw(1, 0, "3");
        mvprintw(1, 1, "4");
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
    balls[index]->decreaseVelX(0.6);
}

void animationLoop(unsigned index) {
    int numberOfMoveXBeforeY = 2;
    while (!isEndOfProgram) {
        for (unsigned i = 0; i < 3; ++i) {
            checkIfHitEdge(balls[index]);
            if (numberOfMoveXBeforeY == 0) {
                numberOfMoveXBeforeY = std::abs(balls[index]->getVelocityX());
                std::lock_guard<std::mutex> lock_guard(ncurses_mutex);
                balls[index]->move();
            } else {
                std::lock_guard<std::mutex> lock_guard(ncurses_mutex);
                balls[index]->moveX();
                --numberOfMoveXBeforeY;
            }
            usleep(50000 / std::abs(balls[index]->getVelocityX()));
        }
        simulateGravity(index);
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

void manageCollisions() {
    while (!isEndOfProgram) {
        usleep(5000);
        std::lock_guard<std::mutex> lock_guard(ncurses_mutex);
        for (const auto &ball: balls) {
            if (!ball->isItInQueue()) {
                if (checkIfHitLine(ball)) {
                    ball->setCoordinateY(0);
                    ball->setCoordinateX(0);

                    ball->setIsInQueue(true);
                    queue_balls.push(ball);
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

    std::thread lineThread(moveLine);
    std::thread collisionManager(manageCollisions);

    while (!isEndOfProgram) {
        usleep(1500000);
        generateBall(directionGenerator);
        std::thread threadBall(animationLoop, balls.size() - 1);
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