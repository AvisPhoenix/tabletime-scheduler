#include "ownericonbuilder.h"
#include <QPixmap>
#include <QImage>
#include <QPainter>
#include <QFontDatabase>
#include <QDebug>


OwnerIconBuilder::OwnerIconBuilder(const Owner *owner, int size, QColor fontColor) : owner(owner), size(size), fontColor(fontColor)
{
    if (QFontDatabase::addApplicationFont(":/fonts/icons/FontAwesome6Free-Regular-400.otf") < 0)
        qWarning() << "FontAwesome cannot be loaded !";
    if (QFontDatabase::addApplicationFont(":/fonts/icons/FontAwesome6Free-Solid-900.otf") < 0)
        qWarning() << "FontAwesome cannot be loaded !";
}

QIcon OwnerIconBuilder::getIcon() {
    if (owner->getImage().isNull()){
        return buildFontIcon();
    } else {
        return buildImageIcon();
    }
}

QIcon OwnerIconBuilder::buildImageIcon() {
    QSize imageSize = QSize(size, size);
    QPixmap pixmap(imageSize);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    QPoint centerPoint = (pixmap.rect().center() - QPoint(imageSize.width() / 2, imageSize.height() / 2));
    painter.drawImage(centerPoint, owner->getImage().scaled(imageSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    painter.setBrush(owner->getColor());
    painter.drawRect(0,0,3,size);

    painter.end();
    return QIcon(pixmap);

}

QIcon OwnerIconBuilder::buildFontIcon() {
    QPixmap pixmap(QSize(size,size));
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    QFont font = painter.font();
    font.setFamily("FontAwesome");
    font.setBold(true);
    font.setPixelSize(size);
    painter.setFont(font);
    painter.setPen(fontColor);
    painter.drawText(pixmap.rect(), Qt::AlignCenter, "\uf007");

    painter.setBrush(owner->getColor());
    painter.setPen(Qt::NoPen);
    painter.drawRect(0,0,3,size);

    painter.end();
    return QIcon(pixmap);
}
