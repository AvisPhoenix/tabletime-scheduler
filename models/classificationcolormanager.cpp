#include "classificationcolormanager.h"

ClassificationColorManager::ClassificationColorManager(ActivityPoolManager *activityPoolMng):
    activityPool(activityPoolMng),
    colors(nullptr),
    colorRamp(nullptr),
    colorGen(nullptr){
}

void ClassificationColorManager::setGradientRule(QGradient *colorRule) {
    colors = colorRule;
    reload();
}

void ClassificationColorManager::setColorGenerator(ColorGenerator *colorGen) {
    this->colorGen = colorGen;
}

void ClassificationColorManager::addNewClassification(QString classification) {
    QColor color = genNewColor();
    classificationColors[classification] = color;
}

QColor ClassificationColorManager::getColor(QString classification) {
    return classificationColors.contains(classification)? classificationColors[classification] : genNewColor();
}

void ClassificationColorManager::clear() {
    classificationColors.clear();
    classificationColors["None"] = colorRamp->toImage().pixelColor(0,0);
}

void ClassificationColorManager::reload() {
    QList<QString> classifications = activityPool->getClassifications();
    defineClassificationColor((classifications.length() >0)? QStringList(classifications) : QStringList({ "None" }));
}

void ClassificationColorManager::defineClassificationColor(QStringList classifications) {
    classificationColors.clear();
    if (classifications.size() > 0) {
        buildColorramp(classifications.size());
        int idx=0;
        for (const QString &classification: classifications){
            classificationColors[classification] = colorRamp->toImage().pixelColor(0,idx);
            idx++;
        }
    }
}

void ClassificationColorManager::buildColorramp(int size) {
    if (colorRamp != nullptr){
        delete colorRamp;
    }
    colorRamp = new QPixmap(1,size);
    QPainter painter(colorRamp);
    painter.fillRect(QRect(0,0,colorRamp->width(),colorRamp->height()), *colors);
}

QColor ClassificationColorManager::genNewColor() {
    return colorGen != nullptr? colorGen->gen() : Qt::white;
}
