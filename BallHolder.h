//
// Created by janek on 20.04.19.
//

#ifndef BALLSONTHREADS_BALLHOLDER_H
#define BALLSONTHREADS_BALLHOLDER_H

#include <memory>
#include "Ball.h"

class BallHolder {
public:
    explicit BallHolder(const std::shared_ptr<Ball> &ball);

private:
    std::shared_ptr<Ball> ball_;
    int velocityX_;
    int velocityY_;
};


#endif //BALLSONTHREADS_BALLHOLDER_H
