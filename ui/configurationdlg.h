#ifndef CONFIGURATIONDLG_H
#define CONFIGURATIONDLG_H

#include <QDialog>
#include "../models/themecolors.h"
#include "../parser/configurationparser.h"

namespace Ui {
class ConfigurationDlg;
}

class ConfigurationDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigurationDlg(ThemeColors *theme, QString confFilename, QWidget *parent = nullptr);
    ~ConfigurationDlg();
private slots:
    void colorPicker(QPushButton *button);
    void applyChanges();
private:
    Ui::ConfigurationDlg *ui;
    ThemeColors *theme;
    ConfigurationParser *confFile;

    void loadThemeData();
    void setColorToButton(QPushButton *button, QColor color);
    QIcon buildColorIcon(QColor color, int size);
    void setupColorButtons();
    void setupGradientList();
    void applyThemeChanges();
    void saveConfigFile();
    QGradient getGradientByPreset(int idx);
};

#endif // CONFIGURATIONDLG_H
