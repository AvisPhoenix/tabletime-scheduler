#include "rainbowlcolorsgenerator.h"

RainbowlColorsGenerator::RainbowlColorsGenerator(): generator(rd()) {
    RainbowlColorsGenerator::setPartitionSize(200);
}

void RainbowlColorsGenerator::setPartitionSize(int partitionSize) {
    distribution = new std::uniform_int_distribution<int>(0,partitionSize);
    partitionSize = partitionSize;
}

QColor RainbowlColorsGenerator::gen() {
    QColor color;
    float h = ((float)(*distribution)(generator))/((float)partitionSize);
    color.setHslF(h,1.0,0.5);
    return color;
}
