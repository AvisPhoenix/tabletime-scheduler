#include "ownerselectionbutton.h"
#include <QMenu>
#include <QAction>
#include <QPalette>

#include "fontawesomeiconbuilder.h"
#include "ownericonbuilder.h"
#include "usereditselectionaction.h"

void OwnerSelectionButton::addNewActionBuild()
{
    addNewText = tr("Add new user");
    addNew.setIcon(FontAwesomeIconBuilder("\u002b",size,defaultFontColor).build());
    addNew.setText(addNewText);
}

OwnerSelectionButton::OwnerSelectionButton(const QList<Owner*> *owners, const int &size): owners(owners), anonymous("Anonymous"), size(size) {
    anonymous.setColor(Qt::transparent);
    setFixedSize(QSize(size+10,size));
    QPalette palette = this->palette();
    defaultFontColor = palette.color(QPalette::ButtonText);
    addNewActionBuild();
    clearMenu();
    reloadMenus();
    setMenu(&menu);
    setPopupMode(QToolButton::MenuButtonPopup);
    connect(&menu, SIGNAL(triggered(QAction*)), this, SLOT(selectOwner(QAction*)));
}

void OwnerSelectionButton::clearMenu(){
    menu.clear();
    menu.addAction(&addNew);
    setIcon(OwnerIconBuilder(&anonymous, size, defaultFontColor).getIcon());
    setToolTip(anonymous.getName());
}

void OwnerSelectionButton::reloadMenus() {
    for(auto owner: std::as_const(*owners)){
        UserEditSelectionAction *action = new UserEditSelectionAction(owner, size, defaultFontColor);
        connect(action, &UserEditSelectionAction::editOwner, this, &OwnerSelectionButton::onEditOwner);
        menu.addAction(action);
    }
    autoSelectUser();
}

void OwnerSelectionButton::autoSelectUser() {
    if(owners->size() == 1){
        selectOwner(menu.actions().last());
    }
}

void OwnerSelectionButton::addLastOwner() {
    if (owners->size() > 0){
        UserEditSelectionAction *action = new UserEditSelectionAction(owners->last(), size, defaultFontColor);
        connect(action, &UserEditSelectionAction::editOwner, this, &OwnerSelectionButton::onEditOwner);
        menu.addAction(action);
        autoSelectUser();
    }
}

Owner *OwnerSelectionButton::getCurrentOwner() {
    if (owners->size() > 0){
        Owner *selected = nullptr;
        for(auto owner: std::as_const(*owners)){
            if (owner->getName().compare(toolTip()) == 0) selected = owner;
        }
        if (selected != nullptr) return selected;
        else return &anonymous;
    }else {
        return &anonymous;
    }
}

void OwnerSelectionButton::selectOwner(QAction *action) {
    if (action->text().compare(addNewText) == 0){
        selectNewOwner();
    }else {
        setIcon(action->icon());
        setToolTip(action->text());
        Owner *selected = nullptr;
        for(auto owner: std::as_const(*owners)){
            if (owner->getName().compare(action->text()) == 0) selected = owner;
        }
        if (selected != nullptr)
            emit changeCurrentOwner(selected);
    }
}

void OwnerSelectionButton::onEditOwner(Owner *owner) {
    emit editOwner(owner);
}

void OwnerSelectionButton::selectNewOwner() {
    emit addNewOwner();
}
