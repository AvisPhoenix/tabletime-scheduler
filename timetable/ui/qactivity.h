#ifndef QACTIVITY_H
#define QACTIVITY_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include "../models/activity.h"
#include "../../ui/qawesometoolbutton.h"
#include "../../models/themecolors.h"

class QActivity : public QWidget, public Activity
{
    Q_OBJECT
public:
    explicit QActivity(QWidget *parent = nullptr);
    QActivity(QString name, QString classification, QWidget *parent = nullptr);
    void setColor(QColor color, int alphaFill=178);
    void setTheme(ThemeColors *theme){this->theme = theme;}
    QColor fontColor;
    QColor deleteColor;
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
protected slots:
    void pressDelete();
signals:
    void onDelete(QActivity *act);
private:
    QAwesomeToolButton removeBtn;
    int aFill;
    QColor color;
    ThemeColors *theme;

    void defineRemoveButton();
    void defaultConstructor();
};

#endif // QACTIVITY_H
