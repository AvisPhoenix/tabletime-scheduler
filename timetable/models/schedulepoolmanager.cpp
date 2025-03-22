#include "schedulepoolmanager.h"

SchedulePoolManager::SchedulePoolManager() {
    pool.clear();
}

SchedulePoolManager::~SchedulePoolManager(){
    pool.clear();
}

void SchedulePoolManager::addActivity(ScheduledActivity *schAct) {
    if (find(schAct, pool) == nullptr) {
        pool.push_back(schAct);
    }
}

void SchedulePoolManager::removeScheduleActivity(ScheduledActivity *schAct) {
    ScheduledActivity *realAct = find(schAct, pool);
    if (realAct != nullptr){
        pool.remove(realAct);
    }
}

list<ScheduledActivity *> SchedulePoolManager::getScheduleByOwnerName(const QString &owner) {
    list<ScheduledActivity *> output;
    std::copy_if(pool.begin(), pool.end(),std::back_inserter(output), [&owner](ScheduledActivity *act){
        return act->getOwnerName() == owner;
    });
    return output;
}

bool SchedulePoolManager::isValidScheduleChange(ScheduledActivity *schAct) {
    bool isValid=true;
    for(ScheduledActivity *act : pool){
        if (schAct != act && schAct->getOwnerName() == act->getOwnerName() && schAct->isOverlap(*act)){
            isValid = false;
        }
    }
    return isValid;
}

list<ScheduledActivity *> SchedulePoolManager::getInvalidScheduleByOwnerName(const QString &owner) {
    list<ScheduledActivity *> invalidList;
    std::copy_if(pool.begin(), pool.end(),std::back_inserter(invalidList), [&owner, &invalidList, this](ScheduledActivity *act){
        return act->getOwnerName() == owner && this->find(act, invalidList) == nullptr && !this->isValidScheduleChange(act);
    });
    return invalidList;
}

ScheduledActivity *SchedulePoolManager::getActivity(size_t i) {
    int idx=0;
    auto it = pool.begin();
    while (idx < i && it != pool.end()){
        it++;
        idx++;
    }
    return *it;
}

ScheduledActivity *SchedulePoolManager::find(ScheduledActivity *schAct) {
    return find(schAct, pool);
}

ScheduledActivity *SchedulePoolManager::find(ScheduledActivity *schAct, const list<ScheduledActivity*> &actPool) {
    ScheduledActivity * found = nullptr;
    for (ScheduledActivity *act : actPool) {
        if (found == nullptr && *act == *schAct) found = act;
    }
    return found;
}
