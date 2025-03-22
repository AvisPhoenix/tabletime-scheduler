#include "configurationdlg.h"
#include "ui_configurationdlg.h"
#include <QColorDialog>
#include <QPixmap>
#include <QPainter>
#include <QMetaEnum>
#include <QGradient>


ConfigurationDlg::ConfigurationDlg(ThemeColors *theme, QString confFilename, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConfigurationDlg)
{
    ui->setupUi(this);
    this->theme = theme;

    setupGradientList();
    loadThemeData();
    setupColorButtons();

    confFile = new ConfigurationParser(confFilename.toStdString(),theme);

    connect(ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &ConfigurationDlg::applyChanges);
}

ConfigurationDlg::~ConfigurationDlg() {
    delete ui;
}


void ConfigurationDlg::setupColorButtons() {
    connect(ui->fontActivityCLBtn, &QPushButton::clicked, this, [this](){ this->colorPicker(this->ui->fontActivityCLBtn); } );
    connect(ui->fontActivityCDBtn, &QPushButton::clicked, this, [this](){ this->colorPicker(this->ui->fontActivityCDBtn); } );
    connect(ui->overloadActLBtn, &QPushButton::clicked, this, [this](){ this->colorPicker(this->ui->overloadActLBtn); } );
    connect(ui->overloadActDBtn, &QPushButton::clicked, this, [this](){ this->colorPicker(this->ui->overloadActDBtn); } );
    connect(ui->headerTextCLBtn, &QPushButton::clicked, this, [this](){ this->colorPicker(this->ui->headerTextCLBtn); } );
    connect(ui->headerColorDBtn, &QPushButton::clicked, this, [this](){ this->colorPicker(this->ui->headerColorDBtn); } );
    connect(ui->gridLineCLBtn, &QPushButton::clicked, this, [this](){ this->colorPicker(this->ui->gridLineCLBtn); } );
    connect(ui->hourTextLBtn, &QPushButton::clicked, this, [this](){ this->colorPicker(this->ui->hourTextLBtn); } );
    connect(ui->gridInterCLBtn, &QPushButton::clicked, this, [this](){ this->colorPicker(this->ui->gridInterCLBtn); } );
    connect(ui->headerTextCDBtn, &QPushButton::clicked, this, [this](){ this->colorPicker(this->ui->headerTextCDBtn); } );
    connect(ui->gridLineCDBtn, &QPushButton::clicked, this, [this](){ this->colorPicker(this->ui->gridLineCDBtn); } );
    connect(ui->gridInterCDBtn, &QPushButton::clicked, this, [this](){ this->colorPicker(this->ui->gridInterCDBtn); } );
    connect(ui->hourTextDBtn, &QPushButton::clicked, this, [this](){ this->colorPicker(this->ui->hourTextDBtn); } );
    connect(ui->headerColorLBtn, &QPushButton::clicked, this, [this](){ this->colorPicker(this->ui->headerColorLBtn); } );
}

void ConfigurationDlg::setupGradientList() {
    QMetaEnum presetEnum = QMetaEnum::fromType<QGradient::Preset>();
    for(int i=0; i < presetEnum.keyCount(); i++){
        ui->gradientListCbx->addItem(QLatin1String(presetEnum.key(i)));
    }
}

