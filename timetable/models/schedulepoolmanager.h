#ifndef SCHEDULEPOOLMANAGER_H
#define SCHEDULEPOOLMANAGER_H
#include "scheduledactivity.h"
#include <QString>
#include <list>
#include <map>

using std::list;
using std::map;

class SchedulePoolManager
{
public:
    SchedulePoolManager();
    ~SchedulePoolManager();
    void addActivity(ScheduledActivity *schAct);
    void removeScheduleActivity(ScheduledActivity *schAct);
    list<ScheduledActivity*> getScheduleByOwnerName(const QString &owner);
    bool isValidScheduleChange(ScheduledActivity *schAct);
    list<ScheduledActivity*> getInvalidScheduleByOwnerName(const QString &owner);
    void clear() { pool.clear(); }
    size_t size() {return pool.size(); }
    ScheduledActivity *getActivity(size_t i);
    list<ScheduledActivity*>::iterator begin(){return pool.begin();}
    list<ScheduledActivity*>::iterator end(){return pool.end();}
    ScheduledActivity* find(ScheduledActivity *schAct);
private:
    ScheduledActivity* find(ScheduledActivity *schAct, const list<ScheduledActivity*> &actPool);
    list<ScheduledActivity*> pool;
};

#endif // SCHEDULEPOOLMANAGER_H
