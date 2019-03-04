//
// Created by janek on 04.03.19.
//

#ifndef BALLSONTHREADS_DIRECTIONGENERATOR_H
#define BALLSONTHREADS_DIRECTIONGENERATOR_H

#include <random>

class DirectionGenerator {
    std::mt19937 m_generator;
    std::uniform_int_distribution<int> m_distribution;

public:
    DirectionGenerator();
    int getRandom();
};


#endif //BALLSONTHREADS_DIRECTIONGENERATOR_H
