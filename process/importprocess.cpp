#include "importprocess.h"

ImportProcess::ImportProcess(ActivityPoolManager *activityPool, SchedulePoolManager *schedule, QList<Owner *> *owners, ColorGenerator *colorGenerator, QString filePath, ImportOptions options, ImportPriority priority):
    filePath(filePath),
    importOptions(options),
    importPriority(priority){
    fileImporter = new TimeTableFile<QActivity, QScheduleActivity>(activityPool, schedule, owners, colorGenerator);
}

ImportProcess::~ImportProcess() {
    delete fileImporter;
}

void ImportProcess::setArguments(QString filePath, ImportOptions options, ImportPriority priority) {
    this->filePath = filePath;
    importOptions = options;
    importPriority = priority;
}

void ImportProcess::run() {
    QList<QString> messages = fileImporter->import(filePath.toStdString(),importOptions, importPriority);
    emit processMessages(messages);
}
