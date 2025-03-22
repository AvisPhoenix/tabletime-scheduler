#ifndef CLASSIFICATIONCOLORMANAGER_H
#define CLASSIFICATIONCOLORMANAGER_H

#include <QGradient>
#include <QPixmap>
#include <QPainter>
#include <QMap>
#include <QStringList>

#include "../timetable/models/activitypoolmanager.h"
#include "colorGenerators/colorgenerator.h"

class ClassificationColorManager
{
public:
    ClassificationColorManager(ActivityPoolManager *activityPoolMng);
    void setGradientRule(QGradient *colorRule);
    void setColorGenerator(ColorGenerator *colorGen);
    void addNewClassification(QString classification);
    QColor getColor(QString classification);
    void clear();
    void reload();
private:
    ActivityPoolManager *activityPool;
    QMap<QString, QColor> classificationColors;
    QGradient *colors;
    QPixmap *colorRamp;
    ColorGenerator *colorGen;
    void defineClassificationColor(QStringList classification);
    void buildClassificationData();
    void buildColorramp(int size);
    QColor genNewColor();
};

#endif // CLASSIFICATIONCOLORMANAGER_H
