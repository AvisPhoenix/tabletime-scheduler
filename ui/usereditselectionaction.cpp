#include "usereditselectionaction.h"
#include <QWidget>
#include <QPushButton>
#include <QIcon>
#include "svgiconbuilder.h"
#include "ownericonbuilder.h"


UserEditSelectionAction::UserEditSelectionAction(Owner *owner,
                                                 int size,
                                                 QColor fontColor,
                                                 QObject *parent):
    owner(owner), QWidgetAction(parent){
    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    widget->setLayout(layout);

    addUserBtn(owner, size, fontColor, layout);

    addEditButton(size, fontColor, layout);

    setDefaultWidget(widget);
}

void UserEditSelectionAction::addUserBtn(Owner *owner, int size, QColor fontColor, QHBoxLayout *layout)
{
    QIcon myIcon = OwnerIconBuilder::getIcon(owner,size,fontColor);
    QPushButton *userBtn = new QPushButton(myIcon,
                                           owner->getName());
    userBtn->setFlat(true);
    userBtn->setFixedHeight(size);
    userBtn->setIconSize(QSize(size/2,size/2));
    userBtn->setStyleSheet("QPushButton { text-align: left; } QPushButton::menu-indicator { subcontrol-position: left center; }");
    connect(userBtn,&QPushButton::clicked,this,[this](){ emit triggered(); });
    layout->addWidget(userBtn,1);

    setIcon(myIcon);
    setText(owner->getName());
}

void UserEditSelectionAction::addEditButton(int size, QColor fontColor, QHBoxLayout *layout)
{
    QPushButton *editBtn = new QPushButton(SVGIconBuilder::build(":/icons/icons/pen-to-square.svg",fontColor, size),
                                           "");
    editBtn->setFlat(true);
    editBtn->setFixedSize(size/2,size/2);
    editBtn->setIconSize(QSize(size/2,size/2));
    connect(editBtn,&QPushButton::clicked,this,[this](){emit editOwner(this->owner); });
    layout->addWidget(editBtn);
}
