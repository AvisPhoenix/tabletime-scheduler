#include "activititesmanager.h"
#include "ui_activititesmanager.h"
#include <QBrush>


void ActivititesManager::setCompleter()
{
    completer = new QCompleter();
    completer->setModel(&completerModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->classifyTxt->setCompleter(completer);
}

ActivititesManager::ActivititesManager(ActivityPoolManager *activityPoolMng,
                                       ThemeColors *theme,
                                       QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ActivititesManager)
    , activityPool(activityPoolMng)
    , completer(nullptr)
    , theme(theme)
{   
    ui->setupUi(this);
    setCompleter();
    buildClassificationData();

}

ActivititesManager::~ActivititesManager() {
    delete ui;
}

void ActivititesManager::clearPagesItems() {
    for (auto item: std::as_const(classificationPages)){
        while (item->count() > 0){
            QLayoutItem *act = item->takeAt(0);
            act->widget()->setParent(nullptr);
        }
    }
}

void ActivititesManager::clearClassificationPages()
{
    for(int i=1; i < ui->activityTBx->count(); i++){
        ui->activityTBx->removeItem(i);
    }
    classificationPages.clear();
    classificationPages["None"] = ui->unClassifyBox;
}

void ActivititesManager::clear() {
    clearPagesItems();
    clearClassificationPages();
}

void ActivititesManager::reload() {
    buildClassificationData();
}

QActivity* ActivititesManager::createActivity(QString classification, QString name) {
    QActivity *nactivity = new QActivity(name,classification);
    nactivity->setTheme(theme);
    connect(nactivity,SIGNAL(onDelete(QActivity*)), this,SLOT(onRemoveActivity(QActivity*)));

    return nactivity;
}

void ActivititesManager::addActivity(QString name, QString classification) {
    if (classification.isEmpty()) {
        classification = "None";
    }

    if (!classificationPages.contains(classification)){
        addNewClassification(classification);
    }

    QActivity *nactivity = createActivity(classification, name);
    activityPool->addActivity(nactivity);
    classificationPages[classification]->addWidget(nactivity);
}

void ActivititesManager::on_addBtn_clicked() {
    QString name = ui->nameTxt->text().trimmed();
    QString classification = ui->classifyTxt->text().trimmed();
    if (name.isEmpty()){
        ui->errorMsg->setText(tr("Error: Define the activity name."));
    } else {
        addActivity(name, classification);
        clearForm();
    }
    ui->nameTxt->setFocus();
}

void ActivititesManager::onRemoveActivity(QActivity *act){
    classificationPages[act->classification]->removeWidget(act);
    activityPool->removeActivity(act);
    act->setParent(nullptr);
    delete act;
}

void ActivititesManager::addClassificationPage(QString classification) {
    QWidget *pageClassify = new QWidget();
    QVBoxLayout  *layout = new QVBoxLayout();
    pageClassify->setLayout(layout);
    classificationPages[classification] = layout;
    ui->activityTBx->addItem(pageClassify,classification);
}

void ActivititesManager::addClassification2Completer(QString classification)
{
    QStringList currentWords = completerModel.stringList();
    currentWords.append(classification);
    completerModel.setStringList(currentWords);
}

void ActivititesManager::addNewClassification(QString classification) {
    theme->addActivityClassification(classification);
    addClassificationPage(classification);
    addClassification2Completer(classification);
}

QStringList ActivititesManager::getCurrentClassification()
{
    QStringList wordList = {"None"};

    QList<QString> classifications = activityPool->getClassifications();
    if (classifications.length() > 0) {
        wordList = QStringList(classifications);
    }

    return wordList;
}

void ActivititesManager::setCompleter(QStringList wordList) {
    completerModel.setStringList(wordList);
}

void ActivititesManager::defineClassificationPages(QStringList wordList) {
    for (const QString &classification: wordList){
        if (classification.compare("None") != 0)
            addClassificationPage(classification);
    }
    classificationPages["None"] = ui->unClassifyBox;
}

void ActivititesManager::fillActivityPages(QStringList wordList)
{
    for(const QString &classification : std::as_const(wordList)){
        QList<Activity *> items = activityPool->getByClassification(classification);
        for(auto *act: std::as_const(items)){
            QActivity* actWidget = static_cast<QActivity*>(act);
            actWidget->setTheme(theme);
            classificationPages[classification]->addWidget(actWidget);
            connect(actWidget,SIGNAL(onDelete(QActivity*)), this,SLOT(onRemoveActivity(QActivity*)));
        }
    }
}

void ActivititesManager::buildClassificationData() {
    QStringList wordList = getCurrentClassification();

    setCompleter(wordList);
    defineClassificationPages(wordList);
    fillActivityPages(wordList);
}

void ActivititesManager::clearForm() {
    ui->nameTxt->setText("");
    ui->classifyTxt->setText("");
    ui->errorMsg->setText("");
}

