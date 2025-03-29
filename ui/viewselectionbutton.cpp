#include "viewselectionbutton.h"
#include "svgiconbuilder.h"
#include <QWidgetAction>
#include <QCheckBox>


ViewSelectionButton::ViewSelectionButton(const QList<Owner *> *owners, const QColor &color, const int &size):
    owners(owners),
    size(size),
    defaultIconColor(color){
    setFixedSize(QSize(size+10,size));
    setIconSize(QSize(size,size));
    setToolTip(tr("View selection"));
    clearMenu();
    reloadMenus();
    setMenu(&menu);
    setPopupMode(QToolButton::MenuButtonPopup);
    setIcon(SVGIconBuilder::build(":/icons/icons/eye-low-vision.svg", defaultIconColor, size));
}

QWidgetAction* ViewSelectionButton::buildMenuItem(Owner *owner) {
    QWidget *zone = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout();
    zone->setLayout(layout);
    layout->setContentsMargins(5,5,5,5);
    QCheckBox *ownerCB = new QCheckBox(owner->getName());
    connect(ownerCB, &QCheckBox::checkStateChanged, this, [this, owner](Qt::CheckState state){changeSelection(state, owner);});
    layout->addWidget(ownerCB);

    QWidgetAction *selectionAct = new QWidgetAction(&menu);
    selectionAct->setDefaultWidget(zone);
    return selectionAct;
}

void ViewSelectionButton::clearMenu() {
    menu.clear();
}

void ViewSelectionButton::reloadMenus() {
    for(auto owner: std::as_const(*owners)){
        if (owner->getName() != currentOwner->getName()){
            menu.addAction(buildMenuItem(owner));
        }
    }
}

void ViewSelectionButton::addLastOwner() {
    if (owners->size() > 0){
        Owner *owner = owners->last();
        if (owner->getName() != currentOwner->getName()){
            menu.addAction(buildMenuItem(owner));
        }
    }
}

void ViewSelectionButton::udpateCurrentOwner(Owner *owner) {
    currentOwner = owner;
    clearMenu();
    reloadMenus();
}

void ViewSelectionButton::changeSelection(Qt::CheckState state, Owner *owner) {
    if (state == Qt::Checked){
        if (selected.lastIndexOf(owner) < 0)
            selected.append(owner);
    }else {
        if (selected.lastIndexOf(owner) >= 0)
            selected.removeAll(owner);
    }
    emit changeSelectedOwners(&selected);
}
