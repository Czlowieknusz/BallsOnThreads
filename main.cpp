#include <iostream>
#include "Ball.h"
#include <thread>
#include <ncurses.h>
#include <unistd.h>
#include <vector>
#include <mutex>

/*
 * Aplikacja tworząca co 5 sekund obiektu Ball. Każdy ma ustalony początkowy wektor ruchu
 * Może być ustawiony pod kątem 45, 90, 135 stopni. Jego prędkość ma stopniowo maleć.
 * Wykorzystana ma być biblioteka ncurses w celu wizualizacji.
 *
 */
int maxX, maxY;
std::vector<Ball> balls;
std::mutex ncurses_mutex;

void checkIfHitEdge(Ball &ball) {
    if (ball.getCoordinateX() + ball.getVelocityX() <= 0 or ball.getCoordinateX() + ball.getVelocityX() >= maxX) {
        ball.turnVelX();
    }

    if (ball.getCoordinateY() + ball.getVelocityY() <= 0 or ball.getCoordinateY() + ball.getVelocityY() >= maxY) {
        ball.turnVelY();
    }
}

void animateBalls() {
    std::lock_guard<std::mutex> lock_guard(ncurses_mutex);
    erase();
    for (auto &ball : balls) {
        mvprintw(ball.getCoordinateX(), ball.getCoordinateY(), "O");
        refresh();
    }
}

void animationLoop(unsigned index) {
    while (balls[index].getVelocityX() != 0 and balls[index].getVelocityY() != 0) {
        for (unsigned i = 0; i < 10; ++i) {
            checkIfHitEdge(balls[index]);
            balls[index].move(balls[index].getVelocityX(), balls[index].getVelocityY());
            animateBalls();
            usleep(100000);
        }
        balls[index].decrementateVelX();
        balls[index].decrementateVelY();
    }
}

void generateBall() {
    Ball buf(2,2);
    balls.push_back(buf);
}

int main() {
    initscr();
    getmaxyx(stdscr, maxX, maxY);
    std::vector<std::thread> threadBalls;
    while(true) {
        generateBall();
        std::thread threadBall(animationLoop, balls.size()-1);
        threadBalls.push_back(std::move(threadBall));
        threadBalls[threadBalls.size()-1].join();
        usleep(5000);
    }
/*    for (unsigned i = 0; i < 3; ++i) {
        generateBall(i);
    }

    for (unsigned i = 0; i < 3; ++i) {
        std::thread threadBall(animationLoop, i);
        threadBalls.push_back(std::move(threadBall));
    }
    for (unsigned i = 0; i < 3; ++i) {
        threadBalls[i].join();
    }*/
/*
    std::cout << "max = " << maxX << "; maxy = " << maxY << std::endl;
    getch();
    endwin();
    return 0;
*/
}