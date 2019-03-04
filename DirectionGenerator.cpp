//
// Created by janek on 04.03.19.
//

#include "DirectionGenerator.h"

DirectionGenerator::DirectionGenerator() :
    m_generator(std::random_device()()),
    m_distribution(0, 3) {}

int DirectionGenerator::getRandom() {
    return m_distribution(m_generator);
}
