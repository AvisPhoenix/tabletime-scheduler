#ifndef TIMEINTERVAL_H
#define TIMEINTERVAL_H

#include <QTime>

class TimeInterval {
public:
    TimeInterval(QTime start, QTime end){
        qDebug() << "Time: " << start << end;
        if (end >= start){ this->start = start; this->end = end;}
        else { this->start = end; this->end = start; }
    }
    QTime getStart(){return start;}
    QTime getEnd(){return end;}
    QTime elapsedTime(){
        return QTime::fromMSecsSinceStartOfDay(end.msecsSinceStartOfDay() - start.msecsSinceStartOfDay());
    }
    bool isInside(const QTime &time){
        return (time > start && time < end);
    }
    bool isOverloap(const TimeInterval &other) {
        return isInside(other.start) || isInside(other.end);
    }
    bool operator==(const TimeInterval& b) const {
        return start == b.start && end == b.end;
    }
    TimeInterval& operator=(const TimeInterval& b){
        if (this != &b){
            start = b.start;
            end = b.end;
        }
        return *this;
    }
private:
    QTime start, end;
};

#endif // TIMEINTERVAL_H
