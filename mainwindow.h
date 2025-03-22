#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>

#include "timetable/models/schedulegridsizemanager.h"
#include "timetable/models/schedulepoolmanager.h"
#include "ui/activititesmanager.h"
#include "ui/ownerselectionbutton.h"
#include "ui/viewselectionbutton.h"
#include "ui/tabletimescene.h"
#include "ui/weektabletime.h"
#include "models/themecolors.h"
#include "models/colorGenerators/colorgenerator.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void resizeEvent(QResizeEvent *event);
private slots:
    void importFile();
    void saveSchedule();
    void loadSchedule();
    void newSchedule();
    void displayNewUserDialog();
    void editUserDialog(Owner*);
    void changeCurrentUser(Owner*);
    void finishEditUser(Owner*, bool);
    void changeViewSelection(QList<Owner*>*);
    void onImportData();
    void setupDialog();
private:

    QAction newScheduleAction, loadScheduleAction, saveScheduleAction, importAction, configurationAction;
    QAction configAction;
    QDockWidget *activitiesDock;
    ActivititesManager *container;
    OwnerSelectionButton *userList;
    ViewSelectionButton *viewSelectionBtn;
    TableTimeScene *scene;
    WeekTableTime *weekTableTime;

    ActivityPoolManager activitiesPool;
    SchedulePoolManager schedulePool;
    QList<Owner *> ownerList;
    ClassificationColorManager *colorManager;
    ScheduleGridSizeManager *scheduleSizeManager;
    ThemeColors *theme;
    ColorGenerator *genColor;

    const QString fileExtension = "yml";
    QString configurationFile = "/theme-conf.yml";
    const int iconSize;
    bool firstResize;

    void setActions();
    void setToolBar();
    void setActivitiesDock();
    void setScheduleScene();
    void setConfigFile();
    QString ensureExtension(const QString& filename, const QString& extension);
    void clearData();
    void reloadUI();
    void setColorManager();
    void setScheduleSizeManager();
    void GenerateGridItem();
    bool isDarkTheme();
    void addNewUser(Owner *owner);
    void loadFontAwesome();
    void clearUI();
    void loadConfiguration();
};
#endif // MAINWINDOW_H
