#ifndef OWNERICONBUILDER_H
#define OWNERICONBUILDER_H

#include <QIcon>
#include "../timetable/models/owner.h"
class OwnerIconBuilder
{
public:
    OwnerIconBuilder(){}
    static QIcon getIcon(const Owner *, const int& size=32, const QColor& color= Qt::white);
private:
    static QIcon buildImageIcon(const Owner *, const int& size, const QColor& color);
    static QIcon buildFontIcon(const Owner *, const int& size, const QColor& color);
};

#endif // OWNERICONBUILDER_H
