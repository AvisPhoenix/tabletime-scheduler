#ifndef THEMECOLORS_H
#define THEMECOLORS_H

#include <QString>
#include <QColor>
#include <QMap>
#include <QGradient>
#include "classificationcolormanager.h"

class ThemeColors
{
public:
    ThemeColors(bool darkTheme, ClassificationColorManager *colorMng);
    void setSystemTheme(bool darkTheme){isDarkTheme = darkTheme;}
    QColor getGridColor(QString name);
    QColor getActivityWidgetColor(QString name);
    QColor getScheduleActivityColor(QString name);
    int alphaValue(QString type);
    QColor getGridColor(QString name, bool darkTheme);
    QColor getActivityWidgetColor(QString name, bool darkTheme);
    QColor getScheduleActivityColor(QString name, bool darkTheme);
    int alphaValue(QString type, bool darkTheme);
    void setGridColor(QString name, bool darkTheme, QColor value);
    void setActivityWidgetColor(QString name, bool darkTheme, QColor value);
    void setScheduleActivityColor(QString name, bool darkTheme, QColor value);
    void setAlphaValue(QString type, bool darkTheme, int value);

    void setGradientActivityPreset(QString name);
    QString getGradientActivityPresetName();

    void addActivityClassification(QString classification);
    QColor getActivityColor(QString classification);

    friend class ConfigurationParser;
private:
    bool isDarkTheme;
    QMap<QString, QColor> lightGridTheme, darkGridTheme;
    QMap<QString, QColor> lightActivityTheme, darkActivityTheme;
    QMap<QString, QColor> lightScheduleATheme, darkScheduleATheme;
    QMap<QString, int> lightAlpha, darkAlpha;

    ClassificationColorManager *colorMng;
    QString activityPresetName;

    void setGridTheme();
    void setActivityTheme();
    void setScheduleActivityTheme();
    void setGradientTheme();

    QGradient *getGradientByPresetName(QString name);
    QGradient::Preset getGradientPresetByName(QString name);
};

#endif // THEMECOLORS_H
