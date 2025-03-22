#ifndef SCHEDULEDACTIVITY_H
#define SCHEDULEDACTIVITY_H

#include "activity.h"
#include "owner.h"
#include "timeinterval.h"

class ScheduledActivity : public Activity
{
public:
    ScheduledActivity(QString name,const Owner &owner, QTime start, QTime end, int dayOfWeek): time(start, end){
        this->name = name;
        this->dayOfWeek = dayOfWeek%7;
        classification = "None";
        this->owner = &owner;
    }
    ScheduledActivity(const Activity &activity,const Owner &owner, QTime start, QTime end, int dayOfWeek): time(start, end){
        name = activity.name;
        classification = activity.classification;
        this->dayOfWeek = dayOfWeek%7;
        this->owner = &owner;
    }
    QString classification;
    QString getOwnerName(){return owner->getName(); }
    QTime getStartTime() {return time.getStart();}
    QTime getEndTime() {return time.getEnd();}
    void changeStartTime(QTime start){
        time = TimeInterval(start, time.getEnd());
    }
    void changeEndTime(QTime end){
        time = TimeInterval(time.getStart(), end);
    }
    bool isOverlap(const ScheduledActivity& b){
        return time.isOverloap(b.time);
    }
    ScheduledActivity& operator=(const ScheduledActivity& b){
        if (this != &b){
            name = b.name;
            classification = b.classification;
            dayOfWeek = b.dayOfWeek;
            time = b.time;
            owner = b.owner;
        }
        return *this;
    }
    bool operator==(const ScheduledActivity& b) const{
        return dayOfWeek == b.dayOfWeek && name.compare(b.name)==0 && *owner == *b.owner && time == b.time;
    }
    int getDayOfWeek() const { return dayOfWeek; }
    void setDayOfWeek(int newDayOfWeek) {dayOfWeek = newDayOfWeek%7;}

protected:
    const Owner *owner;
    TimeInterval time;
    int dayOfWeek;
};

#endif // SCHEDULEDACTIVITY_H
