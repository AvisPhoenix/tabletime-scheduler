#ifndef QSCHEDULEACTIVITY_H
#define QSCHEDULEACTIVITY_H

#include <QGraphicsItem>
#include "../models/scheduledactivity.h"
#include "../models/schedulegridsizemanager.h"
#include "../models/resizemode.h"
#include "../models/schedulepoolmanager.h"
#include "../../models/themecolors.h"

class QScheduleActivity : public QObject, public QGraphicsItem, public ScheduledActivity
{
    Q_OBJECT
public:
    QScheduleActivity(const Activity &activity,const Owner &owner, QTime start, QTime end, int dayOfWeek);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void setColor(QColor color, int fillAlpha){ this->color = color; aFill = fillAlpha; }
    void setOverlapColor(QColor overlap) { this->overlap = overlap; }
    void setDisableSaturation(float factor) {desaturateF = factor;}
    void setWidth(int width);
    void setHeight(int height);
    void setSize(int width, int height);
    void setRound(float roundPercentage){ round = roundPercentage; }
    void setEnabled(bool enabled) {this->enabled = enabled;}
    void setGridManager(ScheduleGridSizeManager *gridManager) { gridMng = gridManager; }
    void setPoolManager(SchedulePoolManager *schedulePoolManager) { scheduleManager = schedulePoolManager;}
    void calculateInitialPosition();
    void calculateFinalPosition();
    bool isOverlaped(){ return overlaped;}
    void setMultiViewPosition(int ownerIdx, int ownerCount);
    void setTheme(ThemeColors *theme){ this->theme = theme; }
    QColor fontColor;
    ResizeMode resizeMode;

protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
signals:
    void deleteMe(QScheduleActivity *);
private slots:
    void updateStartTime(QTime);
    void updateEndTime(QTime);
    void onDelete();
private:
    QRectF rectModel;
    QRectF originalModel;
    float round;
    float desaturateF;
    int aFill;
    QColor color, overlap;
    bool enabled;
    bool overlaped;
    ScheduleGridSizeManager *gridMng;
    SchedulePoolManager *scheduleManager;
    ThemeColors *theme;

    void setRect(const QRectF& newRect);
    void drawOwnerID(QPainter *painter);
    void drawRoundRect(QPainter *painter);
    void drawName(QPainter *painter);
    QPointF fixObsoletePosition();
    QRectF getCurrentRectModel();
    void setFinalModel(QRectF currentModel);
    QRectF getFinalModel(QRectF currentModel);
    void setFinalValues(QRectF nrectModel);
    void checkOverlap();
    QColor desaturateColor(QColor color);
    double calcRound();
    void roundedID(double fround, QPainter *painter);
    void rectID(QPainter *painter);
};

#endif // QSCHEDULEACTIVITY_H
