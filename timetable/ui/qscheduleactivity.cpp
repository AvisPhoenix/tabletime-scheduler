#include "qscheduleactivity.h"
#include <QBrush>
#include <QColor>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QPainterPath>
#include <QPointF>
#include <QSizeF>

#include "../../ui/menu/scheduledactivitymenu.h"


QScheduleActivity::QScheduleActivity(const Activity &activity, const Owner &owner, QTime start, QTime end, int dayOfWeek) :
    ScheduledActivity(activity, owner, start, end, dayOfWeek),
    enabled(true), aFill(70), resizeMode(ResizeMode::None), round(10), desaturateF(0.3),
    gridMng(nullptr), scheduleManager(nullptr), theme(nullptr)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    rectModel = QRectF(-50, -50, 100, 100);
    color = Qt::red;
    setAcceptHoverEvents(true);
}

QRectF QScheduleActivity::boundingRect() const {
    return QRectF(rectModel.x()-1, rectModel.y()-1, rectModel.width()+2, rectModel.height()+2);
}

double QScheduleActivity::calcRound()
{
    double fround =0;
    if (rectModel.height() >= 10 && rectModel.height() < 50){
        fround = (round/2400.0)*(rectModel.height()*rectModel.height()) - ((round*100)/2400.0);
    }else if (rectModel.height() >= 50){
        fround = round;
    }

    return fround;
}

void QScheduleActivity::roundedID(double fround, QPainter *painter)
{
    QPainterPath path;
    QPointF cornerRadius(fround,fround);
    QRectF rect = rectModel;
    rect.setWidth(round+2);

    path.moveTo(rect.topLeft() + QPoint(cornerRadius.x(),0));
    path.arcTo(QRectF(rect.topLeft(), QSizeF(cornerRadius.x(), cornerRadius.y())), 90, 90);
    path.lineTo(rect.bottomLeft()- QPoint(0, cornerRadius.y()));
    path.arcTo(QRectF(rectModel.bottomLeft() - QPointF(0, cornerRadius.y()), QSizeF(cornerRadius.x(), cornerRadius.y())), 180, 90);
    path.lineTo(rect.bottomRight());
    path.lineTo(rect.topRight());

    path.closeSubpath();

    painter->setBrush(owner->getColor());
    painter->setPen(owner->getColor());
    painter->drawPath(path);
}

void QScheduleActivity::rectID(QPainter *painter)
{
    QRectF rect = rectModel;
    rect.setWidth(round+2);
    painter->setBrush(owner->getColor());
    painter->setPen(owner->getColor());
    painter->drawRect(rect);
}

void QScheduleActivity::drawOwnerID(QPainter *painter) {
    double fround = calcRound();
    if (fround >= 0){
        roundedID(fround, painter);
    } else{
        rectID(painter);
    }
}

void QScheduleActivity::drawName(QPainter *painter)
{
    painter->setPen(theme == nullptr? fontColor: theme->getScheduleActivityColor("font"));
    QTextOption tOptions;
    tOptions.setAlignment(Qt::AlignCenter);
    tOptions.setWrapMode(QTextOption::WordWrap);
    painter->drawText(rectModel,name,tOptions);
}

void QScheduleActivity::drawRoundRect(QPainter *painter) {
    QColor overlapFillColor = theme == nullptr? overlap : theme->getScheduleActivityColor("overlap");
    int alphaValue = theme == nullptr? aFill : theme->alphaValue("activity");
    QColor colorFill = theme == nullptr? color : theme->getActivityColor(classification);
    QColor outsideColor = colorFill;

    overlapFillColor.setAlpha(alphaValue);
    colorFill.setAlpha(alphaValue);

    QColor fColor = overlaped? overlapFillColor
                            : colorFill;
    painter->setPen(outsideColor);
    if (!enabled){
        fColor = desaturateColor(fColor);
        painter->setPen(Qt::transparent);
    }
    double fround = calcRound();

    painter->setBrush(fColor);
    if (fround >= 0)
        painter->drawRoundedRect(rectModel, fround, fround);
    else
        painter->drawRect(rectModel);
}

void QScheduleActivity::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setRenderHint(QPainter::Antialiasing);

    drawRoundRect(painter);
    drawOwnerID(painter);
    drawName(painter);
}

void QScheduleActivity::setWidth(int width) {
    QRectF rect = rectModel;
    setRect(QRectF(-width/2.0, rect.y(), width, rect.height()));
}

void QScheduleActivity::setHeight(int height) {
    QRectF rect = rectModel;
    setRect(QRectF(rect.x(), -height/2.0, rect.width(), height));
}
void QScheduleActivity::setSize(int width, int height)
{
    setRect(QRectF(-width/2.0,-height/2.0,width,height));
}

void QScheduleActivity::calculateInitialPosition() {
    if (gridMng != nullptr){
        QRectF nModel = gridMng->getActivitySizeByTimeRange(time.getStart(), time.getEnd(), dayOfWeek);
        setSize(nModel.width(), nModel.height());
        setPos(nModel.center());
    }
    checkOverlap();
}

void QScheduleActivity::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    if (enabled){
        QPointF pos = event->pos();

        if (pos.y() < rectModel.top() + 10 || pos.y() > rectModel.bottom() - 10) {
            setCursor(Qt::SizeVerCursor);
        } else {
            setCursor(Qt::OpenHandCursor);
        }
    } else {
        setCursor(Qt::ArrowCursor);
    }
    QGraphicsItem::hoverMoveEvent(event);
}

