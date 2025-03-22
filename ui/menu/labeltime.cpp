#include "labeltime.h"
#include <QVBoxLayout>
#include <QTimeEdit>
#include <QLabel>

LabelTime::LabelTime(QString label, QTime defaultValue, QWidget *parent): QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout();
    QLabel *labelLbl = new QLabel(label);
    QTimeEdit *editTime = new QTimeEdit(defaultValue);
    connect(editTime, &QTimeEdit::userTimeChanged,this, [this](QTime change){emit changeTime(change);});

    layout->addWidget(labelLbl);
    layout->addWidget(editTime);

    setLayout(layout);
}
