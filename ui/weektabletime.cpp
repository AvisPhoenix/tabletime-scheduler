#include "weektabletime.h"
#include <QPainter>
#include <QBrush>
#include <QColor>
#include <QLineF>
#include <QRectF>
#include <QPointF>
#include <QPalette>
#include <QTime>
#include <QFont>

WeekTableTime::WeekTableTime(ScheduleGridSizeManager *gridManager,
                             ThemeColors *theme,
                             QStringList weekdays):
    gridMng(gridManager),
    theme(theme),
    weekdaysTitle(weekdays){
}

void WeekTableTime::drawHeaders(QPainter *painter, QTextOption tOptions)
{
    QColor headers = theme->getGridColor("headers");
    painter->setBrush(QColor(headers.red(), headers.green(), headers.blue(), theme->alphaValue("grid")));
    for(int i=0; i < 7; i++){
        painter->setPen(Qt::transparent);
        painter->drawRect(gridMng->getColumnHeader(i));
        painter->setPen(theme->getGridColor("headerText"));
        painter->drawText(gridMng->getColumnHeader(i),weekdaysTitle.at(i), tOptions);
    }
}

void WeekTableTime::drawVerticalLines(QRectF area, QPainter *painter)
{
    painter->setBrush(Qt::transparent);
    painter->setPen(theme->getGridColor("line"));
    QLineF baseLine(area.topLeft(), area.bottomLeft());
    QRectF header = gridMng->getColumnHeader(0);
    for(int i =0; i < 7; i++){
        header =  gridMng->getColumnHeader(i);
        painter->drawLine(baseLine.translated(header.topLeft()-baseLine.p1()));
    }
    painter->drawLine(baseLine.translated(header.topRight()-baseLine.p1()));
}

void WeekTableTime::drawHorizontalMainLines(QPainter *painter, QRectF area)
{
    painter->setBrush(Qt::transparent);
    painter->setPen(theme->getGridColor("line"));

    painter->drawLine(QLineF(QPointF(gridMng->getStartWeekZone(), area.top()), area.topRight()));

    QLineF baseLine(area.topLeft(), area.topRight());
    baseLine.translate(QPointF(0, gridMng->getHeaderHeight()));
    for(int i =0; i < 21; i++){
        painter->drawLine(baseLine);
        baseLine.translate(QPointF(0, gridMng->getHourGap()));
    }
}

void WeekTableTime::drawHorizontalInterlines(QPainter *painter, QRectF area)
{
    if (gridMng->getHourPartitionGap() > 0){
        painter->setBrush(Qt::transparent);
        painter->setPen(theme->getGridColor("interline"));
        QLineF baseLine(area.topLeft(), area.topRight());
        baseLine.translate(QPointF(0, gridMng->getStartHoursZone()-area.top() + gridMng->getHourPartitionGap()));
        while (baseLine.p2().y() < area.bottom()){
            painter->drawLine(baseLine);
            baseLine.translate(QPointF(0, gridMng->getHourPartitionGap()));
        }
    }
}

void WeekTableTime::drawHours(QPainter *painter, QTextOption tOptions, QRectF area)
{
    painter->setFont(gridMng->getTimeFont());
    painter->setPen(theme->getGridColor("hourText"));
    if (gridMng->getHourPartitionGap() > 0){
        QRectF timeArea(QPointF(area.left(), gridMng->getStartHoursZone()), QSizeF(gridMng->getStartWeekZone()-area.left(), gridMng->getHourPartitionGap()));
        QTime time(5,0);
        while (timeArea.bottom() - area.bottom() <= 0.5){
            painter->drawText(timeArea, time.toString("HH:mm"),tOptions);
            timeArea.moveTop(timeArea.top()+gridMng->getHourPartitionGap());
            time = time.addSecs(gridMng->getSecPartition());
        }
    }else {
        QRectF timeArea(QPointF(area.left(), gridMng->getStartHoursZone()), QSizeF(gridMng->getStartWeekZone()-area.left(), gridMng->getHourGap()));
        QTime time(5,0);
        while (timeArea.bottom() < area.bottom()){
            painter->drawText(timeArea, time.toString("HH:mm"),tOptions);
            timeArea.moveTop(timeArea.top()+gridMng->getHourGap());
            time = time.addSecs(3600);
        }
    }

}

void WeekTableTime::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    if (gridMng != nullptr){
        painter->setRenderHint(QPainter::Antialiasing);
        QTextOption tOptions;
        tOptions.setAlignment(Qt::AlignCenter);
        tOptions.setWrapMode(QTextOption::WordWrap);

        QRectF area = gridMng->getScheduleZone();

        drawHeaders(painter, tOptions);
        drawHorizontalInterlines(painter, area);
        drawVerticalLines(area, painter);
        drawHorizontalMainLines(painter, area);
        drawHours(painter, tOptions, area);

    }
}

QRectF WeekTableTime::boundingRect() const {
    return gridMng == nullptr ? QRectF(0,0,1,1) : gridMng->getScheduleZone();
}
