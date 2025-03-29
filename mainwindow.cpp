#include "mainwindow.h"
#include <QToolBar>
#include <QAction>
#include <QFontDatabase>
#include <QFileDialog>
#include <QStandardPaths>
#include <QFileInfo>
#include <QErrorMessage>
#include <QGraphicsView>
#include <QGuiApplication>
#include <QScreen>
#include <QDebug>
#include "timetable/parser/timetablefile.h"
#include "timetable/ui/qscheduleactivity.h"
#include "ui/configurationdlg.h"
#include "ui/importdlg.h"
#include "ui/newuserform.h"
#include "models/colorGenerators/rainbowlcolorsgenerator.h"
#include "ui/svgiconbuilder.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), iconSize(32), scene(nullptr)
{

    setWindowIcon(QIcon(":/icon/icon/org.tabletime-scheduler.svg"));
    setWindowTitle(tr("Tabletime Scheduler"));

    setMinimumSize(640,480);
    showMaximized();

    iconColor = isDarkTheme()? QColor(168,168,168) : QColor(40,40,40);
    setConfigFile();
    setColorManager();
    setActions();
    setToolBar();
    setActivitiesDock();
    setScheduleScene();
    GenerateGridItem();

    changeCurrentUser(userList->getCurrentOwner());
}

MainWindow::~MainWindow() {
    delete colorManager;
    delete scheduleSizeManager;
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    if (isMaximized() && (scene != nullptr)
            && (centralWidget() != nullptr)){
        if ((centralWidget()->width()-16) > scene->width()
                || (centralWidget()->height()-2) > scene->height() ){
            scene->setSceneRect(0, 0, centralWidget()->width()-16, centralWidget()->height()-2);
            scheduleSizeManager->setSpaceSize(scene->width(), scene->height());
        }
    }
    QMainWindow::resizeEvent(event);
}

void MainWindow::importFile() {
    ImportDlg *dialog = new ImportDlg(&activitiesPool, &schedulePool, &ownerList, genColor, fileExtension);
    connect(dialog, &ImportDlg::finishImport, this, &MainWindow::onImportData);
    dialog->show();
}

void MainWindow::saveSchedule() {
    TimeTableFile<QActivity,QScheduleActivity> file(&activitiesPool, &schedulePool, &ownerList);
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Schedule"),
                                                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                    tr("Schedule YAML Files (*.%1)").arg(fileExtension));
    if (!fileName.isEmpty()){
        try{
            file.save(ensureExtension(fileName, fileExtension).toStdString());
        }catch(std::exception &ex){
            QErrorMessage *msgDlg = new QErrorMessage(this);
            msgDlg->showMessage(tr("Error while trying to save the file %1:\n%2").arg(fileName, ex.what()));
        }
    }
}

void MainWindow::clearUI()
{
    scene->clear();
    userList->clearMenu();
    container->clear();
    colorManager->clear();
    GenerateGridItem();
}

void MainWindow::loadConfiguration() {
    if (QFile(configurationFile).exists()){
        ConfigurationParser confFile(configurationFile.toStdString(), theme);
        confFile.load();
    }
}

void MainWindow::clearData()
{
    clearUI();
    activitiesPool.clear();
    schedulePool.clear();
    ownerList.clear();
}

void MainWindow::reloadUI()
{
    colorManager->reload();
    container->reload();
    scene->reload();
    userList->reloadMenus();
}

void MainWindow::loadSchedule() {

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Schedule"),
                                                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                    tr("Schedule YAML Files (*.%1)").arg(fileExtension));
    if (!fileName.isEmpty()) {
        try{
            clearData();
            TimeTableFile<QActivity,QScheduleActivity> file(&activitiesPool, &schedulePool, &ownerList);
            file.load(ensureExtension(fileName, fileExtension).toStdString());
            reloadUI();
        }catch(std::exception &ex){
            QErrorMessage *msgDlg = new QErrorMessage(this);
            msgDlg->showMessage(tr("Error while trying to load the file %1:\n%2").arg(fileName, ex.what()));
        }
    }

}

void MainWindow::newSchedule() {
    clearData();
}

void MainWindow::displayNewUserDialog() {
    NewUserForm *newUserFormDialog = new NewUserForm(&ownerList, iconSize, this);
    connect(newUserFormDialog, &NewUserForm::finishEdit, this, &MainWindow::finishEditUser);
    newUserFormDialog->show();
}

void MainWindow::editUserDialog(Owner *owner) {
    NewUserForm *newUserFormDialog = new NewUserForm(&ownerList, iconSize, this);
    newUserFormDialog->isEdit(owner);
    connect(newUserFormDialog, &NewUserForm::finishEdit, this, &MainWindow::finishEditUser);
    newUserFormDialog->show();
}

void MainWindow::changeCurrentUser(Owner *owner) {
    scene->changeCurrentOwner(owner);
    viewSelectionBtn->udpateCurrentOwner(owner);
}

void MainWindow::addNewUser(Owner *owner)
{
    ownerList.append(owner);
    userList->addLastOwner();
    viewSelectionBtn->addLastOwner();
}

void MainWindow::finishEditUser(Owner *owner, bool isEdit){
    if (!isEdit){
        addNewUser(owner);
    }else {
        userList->clearMenu();
        userList->reloadMenus();
    }
}

void MainWindow::changeViewSelection(QList<Owner*> *selectedView) {
    scene->updateViewSelection(*selectedView);
}

void MainWindow::onImportData(){
    clearUI();
    reloadUI();
}

void MainWindow::setupDialog() {
    ConfigurationDlg *confDlg = new ConfigurationDlg(theme,configurationFile);
    confDlg->show();
}

