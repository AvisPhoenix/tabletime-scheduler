#include "schedulegridsizemanager.h"
#include <QFontMetricsF>
#include <QTime>
#include <QRectF>
#include <QPointF>
#include <QSizeF>
#include <QDebug>

ScheduleGridSizeManager::ScheduleGridSizeManager(QFont font,
                                                 int width,
                                                 int height,
                                                 int timePartition,
                                                 int paddingPx,
                                                 int startHour,
                                                 int endHour):
    timeParts(timePartition), padding(paddingPx), textFont(font), snap2Time(true), sHour(startHour), eHour(endHour), elapsedHours(endHour-startHour){
    setSpaceSize(width, height);
}

void ScheduleGridSizeManager::updateTextFont()
{
    textFont.setPixelSize(getHourPartitionGap() - 2);
}

float ScheduleGridSizeManager::getHeaderHeight() {
    QFontMetrics metric((QFont()));
    return 2*padding + metric.height();
}

void ScheduleGridSizeManager::setSpaceSize(int width, int height) {
    widthR = width - 2*padding;
    heightR = height - 2*padding;
    widthR = widthR < 0 ? 0: widthR;
    heightR = heightR < 0? 0: heightR;
    updateTextFont();
}

float ScheduleGridSizeManager::getDayWidth() {
    float hourWidth = getHourWidth();
    return (widthR-hourWidth)/7.0;
}

float ScheduleGridSizeManager::getHourPartitionGap() {
    return (heightR - getHeaderHeight())/((float)elapsedHours*timeParts);
}

float ScheduleGridSizeManager::getHourGap() {
    return (heightR - getHeaderHeight())/(float)elapsedHours;
}

QRectF ScheduleGridSizeManager::getColumnHeader(int dayOfWeek) {
    if (dayOfWeek < 0){
        return QRectF(padding, padding, getHourWidth(), getHeaderHeight());
    }
    float base = getStartWeekZone();
    return QRectF(base+dayOfWeek*getDayWidth(), padding, getDayWidth(), getHeaderHeight());
}

void ScheduleGridSizeManager::snap2ColumnDay(int i, float dayWidth, QRectF &output, float base)
{
    float left = (base + i*dayWidth) - dayWidth/3.0;
    float right = (base + (i+1)*dayWidth) - dayWidth/3.0;
    if (left < output.left() && output.left() <= right ){
        output.moveLeft(base + i*dayWidth);
    }
}

void ScheduleGridSizeManager::foundHorizontalPosition(QRectF &output)
{
    float base = getStartWeekZone();
    float dayWidth = getDayWidth();

    output.setWidth(dayWidth);

    if (output.left() < base){
        output.moveLeft(base);
    } else if (padding + widthR - getDayWidth()  < output.left()){
        output.moveLeft(padding + widthR - getDayWidth());
    } else {
        for(int i =0; i < 7; i++){
            snap2ColumnDay(i, dayWidth, output, base);
        }
    }
}

int ScheduleGridSizeManager::findTimeZone(float edge)
{
    int i=10*timeParts;
    int ini = 0;
    int end = elapsedHours*timeParts;
    while (ini <= i  && i < end && !isOnTimePartZone(i, edge)){
        if (edge < getTimeZoneStart(i)){
            end = i-1;
        } else {
            ini = i+1;
        }
        i = (end + ini)/2;
    }

    return i;
}

void ScheduleGridSizeManager::snapTop(ResizeMode mode, QRectF &output) {
    int i = findTimeZone(output.top());
    if (0 <= i  && i < 20*timeParts){
        if (mode == ResizeMode::Move)
            output.moveTop(getTimeZoneStart(i));
        else if (mode == ResizeMode::Top)
            output.setTop(getTimeZoneStart(i));
    }else if (i < 0){
        if (mode == ResizeMode::Move)
            output.moveTop(getTimeZoneStart(0));
        else if (mode == ResizeMode::Top)
            output.setTop(getTimeZoneStart(0));
    } else{
        if (mode == ResizeMode::Move)
            output.moveTop(getTimeZoneStart(elapsedHours*timeParts));
        else if (mode == ResizeMode::Top)
            output.setTop(getTimeZoneStart(elapsedHours*timeParts));
    }
}

void ScheduleGridSizeManager::snapBottom(ResizeMode mode, QRectF &output)
{
    if (mode == ResizeMode::Bottom) {
        int i = findTimeZone(output.bottom());
        if (0 <= i  && i < elapsedHours*timeParts){
            output.setBottom(getTimeZoneEnd(i));
        }else if (i < 0){
            output.setBottom(getTimeZoneEnd(0));
        } else{
            output.setBottom(getTimeZoneEnd(elapsedHours*timeParts));
        }
    }

}

