#ifndef COLORGENERATOR_H
#define COLORGENERATOR_H

#include <QColor>

class ColorGenerator {
public:
    ColorGenerator(){ }
    virtual void setPartitionSize(int partitionSize)=0;
    virtual QColor gen()=0;
};

#endif // COLORGENERATOR_H
