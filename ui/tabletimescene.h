#ifndef TABLETIMESCENE_H
#define TABLETIMESCENE_H

#include <QGraphicsScene>
#include "../timetable/models/owner.h"
#include "../timetable/models/schedulegridsizemanager.h"
#include "../timetable/models/schedulepoolmanager.h"
#include "../timetable/ui/qscheduleactivity.h"
#include "../models/themecolors.h"


class TableTimeScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit TableTimeScene(SchedulePoolManager *schedulePoolManager, ScheduleGridSizeManager *gridManager, ThemeColors *theme, QObject *parent = nullptr);
    void reload();
    void clear();
    void changeCurrentOwner(Owner *currentOwner);
    void updateViewSelection(QList<Owner *> selection);
protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override ;
    void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override ;
    void dropEvent(QGraphicsSceneDragDropEvent* event) override ;
private slots:
    void deleteActivity(QScheduleActivity *);
private:
    Owner *currentOwner;
    SchedulePoolManager *schPoolMng;
    ScheduleGridSizeManager *gridMng;
    ThemeColors *theme;

    void updateandremovecurrentView(QList<Owner *> &selection);
    void addDisabledActivity(QList<Owner *> &selection);
    int isOnOwnerList(QString name, QList<Owner *> &selection);
    void clearOwnerData(Owner *currentOwner);
};

#endif // TABLETIMESCENE_H