int ScheduleGridSizeManager::isOnHourPartZone(int i, float pos) {
    return getStartHoursZone() + i*getHourGap() < pos && pos <= getStartHoursZone() + (i+1)*getHourGap();
}

int ScheduleGridSizeManager::isOnDayZone(int i, float pos) {
    return getStartWeekZone() + i*getDayWidth() < pos && pos <= getStartWeekZone() + (i+1)*getDayWidth();
}


void ScheduleGridSizeManager::foundVerticalPosition(QRectF &output, ResizeMode mode)
{
    float base = getStartHoursZone();

    if (output.top() < base) {
        if (mode == ResizeMode::Move || mode == ResizeMode::None)
            output.moveTop(base);
        else
            output.setTop(base);
    } else if (output.top() > padding + heightR) {
        output.moveTop(padding + heightR - getHourGap());
    }
    if (output.bottom() > padding + heightR){
        if (mode == ResizeMode::Move || mode == ResizeMode::None)
            output.moveBottom(padding + heightR);
        else
            output.setBottom(padding + heightR);
    }

    if (snap2Time) {
        snapTop(mode, output);
        snapBottom(mode, output);
    }
}

bool ScheduleGridSizeManager::isOnTimePartZone(int i, float pos) {
    return getStartHoursZone() + i*getHourPartitionGap() < pos && pos <= getStartHoursZone() + (i+1)*getHourPartitionGap();
}

float ScheduleGridSizeManager::getTimeZoneEnd(int i) {
    return getStartHoursZone() + (i+1)*getHourPartitionGap();
}

float ScheduleGridSizeManager::getTimeZoneStart(int i) {
    return getStartHoursZone() + i*getHourPartitionGap();
}

QRectF ScheduleGridSizeManager::getPositionOnSchedule(QRectF quad, ResizeMode mode) {

    QRectF output = quad;

    foundHorizontalPosition(output);
    foundVerticalPosition(output, mode);

    return output;
}

float ScheduleGridSizeManager::getStartWeekZone() {
    return padding +getHourWidth();
}

float ScheduleGridSizeManager::getStartHoursZone() {
    return padding + getHeaderHeight();
}

int ScheduleGridSizeManager::getSecPartition() {
    return std::round(3600.0/timeParts);
}

QTime ScheduleGridSizeManager::buildQTime(int sH, int m) {
    if (m >= 60) {
        int hours = m/60;
        m = m%60;
        sH+=hours;
    }
    if (m < 0){
        int hours = m/60;
        m = m%60;
        sH += hours;
        m = std::abs(m);
    }
    if (sH == 24 && m == 0) {
        sH = 23;
        m = 59;
    }
    if (sH < 0){
        sH =sH%24;
        sH = 24 + sH;
    }
    sH = sH%24;
    m = m%60;
    qDebug() << sH << ":" << m;
    return QTime(sH, m);
}

QTime ScheduleGridSizeManager::getTimeByPosition(float posY) {
    int i = findTimeZone(posY);
    float minuteGap = 60.0 / timeParts;
    if (0 <= i && i < elapsedHours * timeParts) {
        float iniTimeZone = getTimeZoneStart(i);
        float relativePos = (posY - iniTimeZone) / (getHourPartitionGap());
        float sTime = 60 * sHour + i * minuteGap;
        int sH = sTime / 60;
        int m = std::round((sTime - 60 * sH) + (minuteGap)*relativePos);
        return buildQTime(sH, m);
    } else if (i < 0) {
        return buildQTime(sHour, 0);
    } else {
        return buildQTime(eHour, 0);
    }
}

int ScheduleGridSizeManager::getDayByPosition(float posX){
    int i=0;
    while (i < 7 && !isOnDayZone(i, posX)) i++;
    return (0 <= i && i <= elapsedHours)? i : -1;
}

float ScheduleGridSizeManager::getPositionByTime(QTime start)
{
    int i = start.hour()-sHour;
    float relativePos = start.minute()/60.0;
    float pos = getStartHoursZone() + i*getHourGap() + relativePos*getHourGap();

    return pos;
}

QRectF ScheduleGridSizeManager::getActivitySizeByTimeRange(QTime start, QTime end, int dayWeek) {
    float top = getPositionByTime(start);
    float bottom = getPositionByTime(end);
    float left = getStartWeekZone() + dayWeek*getDayWidth();

    return QRectF(QPointF(left, top), QSize(getDayWidth(), bottom-top));
}

float ScheduleGridSizeManager::getHourWidth() {
    QFontMetricsF metric(textFont);
    return metric.horizontalAdvance("99:99") + 2*padding;
}



