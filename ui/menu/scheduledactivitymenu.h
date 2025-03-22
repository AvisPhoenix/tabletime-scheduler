#ifndef SCHEDULEDACTIVITYMENU_H
#define SCHEDULEDACTIVITYMENU_H

#include <QMenu>

class ScheduledActivityMenu : public QMenu
{
    Q_OBJECT
public:
    ScheduledActivityMenu(QTime start, QTime end);
signals:
    void changeStartTime(QTime);
    void changeEndTime(QTime);
    void askDelete();
};

#endif // SCHEDULEDACTIVITYMENU_H
