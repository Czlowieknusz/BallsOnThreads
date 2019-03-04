#include <iostream>
#include "Ball.h"
#include <thread>
#include <ncurses.h>
#include <unistd.h>
#include <vector>

/*
 * Aplikacja tworząca co 5 sekund obiektu Ball. Każdy ma ustalony początkowy wektor ruchu
 * Może być ustawiony pod kątem 45, 90, 135 stopni. Jego prędkość ma stopniowo maleć.
 * Wykorzystana ma być biblioteka ncurses w celu wizualizacji.
 *
 */
int maxX, maxY;
std::vector<Ball> balls;

void checkIfHitEdge(Ball &ball) {
    if (ball.getCoordinateX() + ball.getVelocityX() <= 0 or ball.getCoordinateX() + ball.getVelocityX() >= maxX) {
        ball.turnVelX();
    }

    if (ball.getCoordinateY() + ball.getVelocityY() <= 0 or ball.getCoordinateY() + ball.getVelocityY() >= maxY) {
        ball.turnVelY();
    }
}

void animationLoop(unsigned index) {
//    Ball ball(5, 5);
    while (balls[index].getVelocityX() != 0 and balls[index].getVelocityY() != 0 ) {
        for (unsigned i = 0; i < 30; ++i) {
            checkIfHitEdge(balls[index]);
            balls[index].move(balls[index].getVelocityX(), balls[index].getVelocityY());
            move(balls[index].getCoordinateX(), balls[index].getCoordinateY());
            printw("%d, %d", balls[index].getCoordinateX(), balls[index].getCoordinateY());
            refresh();
            usleep(100000);
            erase();
        }
        balls[index].decrementateVelX();
        balls[index].decrementateVelY();
    }
}

void animateBalls() {
    erase();
    for (auto &ball : balls) {
        move(ball.getCoordinateX(), ball.getCoordinateY());
        printw("%d, %d", ball.getCoordinateX(), ball.getCoordinateY());

    }
}

void generateBall() {
    Ball buf(5, 5);
    balls.push_back(buf);
}

int main() {
    initscr();
    getmaxyx(stdscr, maxX, maxY);
    for (unsigned i = 0; i < 1; ++i) {
        generateBall();
    }
    for (unsigned i = 0; i < 1; ++i) {
        std::thread threadBall(animationLoop, i);
        threadBall.join();
    }
    std::cout << "max = " << maxX << "; maxy = " << maxY << std::endl;
    refresh();
    getch();
    endwin();
    return 0;
}