#ifndef NEWUSERFORM_H
#define NEWUSERFORM_H

#include <QDialog>
#include <QImage>
#include <QPixmap>

#include "../timetable/models/owner.h"

namespace Ui {
class NewUserForm;
}

class NewUserForm : public QDialog
{
    Q_OBJECT

public:
    explicit NewUserForm(const QList<Owner*> *owners,const int &size=32, QWidget *parent = nullptr);
    void isEdit(Owner *owner);
    ~NewUserForm();
signals:
    void finishEdit(Owner*, bool isEdit);
private slots:
    void chooseColor();
    void chooseImage();
    void finishEdition();
private:
    bool editMode;
    Owner *editOwner;
    Ui::NewUserForm *ui;
    QImage image;
    QPixmap transparentImage;
    const int size;
    const QList<Owner*> *owners;

    void createConnections();
    void createTransparentImage();
    QString isValidData();
    bool isValidName(Owner *owner, QString name);
    bool isValidColor(Owner *owner, QColor color);
};

#endif // NEWUSERFORM_H