void ConfigurationDlg::applyThemeChanges(){
    theme->setAlphaValue("activity", true, ui->alphaActDSlider->value());
    theme->setAlphaValue("activity", false, ui->alphaActLSlider->value());
    theme->setAlphaValue("grid", true, ui->headerAlphaD->value());
    theme->setAlphaValue("grid", false, ui->headerAlphaL->value());

    theme->setActivityWidgetColor("font", true, QColor(ui->fontActivityCDBtn->toolTip()));
    theme->setActivityWidgetColor("font", false, QColor(ui->fontActivityCLBtn->toolTip()));

    theme->setScheduleActivityColor("font", true, QColor(ui->fontActivityCDBtn->toolTip()));
    theme->setScheduleActivityColor("font", false, QColor(ui->fontActivityCLBtn->toolTip()));
    theme->setScheduleActivityColor("overlap", true, QColor(ui->overloadActDBtn->toolTip()));
    theme->setScheduleActivityColor("overlap", false, QColor(ui->overloadActLBtn->toolTip()));

    theme->setGridColor("headerText", false, QColor(ui->headerTextCLBtn->toolTip()) );
    theme->setGridColor("headers", true, QColor(ui->headerColorDBtn->toolTip()) );
    theme->setGridColor("line", false, QColor(ui->gridLineCLBtn->toolTip()) );
    theme->setGridColor("hourText", false, QColor(ui->hourTextLBtn->toolTip()) );
    theme->setGridColor("interline", false, QColor(ui->gridInterCLBtn->toolTip()) );
    theme->setGridColor("headerText", true, QColor(ui->headerTextCDBtn->toolTip()) );
    theme->setGridColor("line", true, QColor(ui->gridLineCDBtn->toolTip()) );
    theme->setGridColor("interline", true, QColor(ui->gridInterCDBtn->toolTip()) );
    theme->setGridColor("hourText", true, QColor(ui->hourTextDBtn->toolTip()) );
    theme->setGridColor("headers", false, QColor(ui->headerColorLBtn->toolTip()) );

    theme->setGradientActivityPreset(ui->gradientListCbx->currentText());
}

void ConfigurationDlg::saveConfigFile() {
    confFile->save();
}

QGradient ConfigurationDlg::getGradientByPreset(int idx) {
    QMetaEnum presetEnum = QMetaEnum::fromType<QGradient::Preset>();
    QGradient::Preset preset = static_cast<QGradient::Preset>(presetEnum.value(idx));
    QGradient gradient(preset);
    return gradient;
}

void ConfigurationDlg::colorPicker(QPushButton *button){
    QColor color = QColorDialog::getColor(QColor(button->toolTip()),this, tr("Pick user color"));
    if (color.isValid()) {
        setColorToButton(button,color);
    }
}

void ConfigurationDlg::applyChanges() {
    applyThemeChanges();
    saveConfigFile();
    close();
}

void ConfigurationDlg::loadThemeData() {
    setColorToButton(ui->fontActivityCDBtn, theme->getScheduleActivityColor("font", true));
    setColorToButton(ui->fontActivityCLBtn, theme->getScheduleActivityColor("font", false));
    setColorToButton(ui->overloadActDBtn, theme->getScheduleActivityColor("overlap", true));
    setColorToButton(ui->overloadActLBtn, theme->getScheduleActivityColor("overlap", false));
    setColorToButton(ui->headerTextCLBtn, theme->getGridColor("headerText", false));
    setColorToButton(ui->headerColorDBtn, theme->getGridColor("headers", true));
    setColorToButton(ui->gridLineCLBtn, theme->getGridColor("line", false));
    setColorToButton(ui->hourTextLBtn, theme->getGridColor("hourText", false));
    setColorToButton(ui->gridInterCLBtn, theme->getGridColor("interline", false));
    setColorToButton(ui->headerTextCDBtn, theme->getGridColor("headerText", true));
    setColorToButton(ui->gridLineCDBtn, theme->getGridColor("line", true));
    setColorToButton(ui->gridInterCDBtn, theme->getGridColor("interline", true));
    setColorToButton(ui->hourTextDBtn, theme->getGridColor("hourText", true));
    setColorToButton(ui->headerColorLBtn, theme->getGridColor("headers", false));

    ui->alphaActDSlider->setValue(theme->alphaValue("activity", true));
    ui->alphaActLSlider->setValue(theme->alphaValue("activity", false));
    ui->headerAlphaD->setValue(theme->alphaValue("grid", true));
    ui->headerAlphaL->setValue(theme->alphaValue("grid", false));

    ui->gradientListCbx->setCurrentText(theme->getGradientActivityPresetName());
}

void ConfigurationDlg::setColorToButton(QPushButton *button, QColor color) {
    button->setIcon(buildColorIcon(color, 32));
    button->setToolTip(color.name());
}

QIcon ConfigurationDlg::buildColorIcon(QColor color, int size) {
    QPixmap pixmap(QSize(size,size));
    pixmap.fill(color);
    return QIcon(pixmap);
}
