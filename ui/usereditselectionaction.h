#ifndef USEREDITSELECTIONACTION_H
#define USEREDITSELECTIONACTION_H

#include <QHBoxLayout>
#include <QWidgetAction>
#include "../timetable/models/owner.h"

class UserEditSelectionAction : public QWidgetAction
{
    Q_OBJECT
public:
    explicit UserEditSelectionAction(Owner *owner, int size=32, QColor fontColor= Qt::white, QObject *parent = nullptr);
signals:
    void editOwner(Owner *);
private:
    Owner *owner;
    void addUserBtn(Owner *owner, int size, QColor fontColor, QHBoxLayout *layout);
    void addEditButton(int size, QColor fontColor, QHBoxLayout *layout);
};

#endif // USEREDITSELECTIONACTION_H
