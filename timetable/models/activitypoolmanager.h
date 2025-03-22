#ifndef ACTIVITYPOOLMANAGER_H
#define ACTIVITYPOOLMANAGER_H
#include "activity.h"
#include <QString>
#include <QList>

class ActivityPoolManager
{
public:
    ActivityPoolManager();
    ~ActivityPoolManager();
    void addActivity(Activity *act);
    void removeActivity(QString name, QString classification);
    void removeActivity(Activity *act);
    QList<QString> getActivityNames();
    QList<QString> getClassifications();
    Activity *getActivity(size_t i);
    QList<Activity *> getByClassification(QString classification);
    size_t size();
    void clear() { pool.clear(); }
    Activity* find(QString name, QString classification);
private:
    QList<Activity *> pool;
};

#endif // ACTIVITYPOOLMANAGER_H
