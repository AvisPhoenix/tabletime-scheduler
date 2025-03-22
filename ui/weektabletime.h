#ifndef WEEKTABLETIME_H
#define WEEKTABLETIME_H

#include <QGraphicsItem>
#include <QTextOption>
#include "../timetable/models/schedulegridsizemanager.h"
#include "../models/themecolors.h"

class WeekTableTime : public QGraphicsItem
{
public:
    WeekTableTime(ScheduleGridSizeManager *gridManager, ThemeColors *theme, QStringList weekdays );
    void setWeekdays(QStringList weekdays){ weekdaysTitle = weekdays;}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
private:
    ThemeColors *theme;
    ScheduleGridSizeManager *gridMng;
    QStringList weekdaysTitle;
    void drawHeaders(QPainter *painter, QTextOption tOptions);
    void drawVerticalLines(QRectF area, QPainter *painter);
    void drawHorizontalMainLines(QPainter *painter, QRectF area);
    void drawHorizontalInterlines(QPainter *painter, QRectF area);
    void drawHours(QPainter *painter, QTextOption tOptions, QRectF area);
};

#endif // WEEKTABLETIME_H
