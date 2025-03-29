#include "importdlg.h"
#include "ui_importdlg.h"

#include <QCheckBox>
#include <QPushButton>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include "../models/importoptions.h"

ImportDlg::ImportDlg(ActivityPoolManager *activityPool, SchedulePoolManager *schedule, QList<Owner *> *owners, ColorGenerator *colorGenerator, QString fileExtension, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ImportDlg)
    , fileExtension(fileExtension)
    , lockClose(false)
{
    ui->setupUi(this);
    setModal(true);

    ui->progressBar->setHidden(true);
    ui->errorsGrp->setHidden(true);
    setFixedHeight(300);

    connect(ui->mergeCbx, &QCheckBox::checkStateChanged, this, &ImportDlg::mergeMode);
    connect(ui->importBtn, &QPushButton::clicked, this, &ImportDlg::startImport);
    connect(ui->fileBtn, &QPushButton::clicked, this, &ImportDlg::openFilePath);
    connect(ui->fileTxt, &QLineEdit::textChanged, this, &ImportDlg::changeFilePath);

    /* I created this process to avoid freeze the UI
     * but since the process build QWidget and QWidget could not be created
     * inside other QThread than the main thread I remove this until I solved this
     *
     * importProcess = new ImportProcess(activityPool, schedule, owners, colorGenerator);
     * connect(importProcess, &ImportProcess::finished, this, &ImportDlg::endImport);
     * connect(importProcess, &ImportProcess::processMessages, this, &ImportDlg::showProcessMessages);*/
    fileImporter = new TimeTableFile<QActivity, QScheduleActivity>(activityPool, schedule, owners, colorGenerator);

}

ImportDlg::~ImportDlg() {
    //delete importProcess;
    delete ui;
}

void ImportDlg::closeEvent(QCloseEvent *event) {
    if (lockClose) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("You cannot closed this dialog until the importation finish"));
        event->ignore();
    } else {
        event->accept();
    }
}

void ImportDlg::mergeMode(Qt::CheckState state) {
    ui->optionsGrp->setHidden(state == Qt::Checked);
}

void ImportDlg::startImport() {
    if (!ui->activitiesCbx->isChecked() && !ui->usersCbx->isChecked() && !ui->mergeCbx->isChecked()){
        QMessageBox::critical(this,
                              tr("No data to import"),
                              tr("You need to choose data to import, activities, users or merge."));
    }else {
        startImportProcess();
    }
}

void ImportDlg::openFilePath() {
    QString filepath = QFileDialog::getOpenFileName(this, tr("Pick file to import"),
                                                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                    tr("Schedule YAML Files (*.%1)").arg(fileExtension));
    if (!filepath.isEmpty()){
        ui->fileTxt->setText(filepath);
    }
}

void ImportDlg::changeFilePath(QString filepath){
    ui->importBtn->setEnabled(!filepath.trimmed().isEmpty());
}

void ImportDlg::showProcessMessages(QList<QString> processMessages) {
    QStringList finalProcessMessages;
    for(const QString &message: std::as_const(processMessages)){
        QStringList parts = message.split("_");
        if (parts[1].compare("OC")==0){
            QString msg = tr("Warning: The owner color %1 is duplicated.").arg(parts[2]);
            finalProcessMessages.push_back(msg);
        }
        if (parts[1].compare("SD") == 0){
            QString msg = tr("Warning: Found a duplicated scheduled activity: { Name: %1, Classification: %2, Owner: %3, time: { start: %4, end: %5, dayOfWeek: %6 }.").arg(parts[2], parts[3], parts[4], parts[6], parts[7], parts[5]);
            finalProcessMessages.push_back(msg);
        }
    }
    finalProcessMessages.push_back(tr("Done"));
    ui->errorListTxt->setPlainText(finalProcessMessages.join("\n"));
}

void ImportDlg::prepareImport()
{
    setFixedHeight(470);
    ui->progressBar->setHidden(false);
    ui->errorListTxt->setPlainText("");
    ui->errorsGrp->setHidden(false);
    ui->importBtn->setDisabled(true);
    ui->buttonBox->setDisabled(true);
    lockClose = true;
}

ImportOptions ImportDlg::getOptions() {
    ImportOptions options = ImportOptions::None;
    if (ui->mergeCbx->isChecked()){
        options = ImportOptions::All;
    } else if (ui->activitiesCbx->isChecked() && ui->usersCbx->isChecked()){
        options = ImportOptions::ActivitesUsers;
    } else if (ui->activitiesCbx->isChecked()) {
        options = ImportOptions::ActivitesOnly;
    } else {
        options = ImportOptions::UsersOnly;
    }

    return options;
}

ImportPriority ImportDlg::getPriority() {
    ImportPriority priority = ImportPriority::CurrentFile;
    if (ui->importedRBtn->isChecked()){
        priority = ImportPriority::ImportedFile;
    }

    return priority;
}

void ImportDlg::startImportProcess() {
    prepareImport();
    QString file = ui->fileTxt->text();
    ImportOptions options = getOptions();
    ImportPriority priority = getPriority();
    //importProcess->setArguments(file, options, priority);
    //importProcess->start();
    QList<QString> messages = fileImporter->import(file.toStdString(),options, priority);
    showProcessMessages(messages);
    endImport();
}

void ImportDlg::endImport() {
    ui->progressBar->setHidden(true);
    ui->importBtn->setDisabled(false);
    ui->buttonBox->setDisabled(false);
    lockClose = false;

    emit finishImport();
}
