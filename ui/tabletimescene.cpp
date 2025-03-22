#include "tabletimescene.h"
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QIODevice>
#include <QDebug>
#include <QTime>

#include "../timetable/models/activity.h"

TableTimeScene::TableTimeScene(SchedulePoolManager *schedulePoolManager,
                               ScheduleGridSizeManager *gridManager,
                               ThemeColors *theme,
                               QObject *parent)
    : QGraphicsScene{parent},
    schPoolMng(schedulePoolManager),
    currentOwner(nullptr),
    gridMng(gridManager),
    theme(theme){
}

void TableTimeScene::reload() {
    if(currentOwner != nullptr && schPoolMng != nullptr){
        for (auto i=schPoolMng->begin(); i != schPoolMng->end(); ++i) {
            QScheduleActivity *act = static_cast<QScheduleActivity *>(*i);
            if (act->getOwnerName() == currentOwner->getName()){
                connect(act, &QScheduleActivity::deleteMe, this, &TableTimeScene::deleteActivity);
                act->setTheme(theme);
                act->setGridManager(gridMng);
                act->setPoolManager(schPoolMng);
                act->calculateInitialPosition();
                addItem(act);
            }
        }
    }

}

void TableTimeScene::clear()
{
    QGraphicsScene::clear();
}

void TableTimeScene::clearOwnerData(Owner *currentOwner)
{
    for(auto it = schPoolMng->begin(); it != schPoolMng->end(); ++it){
        QScheduleActivity *act = static_cast<QScheduleActivity *>(*it);
        if (act->getOwnerName() != currentOwner->getName()){
            removeItem(act);
            //act->setParent(nullptr);
        }else{
            act->setEnabled(true);
            act->update();
        }
    }
}

void TableTimeScene::changeCurrentOwner(Owner *currentOwner) {
    if (currentOwner != nullptr && schPoolMng != nullptr){
        clearOwnerData(currentOwner);
    }
    if (this->currentOwner != currentOwner){
        this->currentOwner = currentOwner;
        reload();
    }
}

void TableTimeScene::updateViewSelection(QList<Owner *> selection) {
    updateandremovecurrentView(selection);
    addDisabledActivity(selection);
}

void TableTimeScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event){
    if (event->mimeData()->hasFormat("application/x-qwidget")) {
        event->acceptProposedAction();
    }
}

void TableTimeScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event) {
    if (event->mimeData()->hasFormat("application/x-qwidget")) {
        event->acceptProposedAction();
    }
}

void TableTimeScene::dropEvent(QGraphicsSceneDragDropEvent *event) {
    if (event->mimeData()->hasFormat("application/x-qwidget")) {
        QByteArray itemData = event->mimeData()->data("application/x-qwidget");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QString offset;
        dataStream >> offset;

        QStringList data = offset.split("\"|\"");

        QScheduleActivity* newItem = new QScheduleActivity(Activity(data[0], data[1]),
                                                           *currentOwner,
                                                           QTime::currentTime(),
                                                           QTime::currentTime().addSecs(560),
                                                           0);
        connect(newItem, &QScheduleActivity::deleteMe, this, &TableTimeScene::deleteActivity);

        newItem->setTheme(theme);
        newItem->setHeight(gridMng->getHourGap());
        newItem->setGridManager(gridMng);
        newItem->setPoolManager(schPoolMng);
        newItem->setPos(event->scenePos());
        newItem->calculateFinalPosition();
        addItem(newItem);
        schPoolMng->addActivity(newItem);

        event->acceptProposedAction();
    }

}

void TableTimeScene::deleteActivity(QScheduleActivity *act) {
    schPoolMng->removeScheduleActivity(act);
    removeItem(act);
    delete act;
}

void TableTimeScene::updateandremovecurrentView(QList<Owner *> &selection) {
    if (currentOwner != nullptr){
        QList<QGraphicsItem*> sceneItems = items();
        for(auto it=sceneItems.begin(); it != sceneItems.end(); ++it){
            if (QScheduleActivity *act = dynamic_cast<QScheduleActivity *>(*it)){
                bool isCurrentOwner = act->getOwnerName() == currentOwner->getName();
                int idx = isCurrentOwner? -1 : isOnOwnerList(act->getOwnerName(),
                                                              selection);
                if (isCurrentOwner || idx >= 0){
                    act->setMultiViewPosition(idx, selection.size());
                }else {
                    removeItem(act);
                }
            }
        }
    }

}

void TableTimeScene::addDisabledActivity(QList<Owner *> &selection) {
    if(currentOwner != nullptr && schPoolMng != nullptr){
        for (auto i=schPoolMng->begin(); i != schPoolMng->end(); ++i) {
            QScheduleActivity *act = static_cast<QScheduleActivity *>(*i);
            /// This algorithm can be improve if we first group by owner to only use isOnOwnerList by owner (expected 1 time)
            if (isOnOwnerList(act->getOwnerName(), selection) >= 0){
                act->setMultiViewPosition( isOnOwnerList(act->getOwnerName(),
                                                        selection),
                                          selection.size());
                addItem(act);
            }
        }
    }
}

int TableTimeScene::isOnOwnerList(QString name, QList<Owner *> &selection) {
    bool found = false;
    int i = 0;
    for(auto it=selection.begin(); it != selection.end() && !found; ++it){
        found = (*it)->getName().compare(name) == 0;
        if (!found){i++;}
    }
    return found? i : -1;
}
