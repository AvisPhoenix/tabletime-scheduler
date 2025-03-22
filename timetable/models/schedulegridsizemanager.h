#ifndef SCHEDULEGRIDSIZEMANAGER_H
#define SCHEDULEGRIDSIZEMANAGER_H

#include <QRectF>
#include <QLineF>
#include <QFont>
#include <QPointF>
#include "resizemode.h"

class ScheduleGridSizeManager
{
public:
    ScheduleGridSizeManager(QFont font, int width=0, int height=0, int timePartition=4, int paddingPx=15, int startHour=5, int endHour=24);
    void setSnap2Time(bool snap){ snap2Time = snap;}
    void setFont(QFont font) {textFont = font; updateTextFont();}
    QFont getTimeFont() {return textFont;}
    void setTimePartion(int parts) {timeParts = parts; updateTextFont();}
    void setSpaceSize(int width, int height);
    float getDayWidth();
    float getHourPartitionGap();
    float getHourGap();
    QRectF getColumnHeader(int dayOfWeek);
    QRectF getScheduleZone() { return QRectF(padding, padding, widthR, heightR);}
    QRectF getPositionOnSchedule(QRectF quad, ResizeMode  isMove);
    float getStartWeekZone();
    float getStartHoursZone();
    int getSecPartition();
    QTime getTimeByPosition(float posY);
    int getDayByPosition(float posX);
    QRectF getActivitySizeByTimeRange(QTime start, QTime end, int dayWeek);
    float getPositionByTime(QTime start);

private:
    bool snap2Time;
    float widthR, heightR;
    int timeParts;
    int padding;
    QFont textFont;
    int sHour, eHour, elapsedHours;

    void updateTextFont();
    float getHeaderHeight();
    float getHourWidth();
    void snap2ColumnDay(int i, float dayWidth, QRectF &output, float base);
    void foundHorizontalPosition(QRectF &output);
    void foundVerticalPosition(QRectF &output, ResizeMode mode);
    bool isOnTimePartZone(int i, float pos);
    float getTimeZoneEnd(int i);
    float getTimeZoneStart(int i);
    int findTimeZone(float edge);
    void snapTop(ResizeMode mode, QRectF &output);
    void snapBottom(ResizeMode mode, QRectF &output);
    int isOnHourPartZone(int, float);
    int isOnDayZone(int, float);
    QTime buildQTime(int sH, int m);
};

#endif // SCHEDULEGRIDSIZEMANAGER_H
