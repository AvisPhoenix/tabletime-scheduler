#include "svgiconbuilder.h"
#include <QPixmap>
#include <QPainter>
#include <QtSvg/QSvgRenderer>
#include <QDebug>


QIcon SVGIconBuilder::build(const QString& iconPath, const QColor& color,  const int& size) {
    QSize iconSize(size,size);
    QPixmap pixmap(iconSize);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);

    QSvgRenderer svgRenderer(iconPath);
    if (!svgRenderer.isValid()) {
        qWarning() << "Invalid file: " << iconPath;
        return QIcon(); // Retornar un icono vacío
    }

    painter.setPen(Qt::NoPen);
    painter.setBrush(color);

    svgRenderer.setAspectRatioMode(Qt::KeepAspectRatio);
    svgRenderer.render(&painter);

    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(pixmap.rect(), color);

    painter.end();

    return QIcon(pixmap);
}
