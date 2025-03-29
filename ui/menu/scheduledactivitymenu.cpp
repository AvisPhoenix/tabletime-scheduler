#include "scheduledactivitymenu.h"
#include "labeltime.h"
#include "../svgiconbuilder.h"
#include <QAction>
#include <QWidgetAction>

ScheduledActivityMenu::ScheduledActivityMenu(QTime start, QTime end) {
    LabelTime *startTime = new LabelTime(tr("Start time"), start);
    connect(startTime, &LabelTime::changeTime, this, [this](QTime nvalue){emit changeStartTime(nvalue);});
    QWidgetAction *startAct = new QWidgetAction(this);
    startAct->setDefaultWidget(startTime);

    LabelTime *endTime = new LabelTime(tr("End time"), end);
    connect(endTime, &LabelTime::changeTime, this, [this](QTime nvalue){emit changeEndTime(nvalue);});
    QWidgetAction *endAct = new QWidgetAction(this);
    endAct->setDefaultWidget(endTime);

    QAction *deleteAct = new QAction(SVGIconBuilder::build(":/icons/icons/trash.svg", Qt::red, 32), tr("Delete"));
    connect(deleteAct, &QAction::triggered, this, [this](){emit askDelete();});

    addAction(startAct);
    addAction(endAct);
    addSeparator();
    addAction(deleteAct);
}