void QScheduleActivity::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (enabled){
        QPointF pos = event->pos();

        if (pos.y() < rectModel.top() + 10) {
            resizeMode = ResizeMode::Top;
            originalModel = rectModel;
        } else if (pos.y() > rectModel.bottom() - 10) {
            resizeMode = ResizeMode::Bottom;
            originalModel = rectModel;
        } else {
            resizeMode = ResizeMode::Move;
            setCursor(Qt::ClosedHandCursor);
            QGraphicsItem::mousePressEvent(event);
        }
    }
}

void QScheduleActivity::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (enabled){
        QPointF pos = event->pos();
        QRectF rect = rectModel;

        if (resizeMode == ResizeMode::Top) {
            rect.setTop(pos.y());
            setRect(rect);
        } else if (resizeMode == ResizeMode::Bottom) {
            rect.setBottom(pos.y());
            setRect(rect);
        } else {
            QGraphicsItem::mouseMoveEvent(event);
        }
    }
}

void QScheduleActivity::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (enabled){
        calculateFinalPosition();
        setCursor(Qt::ArrowCursor);
    }
    resizeMode = ResizeMode::None;
    QGraphicsItem::mouseReleaseEvent(event);
}

void QScheduleActivity::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    ScheduledActivityMenu *contextMenu = new ScheduledActivityMenu(time.getStart(), time.getEnd());
    connect(contextMenu, &ScheduledActivityMenu::changeStartTime, this, &QScheduleActivity::updateStartTime);
    connect(contextMenu, &ScheduledActivityMenu::changeEndTime, this, &QScheduleActivity::updateEndTime);
    connect(contextMenu, &ScheduledActivityMenu::askDelete, this, &QScheduleActivity::onDelete);

    contextMenu->popup(event->screenPos());
}

void QScheduleActivity::updateStartTime(QTime start) {
    changeStartTime(start);
    calculateInitialPosition();
    update();
}

void QScheduleActivity::updateEndTime(QTime end) {
    changeEndTime(end);
    calculateInitialPosition();
    update();
}

void QScheduleActivity::onDelete() {
    emit deleteMe(this);
}

QPointF QScheduleActivity::fixObsoletePosition()
{
    QPointF realPosition = pos();
    if (resizeMode == ResizeMode::Bottom){
        realPosition = pos() + QPointF(0, (rectModel.bottom() - originalModel.bottom())/2.0);
    }else if (resizeMode == ResizeMode::Top){
        realPosition = pos() + QPointF(0, (rectModel.top() - originalModel.top())/2.0);
    }

    return realPosition;
}

QRectF QScheduleActivity::getCurrentRectModel()
{
    QRectF currentModel = rectModel;
    QPointF realPosition = fixObsoletePosition();
    currentModel.moveCenter(realPosition);

    return currentModel;
}

QRectF QScheduleActivity::getFinalModel(QRectF currentModel)
{
    QRectF nrectModel = gridMng->getPositionOnSchedule(currentModel, resizeMode);
    setSize(nrectModel.width(), nrectModel.height());
    setPos(nrectModel.center());

    return nrectModel;
}

void QScheduleActivity::setFinalValues(QRectF nrectModel)
{
    time = TimeInterval(gridMng->getTimeByPosition(nrectModel.top()), gridMng->getTimeByPosition(nrectModel.bottom()));
    dayOfWeek = gridMng->getDayByPosition(nrectModel.center().x());
}

void QScheduleActivity::checkOverlap(){
    overlaped = false;
    if (scheduleManager != nullptr){
        for(auto i =scheduleManager->begin(); i != scheduleManager->end() && !overlaped; ++i){
            QScheduleActivity *act = static_cast<QScheduleActivity*>(*i);
            if (act->getOwnerName() == getOwnerName() && act != this && act->dayOfWeek == dayOfWeek ){
                overlaped = isOverlap(*act);
            }
        }
    }
}

QColor QScheduleActivity::desaturateColor(QColor color) {
    // get HSL representation
    float h, s, l, a;
    color.getHslF(&h, &s, &l, &a);

    //reduce saturate
    s *= desaturateF;

    QColor desaturate;
    desaturate.setHslF(h, s, l, a);

    return desaturate;
}

void QScheduleActivity::calculateFinalPosition(){
    if (gridMng != nullptr){
        QRectF currentModel = getCurrentRectModel();
        QRectF nrectModel = getFinalModel(currentModel);
        setFinalValues(nrectModel);
    }
    checkOverlap();
}

void QScheduleActivity::setMultiViewPosition(int ownerIdx, int ownerCount) {
    if (ownerIdx < 0){
        setZValue(2);
        setEnabled(true);
    }else {
        setZValue(0);
        setEnabled(false);
        QRectF currentModel = getCurrentRectModel();
        QRectF nrectModel = getFinalModel(currentModel);
        float width = gridMng->getDayWidth()/ownerCount;
        float change = (gridMng->getDayWidth() - width)/2.0;
        setWidth(width);
        QPointF nPos = nrectModel.center() + QPointF(-change + ownerIdx*width,0);
        setPos(nPos);
    }
}

void QScheduleActivity::setRect(const QRectF &newRect) {
    prepareGeometryChange();
    if (newRect.height() <=  2){
        rectModel = QRectF(newRect.x(), newRect.y(), newRect.width(), 2);
    }else {
        rectModel = newRect;
    }
    update();
}
