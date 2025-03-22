#ifndef IMPORTPROCESS_H
#define IMPORTPROCESS_H

#include <QThread>
#include "../models/importoptions.h"
#include "../models/colorGenerators/colorgenerator.h"
#include "../timetable/parser/timetablefile.h"
#include "../timetable/ui/qactivity.h"
#include "../timetable/ui/qscheduleactivity.h"

class ImportProcess : public QThread
{
    Q_OBJECT
public:
    ImportProcess(ActivityPoolManager *activityPool,
                  SchedulePoolManager *schedule,
                  QList<Owner *> *owners,
                  ColorGenerator *colorGenerator,
                  QString filePath="",
                  ImportOptions options = ImportOptions::None,
                  ImportPriority priority = ImportPriority::CurrentFile);
    ~ImportProcess();
    void setArguments(QString filePath,
                    ImportOptions options,
                    ImportPriority priority);
signals:
    void processMessages(QList<QString>);
protected:
    void run() override;
private:
    TimeTableFile<QActivity, QScheduleActivity> *fileImporter;
    QString filePath;
    ImportOptions importOptions;
    ImportPriority importPriority;
};

#endif // IMPORTPROCESS_H
