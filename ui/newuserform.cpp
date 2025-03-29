#include "newuserform.h"
#include "ui/ui_newuserform.h"
#include <QColorDialog>
#include <QFileDialog>
#include <QStandardPaths>
#include <QImage>
#include <QErrorMessage>
#include <QPalette>
#include <QPainter>

NewUserForm::NewUserForm(const QList<Owner*> *owners, const int &size, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewUserForm)
    , owners(owners)
    , size(size)
    , transparentImage(QSize(size,size))
    , editMode(false)
{
    ui->setupUi(this);
    ui->imagePreview->setFixedSize(size,size);
    setModal(true);
    createConnections();
    createTransparentImage();
}

void NewUserForm::createConnections()
{
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(close()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(finishEdition()));
    connect(ui->colorBtn, SIGNAL(clicked()), this, SLOT(chooseColor()));
    connect(ui->openImageBtn,SIGNAL(clicked()), this, SLOT(chooseImage()));
}

void NewUserForm::createTransparentImage(){
    transparentImage.fill(Qt::transparent);
}

void NewUserForm::isEdit(Owner *owner) {
    editMode = true;
    editOwner = owner;
    ui->nameTxt->setText(editOwner->getName());
    ui->colorTxt->setText(editOwner->getColor().name());
    if (!editOwner->getImage().isNull()){
        QPixmap pixmapPreview = QPixmap::fromImage(editOwner->getImage());
        pixmapPreview = pixmapPreview.scaled(QSize(size,size), Qt::KeepAspectRatio);
        ui->imagePreview->setPixmap(pixmapPreview);
        image = editOwner->getImage();
    }else {
        ui->imagePreview->setPixmap(transparentImage);
        image = QImage();
    }
}

NewUserForm::~NewUserForm()
{
    delete ui;
}

void NewUserForm::chooseColor() {
    QString colorName = ui->colorTxt->text();
    if (colorName.isEmpty()) colorName = "#FFFFFF";
    QColor color = QColorDialog::getColor(QColor(colorName),this, tr("Pick user color"));
    ui->colorTxt->setText(color.name());
}

void NewUserForm::chooseImage() {
    QString imagePath = QFileDialog::getOpenFileName(this,
                                                     tr("Choose a image file"),
                                                     QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                     tr("Pictures (%1)").arg("*.jpg *.jpeg *.png"));
    if (imagePath.isEmpty()){
        ui->imagePreview->setPixmap(transparentImage);
        image = QImage();
    } else {
        QPixmap pixmapPreview = QPixmap(imagePath);
        pixmapPreview = pixmapPreview.scaled(QSize(size,size), Qt::KeepAspectRatio);
        ui->imagePreview->setPixmap(pixmapPreview);
        image = QImage(pixmapPreview.toImage());
    }
}

void NewUserForm::finishEdition() {
    QString errorMsgTxt = isValidData();
    if (errorMsgTxt.isEmpty()) {
        if (!editMode){
            Owner *newUser = new Owner(ui->nameTxt->text());
            QString colorName = ui->colorTxt->text();
            if (ui->colorTxt->text().isEmpty()) colorName = "#FFFFFF";
            newUser->setColor(QColor(colorName));
            if (!image.isNull())
                newUser->setImage(image);
            emit finishEdit(newUser, false);
        } else {
            editOwner->setName(ui->nameTxt->text());
            QString colorName = ui->colorTxt->text();
            if (ui->colorTxt->text().isEmpty()) colorName = "#FFFFFF";
            editOwner->setColor(QColor(colorName));
            if (!image.isNull())
                editOwner->setImage(image);
            emit finishEdit(editOwner, true);
        }

        close();
    }else {
        QErrorMessage *errorMsg = new QErrorMessage(this);
        errorMsg->showMessage(errorMsgTxt);
    }
}

QString NewUserForm::isValidData() {
    QString errorMsg;

    QString name = ui->nameTxt->text().trimmed();
    QColor color = QColor(ui->colorTxt->text());
    if(name.isEmpty())
        errorMsg = tr("We need a name for this user.");
    else{
        bool alreadyName = false;
        bool alreadyColor = false;
        for(auto owner: std::as_const(*owners)){
            if (!isValidName(owner, name)) alreadyName = true;
            if (!isValidColor(owner, color)) alreadyColor = true;
        }
        if (alreadyName) errorMsg = tr("This user name is already choose, you cannot duplicate the user name.");
        if (alreadyColor) errorMsg += tr("This user color is already choose, you cannot duplicate the user color.");
    }

    return errorMsg;
}

bool NewUserForm::isValidName(Owner *owner, QString name) {
    return editMode && owner == editOwner? true : owner->getName().compare(name) != 0;
}

bool NewUserForm::isValidColor(Owner *owner, QColor color) {
    return editMode && owner == editOwner? true : owner->getColor() != color;
}