void MainWindow::setActions() {

    newScheduleAction.setIcon(SVGIconBuilder::build(":/icons/icons/file.svg", iconColor, iconSize));
    newScheduleAction.setStatusTip(tr("New Schedule"));
    newScheduleAction.setToolTip(newScheduleAction.statusTip());
    connect(&newScheduleAction, SIGNAL(triggered()), this, SLOT(newSchedule()));

    loadScheduleAction.setIcon(SVGIconBuilder::build(":/icons/icons/folder-open.svg", iconColor, iconSize));
    loadScheduleAction.setStatusTip(tr("Load Schedule"));
    loadScheduleAction.setToolTip(loadScheduleAction.statusTip());
    connect(&loadScheduleAction, SIGNAL(triggered()), this, SLOT(loadSchedule()));

    saveScheduleAction.setIcon(SVGIconBuilder::build(":/icons/icons/hard-drive.svg", iconColor, iconSize));
    saveScheduleAction.setStatusTip(tr("Save Schedule"));
    saveScheduleAction.setToolTip(saveScheduleAction.statusTip());
    connect(&saveScheduleAction, SIGNAL(triggered()), this, SLOT(saveSchedule()));

    importAction.setIcon(SVGIconBuilder::build(":/icons/icons/file-import.svg", iconColor, iconSize));
    importAction.setStatusTip(tr("Import file data"));
    importAction.setToolTip(importAction.statusTip());
    connect(&importAction, SIGNAL(triggered()), this, SLOT(importFile()));

    configAction.setIcon(SVGIconBuilder::build(":/icons/icons/gear.svg", iconColor, iconSize));
    configAction.setStatusTip(tr("Configuration"));
    configAction.setToolTip(configAction.statusTip());
    connect(&configAction, SIGNAL(triggered()), this, SLOT(setupDialog()));
}

void MainWindow::setToolBar(){

    QToolBar *menu = addToolBar(tr("Menu"));
    menu->addAction(&importAction);
    menu->addSeparator();
    menu->addAction(&newScheduleAction);
    menu->addSeparator();
    menu->addAction(&loadScheduleAction);
    menu->addAction(&saveScheduleAction);

    userList = new OwnerSelectionButton(&ownerList,iconColor, iconSize);
    menu->addWidget(userList);
    connect(userList, SIGNAL(addNewOwner()), this, SLOT(displayNewUserDialog()));
    connect(userList, SIGNAL(changeCurrentOwner(Owner*)), this, SLOT(changeCurrentUser(Owner*)));
    connect(userList, SIGNAL(editOwner(Owner*)), this, SLOT(editUserDialog(Owner*)));

    viewSelectionBtn = new ViewSelectionButton(&ownerList,iconColor, iconSize);
    menu->addWidget(viewSelectionBtn);
    connect(viewSelectionBtn, SIGNAL(changeSelectedOwners(QList<Owner*>*)), this, SLOT(changeViewSelection(QList<Owner*>*)));

    menu->addSeparator();
    menu->addAction(&configAction);
}

void MainWindow::setColorManager() {
    genColor = new RainbowlColorsGenerator();

    colorManager = new ClassificationColorManager(&activitiesPool);
    colorManager->setColorGenerator(genColor);

    theme = new ThemeColors(isDarkTheme(), colorManager);

    loadConfiguration();
}

void MainWindow::GenerateGridItem()
{
    QStringList weekdays({tr("Monday"), tr("Tuesday"), tr("Wednesday"), tr("Thursday"), tr("Friday"), tr("Saturday"), tr("Sunday")});
    weekTableTime = new WeekTableTime(scheduleSizeManager, theme, weekdays);
    scene->addItem(weekTableTime);
}

bool MainWindow::isDarkTheme() {
    QPalette palette = this->palette();
    return palette.color(QPalette::Window).lightness() <= 128;
}

void MainWindow::setScheduleSizeManager(QSize size) {
    scheduleSizeManager = new ScheduleGridSizeManager(QFont(),size.width(), size.height());
}

void MainWindow::setActivitiesDock() {
    activitiesDock = new QDockWidget(tr("Activities"));
    activitiesDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    activitiesDock->setFloating(false);
    activitiesDock->setMaximumWidth(250);
    activitiesDock->setMinimumWidth(250);

    container = new ActivititesManager(&activitiesPool, theme);
    activitiesDock->setWidget(container);

    addDockWidget(Qt::LeftDockWidgetArea, activitiesDock);
}

void MainWindow::setScheduleScene() {
    QGraphicsView* view = new QGraphicsView(this);
    setCentralWidget(view);

    QSize firstSize;
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        firstSize.setWidth(screen->size().width()-267);
        firstSize.setHeight(screen->size().height()-184);
    } else {
        firstSize.setWidth(centralWidget()->width());
        firstSize.setHeight(centralWidget()->height());
    }

    setScheduleSizeManager(firstSize);
    scene = new TableTimeScene(&schedulePool, scheduleSizeManager, theme, this);
    scene->changeCurrentOwner(userList->getCurrentOwner());
    scene->setSceneRect(0, 0, firstSize.width(), firstSize.height());

    view->setScene(scene);
}

void MainWindow::setConfigFile() {
    configurationFile = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + configurationFile;

    QDir configDir = QFileInfo(configurationFile).absoluteDir();
    if (!configDir.exists()) {
        configDir.mkpath(".");
    }
}

QString MainWindow::ensureExtension(const QString &filename, const QString &extension) {
    QFileInfo fileInfo(filename);
    if (fileInfo.suffix().compare(extension, Qt::CaseInsensitive) != 0) {
        return filename + "." + extension;
    }
    return filename;
}
