#ifndef RAINBOWLCOLORSGENERATOR_H
#define RAINBOWLCOLORSGENERATOR_H

#include "colorgenerator.h"
#include <random>

class RainbowlColorsGenerator : public ColorGenerator
{
public:
    RainbowlColorsGenerator();
    void setPartitionSize(int partitionSize) override;
    QColor gen() override;
private:
    std::random_device rd;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> *distribution;
    int partitionSize;
};

#endif // RAINBOWLCOLORSGENERATOR_H
