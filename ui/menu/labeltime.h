#ifndef LABELTIME_H
#define LABELTIME_H

#include <QWidget>
#include <QTime>
#include <QString>

class LabelTime : public QWidget
{
    Q_OBJECT
public:
    explicit LabelTime(QString label, QTime defaultValue, QWidget *parent = nullptr);

signals:
    void changeTime(QTime);
};

#endif // LABELTIME_H
