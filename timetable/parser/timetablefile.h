#ifndef TIMETABLEFILE_H
#define TIMETABLEFILE_H

#include <type_traits>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <QList>
#include <QString>
#include <QByteArray>
#include <QImage>
#include <QBuffer>
#include "../models/activitypoolmanager.h"
#include "../models/schedulepoolmanager.h"
#include "../models/owner.h"
#include "../models/activity.h"
#include "../models/scheduledactivity.h"
#include "../../models/importoptions.h"
#include "../../models/colorGenerators/colorgenerator.h"

using std::string;
using std::ofstream;
using YAML::Node;

template<typename A,typename S>
class TimeTableFile
{
    static_assert(std::is_base_of<Activity, A>::value, "A must be derived from Activity");
    static_assert(std::is_base_of<ScheduledActivity, S>::value, "B must be derived from ScheduledActivity");
public:
    friend class ConfigurationParser;
    TimeTableFile(ActivityPoolManager *activityPool, SchedulePoolManager *schedule, QList<Owner *> *owners, ColorGenerator *colorGenerator = nullptr, int limit = 10000);
    void save(string filename);
    void load(string filename);
    QList<QString> import(string filename, ImportOptions options, ImportPriority priority);
private:
    Node buildYAMLStructure();
    Owner* findOwner(QString name);
    bool findOwnerColor(QColor color);
    QColor generateUniqueColor();
    int limit;

    ActivityPoolManager *activityPool;
    SchedulePoolManager *schedule;
    QList<Owner *> *owners;
    ColorGenerator *colorGen;
};

template<typename A,typename S>
TimeTableFile<A,S>::TimeTableFile(ActivityPoolManager *activityPool,
                                   SchedulePoolManager *schedule,
                                   QList<Owner *> *owners,
                                   ColorGenerator *colorGenerator,
                                   int limit):
    activityPool(activityPool),
    schedule(schedule),
    owners(owners),
    colorGen(colorGenerator),
    limit(limit){
}

template<typename A,typename S>
void TimeTableFile<A,S>::save(string filename){

    ofstream savedFile(filename);
    if (!savedFile) {
        throw std::runtime_error("Unable to open file for writing: " + filename);
    }
    savedFile << buildYAMLStructure();
    savedFile.close();

}

template<typename A,typename S>
void TimeTableFile<A,S>::load(string filename) {
    Node root = YAML::LoadFile(filename);
    if (root["Activities"].IsSequence()){
        for (std::size_t i=0;i<root["Activities"].size();i++) {
            map<string, string> activity = root["Activities"][i].as<map<string, string>>();
            if (activity.count("name") > 0 && activity.count("classification") > 0){
                activityPool->addActivity(new A(QString(activity["name"].c_str()), QString(activity["classification"].c_str())));
            }
        }
    }
    if(root["Owners"].IsSequence()){
        for (std::size_t i=0;i<root["Owners"].size();i++) {
            map<string, string> owner = root["Owners"][i].as<map<string, string>>();
            if (owner.count("name") > 0 && owner.count("color") > 0 && owner.count("image") > 0){
                Owner *nOwner = new Owner(QString(owner["name"].c_str()));
                nOwner->setColor(QColor::fromString(owner["color"]));
                QByteArray byteArray = QByteArray::fromBase64(QString(owner["image"].c_str()).toUtf8());
                nOwner->setImage(QImage::fromData(byteArray));
                owners->append(nOwner);
            }
        }
    }
    if(root["Schedule"].IsSequence()){
        for (std::size_t i=0;i<root["Schedule"].size();i++) {
            map<string, string> scheduleAct = root["Schedule"][i].as<map<string, string>>();
            if (scheduleAct.count("name") > 0 && scheduleAct.count("classification") > 0
                && scheduleAct.count("ownerName") > 0 && scheduleAct.count("start") > 0
                && scheduleAct.count("end") > 0 && scheduleAct.count("dayOfWeek") > 0){

                Activity activity = Activity(QString(scheduleAct["name"].c_str()),
                                             QString(scheduleAct["classification"].c_str()));
                Owner *owner = findOwner(QString(scheduleAct["ownerName"].c_str()));
                if (owner == nullptr) owner = owners->at(0);
                QTime start = QTime::fromString(QString(scheduleAct["start"].c_str()),"HH:mm");
                QTime end = QTime::fromString(QString(scheduleAct["end"].c_str()),"HH:mm");
                int dayOfWeek = QString(scheduleAct["dayOfWeek"].c_str()).toInt();

                schedule->addActivity(new S(activity, *owner, start, end, dayOfWeek));
            }
        }
    }
}

