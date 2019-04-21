//
// Created by janek on 20.04.19.
//

#include "BallHolder.h"

BallHolder::BallHolder(const std::shared_ptr<Ball> &ball) : ball_(ball), velocityX_(ball_->getVelocityX()),
                                                            velocityY_(ball_->getVelocityY()) {
    ball_->setVelocityX(0);
    ball_->setVelocityY(0);
}

BallHolder::~BallHolder() {
    ball_->setVelocityX(velocityX_);
    ball_->setVelocityY(velocityY_);
}

const std::shared_ptr<Ball> &BallHolder::getBall() const {
    return ball_;
}
