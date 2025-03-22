#include "fontawesomeiconbuilder.h"
#include <QFontDatabase>
#include <QDebug>
#include <QPixmap>
#include <QPainter>

FontAwesomeIconBuilder::FontAwesomeIconBuilder(QString characterIcon, int size, QColor color):iconChar(characterIcon), size(size), color(color) {
    if (QFontDatabase::addApplicationFont(":/fonts/icons/FontAwesome6Free-Regular-400.otf") < 0)
        qWarning() << "FontAwesome cannot be loaded !";
    if (QFontDatabase::addApplicationFont(":/fonts/icons/FontAwesome6Free-Solid-900.otf") < 0)
        qWarning() << "FontAwesome cannot be loaded !";
}

QIcon FontAwesomeIconBuilder::build() {
    QPixmap pixmap(QSize(size,size));
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    QFont font = painter.font();
    font.setFamily("FontAwesome");
    font.setBold(true);
    font.setPixelSize(size);
    painter.setFont(font);
    painter.setPen(color);
    painter.drawText(pixmap.rect(), Qt::AlignCenter, iconChar);

    painter.end();
    return QIcon(pixmap);
}