template<typename A, typename S>
inline QList<QString> TimeTableFile<A, S>::import(string filename, ImportOptions options, ImportPriority priority) {
    if (options == ImportOptions::None) return {"Import none"};

    Node root = YAML::LoadFile(filename);
    QList<QString> output;
    if (options != ImportOptions::UsersOnly && root["Activities"].IsSequence()){
        for (std::size_t i=0;i<root["Activities"].size();i++) {
            map<string, string> activity = root["Activities"][i].as<map<string, string>>();
            if (activity.count("name") > 0 && activity.count("classification") > 0){
                QString name = QString(activity["name"].c_str());
                QString classification =  QString(activity["classification"].c_str());
                Activity *temp = activityPool->find(name,
                                                    classification);
                if (temp == nullptr){
                    activityPool->addActivity(new A(name, classification));
                }
            }
        }
    }

    if (options != ImportOptions::ActivitesOnly && root["Owners"].IsSequence()){
        int ownerSize = root["Owners"].size();
        colorGen->setPartitionSize(owners->size() + ownerSize);
        for (std::size_t i=0;i<ownerSize;i++) {
            map<string, string> owner = root["Owners"][i].as<map<string, string>>();
            if (owner.count("name") > 0 && owner.count("color") > 0 && owner.count("image") > 0){
                QString name = QString(owner["name"].c_str());
                QColor color = QColor::fromString(owner["color"]);
                Owner *existedOwner = findOwner(name);
                bool existedColor = findOwnerColor(color);
                if (existedColor){
                    output.push_back("W_OC_" + color.name());
                    color = generateUniqueColor();
                }
                if (existedOwner == nullptr){
                    Owner *nOwner = new Owner(name);
                    nOwner->setColor(color);
                    QByteArray byteArray = QByteArray::fromBase64(QString(owner["image"].c_str()).toUtf8());
                    nOwner->setImage(QImage::fromData(byteArray));
                    owners->append(nOwner);
                } else if (priority == ImportPriority::ImportedFile) {
                    existedOwner->setColor(color);
                    QByteArray byteArray = QByteArray::fromBase64(QString(owner["image"].c_str()).toUtf8());
                    existedOwner->setImage(QImage::fromData(byteArray));
                }

            }
        }
    }

    if (options == ImportOptions::All && root["Schedule"].IsSequence()){
        for (std::size_t i=0;i<root["Schedule"].size();i++) {
            map<string, string> scheduleAct = root["Schedule"][i].as<map<string, string>>();
            if (scheduleAct.count("name") > 0 && scheduleAct.count("classification") > 0
                && scheduleAct.count("ownerName") > 0 && scheduleAct.count("start") > 0
                && scheduleAct.count("end") > 0 && scheduleAct.count("dayOfWeek") > 0){

                Activity activity = Activity(QString(scheduleAct["name"].c_str()),
                                             QString(scheduleAct["classification"].c_str()));
                Owner *owner = findOwner(QString(scheduleAct["ownerName"].c_str()));
                if (owner == nullptr) owner = owners->at(0);
                QTime start = QTime::fromString(QString(scheduleAct["start"].c_str()),"HH:mm");
                QTime end = QTime::fromString(QString(scheduleAct["end"].c_str()),"HH:mm");
                int dayOfWeek = QString(scheduleAct["dayOfWeek"].c_str()).toInt();
                ScheduledActivity *newScheduleAct= new S(activity, *owner, start, end, dayOfWeek);
                ScheduledActivity *currentScheduleAct = schedule->find(newScheduleAct);
                if (currentScheduleAct == nullptr){
                    schedule->addActivity(newScheduleAct);
                }else {
                    output.push_back("W_SD_" + newScheduleAct->name + "_" +
                                     newScheduleAct->classification + "_" +
                                     newScheduleAct->getOwnerName() + "_" +
                                     QString::number(newScheduleAct->getDayOfWeek()) + "_" +
                                     newScheduleAct->getStartTime().toString("HH:mm") + "_" +
                                     newScheduleAct->getEndTime().toString("HH:mm"));
                }
            }
        }
    }

    return output;
}

