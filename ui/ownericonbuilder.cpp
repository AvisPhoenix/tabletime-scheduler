#include "ownericonbuilder.h"
#include <QPixmap>
#include <QImage>
#include <QPainter>
#include <QtResource>
#include "svgiconbuilder.h"


QIcon OwnerIconBuilder::getIcon(const Owner *owner, const int& size, const QColor& color) {
    return owner->getImage().isNull()?
               buildFontIcon(owner, size, color)
                                      :
               buildImageIcon(owner, size, color);
}

QIcon OwnerIconBuilder::buildImageIcon(const Owner *owner, const int& size, const QColor& color) {
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

QIcon OwnerIconBuilder::buildFontIcon(const Owner *owner, const int& size, const QColor& color) {
    return SVGIconBuilder::build(":/icons/icons/user.svg", color, size);
}
