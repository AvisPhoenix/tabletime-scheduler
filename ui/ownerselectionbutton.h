#ifndef OWNERSELECTIONBUTTON_H
#define OWNERSELECTIONBUTTON_H

#include <QToolButton>
#include <QList>
#include <QMenu>
#include "../timetable/models/owner.h"


class OwnerSelectionButton : public QToolButton
{
    Q_OBJECT
public:
    OwnerSelectionButton(const QList<Owner*> *owners, const int &size);
    void clearMenu();
    void reloadMenus();
    void addLastOwner();
    Owner* getCurrentOwner();
signals:
    void changeCurrentOwner(Owner*);
    void addNewOwner();
    void editOwner(Owner *);
private slots:
    void selectOwner(QAction*);
    void onEditOwner(Owner*);
private:
    void selectNewOwner();
    const QList<Owner *> *owners;
    QMenu menu;
    QAction addNew;
    void addNewActionBuild();
    Owner anonymous;
    QString addNewText;
    const int size;
    QColor defaultFontColor;
    void autoSelectUser();
};

#endif // OWNERSELECTIONBUTTON_H