template<typename A,typename S>
Node TimeTableFile<A,S>::buildYAMLStructure() {
    Node info;
    Node activities;
    for(size_t i=0; i < activityPool->size(); i++){
        Node act;
        Activity *currentAct = activityPool->getActivity(i);
        act["name"] = currentAct->name.toStdString();
        act["classification"] = currentAct->classification.toStdString();

        activities.push_back(act);
    }
    if (activities.size() > 0)
        info["Activities"] = activities;

    Node ownersNode;
    for(qsizetype i=0; i < owners->size(); i++) {
        Node owner;
        Owner *currentOwner = owners->at(i);

        QString base64 = "";
        if (!currentOwner->getImage().isNull()){
            QByteArray ba;
            QBuffer buffer(&ba);
            buffer.open(QIODevice::WriteOnly);
            currentOwner->getImage().save(&buffer, "PNG");
            base64 = QString(ba.toBase64(QByteArray::Base64Encoding));
        }

        owner["name"] = currentOwner->getName().toStdString();
        owner["color"] = currentOwner->getColor().name().toStdString();
        owner["image"] = base64.toStdString();

        ownersNode.push_back(owner);
    }
    if (ownersNode.size() > 0)
        info["Owners"] = ownersNode;

    Node schedulesNode;
    for(auto i=schedule->begin(); i != schedule->end(); ++i){
        Node act;
        ScheduledActivity *currentAct = *i;

        act["name"] = currentAct->name.toStdString();
        act["classification"] = currentAct->classification.toStdString();
        act["ownerName"] = currentAct->getOwnerName().toStdString();
        act["start"] = currentAct->getStartTime().toString("HH:mm").toStdString();
        act["end"] = currentAct->getEndTime().toString("HH:mm").toStdString();
        act["dayOfWeek"] = currentAct->getDayOfWeek();

        schedulesNode.push_back(act);
    }
    if(schedulesNode.size() > 0)
        info["Schedule"] = schedulesNode;

    return info;
}

template<typename A, typename S>
inline Owner *TimeTableFile<A, S>::findOwner(QString name) {
    Owner *output = nullptr;
    for(auto owner : std::as_const(*owners)){
        if (owner->getName().compare(name) == 0){
            output = owner;
        }
    }
    return output;
}

template<typename A, typename S>
inline bool TimeTableFile<A, S>::findOwnerColor(QColor color) {
    bool found = false;
    for(auto owner : std::as_const(*owners)){
        if (owner->getColor() == color){
            found = true;
        }
    }
    return found;
}

template<typename A, typename S>
inline QColor TimeTableFile<A, S>::generateUniqueColor() {
    if (colorGen != nullptr){
        QColor color = colorGen->gen();
        int count = 0;
        while(!findOwnerColor(color) && count < limit){
            color = colorGen->gen();
            count++;
        }
        return color;
    }
    return QColor(0,0,0);
}

#endif // TIMETABLEFILE_H
