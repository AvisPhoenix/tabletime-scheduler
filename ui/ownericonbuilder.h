#ifndef OWNERICONBUILDER_H
#define OWNERICONBUILDER_H

#include <QIcon>
#include "../timetable/models/owner.h"
class OwnerIconBuilder
{
public:
    OwnerIconBuilder(const Owner *, int size=32, QColor fontColor= Qt::white);
    QIcon getIcon();
private:
    QIcon buildImageIcon();
    QIcon buildFontIcon();
    const Owner *owner;
    const int size;
    const QColor fontColor;
};

#endif // OWNERICONBUILDER_H
