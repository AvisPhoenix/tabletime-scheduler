#ifndef ACTIVITITESMANAGER_H
#define ACTIVITITESMANAGER_H

#include <QWidget>
#include <QCompleter>
#include <QLabel>
#include <QMap>
#include <QGradient>
#include <QPixmap>
#include <QPainter>
#include <QVBoxLayout>
#include <QStringListModel>

#include "../timetable/models/activitypoolmanager.h"
#include "../timetable/ui/qactivity.h"
#include "../models/themecolors.h"

namespace Ui {
class ActivititesManager;
}

class ActivititesManager : public QWidget
{
    Q_OBJECT

public:
    explicit ActivititesManager(ActivityPoolManager *activityPoolMng, ThemeColors *theme, QWidget *parent = nullptr);
    ~ActivititesManager();
    void clear();
    void reload();
private slots:
    void on_addBtn_clicked();
    void onRemoveActivity(QActivity *act);
private:
    QMap<QString, QVBoxLayout *> classificationPages;

    Ui::ActivititesManager *ui;
    ActivityPoolManager *activityPool;
    QCompleter *completer;
    QStringListModel completerModel;
    ThemeColors *theme;

    void addNewClassification(QString classification);
    QStringList getCurrentClassification();
    QActivity * createActivity(QString classification, QString name);
    void setCompleter(QStringList wordList);
    void clearForm();
    void addActivity(QString name, QString classification);
    QStringList getClassification();
    void buildClassificationData();
    void addClassificationPage(QString classification);
    void defineClassificationPages(QStringList wordList);
    void clearPagesItems();
    void clearClassificationPages();
    void setCompleter();
    void addClassification2Completer(QString classification);
    void fillActivityPages(QStringList wordList);
};

#endif // ACTIVITITESMANAGER_H
