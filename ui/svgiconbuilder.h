#ifndef SVGICONBUILDER_H
#define SVGICONBUILDER_H

#include <QString>
#include <QIcon>
#include <QColor>

class SVGIconBuilder
{
public:
    SVGIconBuilder() {}
    static QIcon build(const QString& iconPath, const QColor& color,  const int& size);
};

#endif // SVGICONBUILDER_H
