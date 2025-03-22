#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <QString>

class Activity
{
public:
    Activity() {this->name = "No Name Activity"; this->classification="None"; }
    Activity(QString name, QString classification = "None") {this->name = name; this->classification=classification; }
    QString name;
    QString classification;
    Activity& operator=(const Activity& b){
        if (this != &b){
            name = b.name;
            classification = b.classification;
        }
        return *this;
    }
    bool operator==(const Activity& b) const {return name.compare(b.name) == 0 && classification.compare(b.classification) == 0;}
};

#endif // ACTIVITY_H
