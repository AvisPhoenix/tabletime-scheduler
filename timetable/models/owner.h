#ifndef OWNER_H
#define OWNER_H

#include <QString>
#include <QColor>
#include <QImage>

class Owner
{
public:
    Owner(QString name= "Unknow") {this->name = name; this->color = QColor(255,0,0);}

    QImage getImage() const { return imagePath; }
    void setImage(const QImage &newImagePath) { imagePath = newImagePath; }

    QString getName() const { return name; }
    void setName(const QString &newName){ name = newName; }

    QColor getColor() const { return color; }
    void setColor(const QColor &newColor){ color = newColor; }

    bool operator==(const Owner& b) const {
        return this->name.compare(b.name) == 0;
    }

private:
    QString name;
    QImage imagePath;
    QColor color;
};

#endif // OWNER_H
