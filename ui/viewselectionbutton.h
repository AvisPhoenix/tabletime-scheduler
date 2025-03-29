#ifndef VIEWSELECTIONBUTTON_H
#define VIEWSELECTIONBUTTON_H

#include <QToolButton>
#include <QList>
#include <QMenu>
#include <QVBoxLayout>
#include <QWidgetAction>
#include "../timetable/models/owner.h"

class ViewSelectionButton : public QToolButton
{
    Q_OBJECT
public:
    ViewSelectionButton(const QList<Owner*> *owners, const QColor &color, const int &size);
    void clearMenu();
    void reloadMenus();
    void addLastOwner();
signals:
    void changeSelectedOwners(QList<Owner*>*);
public slots:
    void udpateCurrentOwner(Owner *);
private slots:
    void changeSelection(Qt::CheckState, Owner*);
private:
    const QList<Owner *> *owners;
    Owner *currentOwner;
    QList<Owner *> selected;
    QMenu menu;
    const int size;
    QColor defaultIconColor;
    void buildMenu();
    QWidgetAction* buildMenuItem(Owner *owner);
};

#endif // VIEWSELECTIONBUTTON_H
