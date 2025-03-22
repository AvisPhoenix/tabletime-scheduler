#include "qactivity.h"
#include <QPaintEvent>
#include <QPainter>
#include <QColor>
#include <QTextOption>
#include <QStyle>
#include <QDrag>
#include <QMimeData>

QActivity::QActivity(QWidget *parent)
    : QWidget{parent}, Activity(), removeBtn("\uf2ed", 16), theme(nullptr) {
    defaultConstructor();
}

void QActivity::defineRemoveButton()
{
    removeBtn.setStyleSheet("QPushButton {color: red; font-family: 'FontAwesome';}");
    removeBtn.setParent(this);
    removeBtn.setFlat(true);
    removeBtn.setGeometry(5,5,18,18);

    connect(&removeBtn,SIGNAL(clicked(bool)),SLOT(pressDelete()));
}

void QActivity::defaultConstructor()
{
    defineRemoveButton();
    setMaximumHeight(50);
}

QActivity::QActivity(QString name, QString classification, QWidget *parent)
    : QWidget(parent), Activity(name, classification), removeBtn("\uf2ed", 16) {
    defaultConstructor();
}

void QActivity::setColor(QColor color, int alphaFill) {
    this->color = color;
    aFill = alphaFill;
}

void QActivity::paintEvent(QPaintEvent *event) {
    QPainter painter = QPainter(this);
    QColor bg = theme == nullptr?color : theme->getActivityColor(classification);
    QColor outsideColor = bg;

    bg.setAlpha(theme == nullptr?aFill:theme->alphaValue("activity"));
    painter.setBrush(bg);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QPen(outsideColor,2));
    painter.drawRoundedRect(1,1,width()-1,height()-1,10,10);

    painter.setPen(theme == nullptr?fontColor: theme->getActivityWidgetColor("font"));
    QTextOption tOptions;
    tOptions.setAlignment(Qt::AlignCenter);
    tOptions.setWrapMode(QTextOption::WordWrap);
    painter.drawText(QRectF(0,0,width(),height()),name,tOptions);
}

void QActivity::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {

        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;
        QByteArray itemData;
        QDataStream dataStream(&itemData, QIODevice::WriteOnly);

        dataStream << name + "\"|\"" + classification;

        mimeData->setData("application/x-qwidget", itemData);

        QPixmap widgetPixmap(size());
        this->render(&widgetPixmap);

        drag->setMimeData(mimeData);
        drag->setPixmap(widgetPixmap);

        drag->exec(Qt::CopyAction | Qt::MoveAction);
    }
}

void QActivity::pressDelete() {
    emit onDelete(this);
}
