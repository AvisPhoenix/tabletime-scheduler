#ifndef FONTAWESOMEICONBUILDER_H
#define FONTAWESOMEICONBUILDER_H

#include <QString>
#include <QIcon>

class FontAwesomeIconBuilder
{
public:
    FontAwesomeIconBuilder(QString characterIcon, int size, QColor color);
    QIcon build();
private:
    QString iconChar;
    int size;
    QColor color;
};

#endif // FONTAWESOMEICONBUILDER_H
