#ifndef IMPORTDLG_H
#define IMPORTDLG_H

#include <QDialog>
//#include "../process/importprocess.h"
#include "../models/importoptions.h"
#include "../timetable/parser/timetablefile.h"
#include "../timetable/ui/qactivity.h"
#include "../timetable/ui/qscheduleactivity.h"
#include "../models/colorGenerators/colorgenerator.h"


namespace Ui {
class ImportDlg;
}

class ImportDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ImportDlg(ActivityPoolManager *activityPool, SchedulePoolManager *schedule, QList<Owner *> *owners, ColorGenerator *colorGenerator, QString fileExtension, QWidget *parent = nullptr);
    ~ImportDlg();

signals:
    void finishImport();

protected:
    void closeEvent(QCloseEvent*) override;

private slots:
    void mergeMode(Qt::CheckState);
    void startImport();
    void endImport();
    void openFilePath();
    void changeFilePath(QString);
    void showProcessMessages(QList<QString>);

private:
    Ui::ImportDlg *ui;
    const QString fileExtension = "yml";
    bool lockClose;
    //ImportProcess *importProcess;
    TimeTableFile<QActivity, QScheduleActivity> *fileImporter;


    void startImportProcess();
    void prepareImport();
    ImportOptions getOptions();
    ImportPriority getPriority();
};

#endif // IMPORTDLG_H
