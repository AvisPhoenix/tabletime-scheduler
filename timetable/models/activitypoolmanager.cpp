#include "activitypoolmanager.h"


ActivityPoolManager::ActivityPoolManager() {
    pool.clear();
}

ActivityPoolManager::~ActivityPoolManager(){
    pool.clear();
}

void ActivityPoolManager::addActivity(Activity *act) {
    pool.push_back(act);
}

void ActivityPoolManager::removeActivity(QString name, QString classification) {
    Activity *act = nullptr;
    for(auto *activity : std::as_const(pool)){
        if (activity->name.compare(name) == 0 && activity->classification.compare(classification) == 0)
            act = activity;
    }
    removeActivity(act);
}

void ActivityPoolManager::removeActivity(Activity *act) {
    pool.removeAll(act);
}

QList<QString> ActivityPoolManager::getActivityNames() {
    QList<QString> names;
    for (auto *activity : std::as_const(pool)){
        names.push_back(activity->name);
    }
    return names;
}

QList<QString> ActivityPoolManager::getClassifications() {
    QList<QString> classifications;

    for (auto *activity : std::as_const(pool)){
        classifications.push_back(activity->classification);
    }

    std::sort(classifications.begin(), classifications.end());

    auto last = std::unique(classifications.begin(), classifications.end());
    if (last != classifications.end()) classifications.erase(last);

    return classifications;
}

Activity *ActivityPoolManager::getActivity(size_t i) {
    int idx=0;
    auto it = pool.begin();
    while (idx < i && it != pool.end()){
        it++;
        idx++;
    }
    return *it;
}

QList<Activity *> ActivityPoolManager::getByClassification(QString classification) {
    QList<Activity *> byClassify;
    std::copy_if(pool.begin(), pool.end(),std::back_inserter(byClassify), [&classification]( Activity *act){
        return act->classification.compare(classification) == 0;
    });
    return byClassify;
}

size_t ActivityPoolManager::size() {
    return pool.size();
}

Activity *ActivityPoolManager::find(QString name, QString classification) {
    Activity *output= nullptr;
    for (auto *activity : std::as_const(pool)){
        if (activity->classification.compare(classification)==0 && activity->name.compare(name) == 0) {
            output = activity;
        }
    }
    return output;
}
