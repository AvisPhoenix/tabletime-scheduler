#include <QMetaEnum>
#include "themecolors.h"

ThemeColors::ThemeColors(bool darkTheme, ClassificationColorManager *colorMng):
    isDarkTheme(darkTheme),
    colorMng(colorMng),
    activityPresetName("SharpeyeEagle"){
    setGridTheme();
    setActivityTheme();
    setScheduleActivityTheme();
    setGradientTheme();
}

QColor ThemeColors::getGridColor(QString name) {
    return isDarkTheme? darkGridTheme.value(name, Qt::white) : lightGridTheme.value(name, Qt::black);
}

QColor ThemeColors::getActivityWidgetColor(QString name) {
    return isDarkTheme? darkActivityTheme.value(name, Qt::white) : lightActivityTheme.value(name, Qt::black);
}

QColor ThemeColors::getScheduleActivityColor(QString name) {
    return isDarkTheme? darkScheduleATheme.value(name, Qt::white) : lightScheduleATheme.value(name, Qt::black);
}

int ThemeColors::alphaValue(QString type) {
    return isDarkTheme? darkAlpha.value(type, 70): lightAlpha.value(type, 70);
}

QColor ThemeColors::getGridColor(QString name, bool darkTheme) {
    return darkTheme? darkGridTheme.value(name, Qt::white) : lightGridTheme.value(name, Qt::black);
}

QColor ThemeColors::getActivityWidgetColor(QString name, bool darkTheme) {
    return darkTheme? darkActivityTheme.value(name, Qt::white) : lightActivityTheme.value(name, Qt::black);
}

QColor ThemeColors::getScheduleActivityColor(QString name, bool darkTheme) {
    return darkTheme? darkScheduleATheme.value(name, Qt::white) : lightScheduleATheme.value(name, Qt::black);
}

int ThemeColors::alphaValue(QString type, bool darkTheme) {
    return darkTheme? darkAlpha.value(type, 70): lightAlpha.value(type, 70);
}

void ThemeColors::setGridColor(QString name, bool darkTheme, QColor value) {
    if (darkTheme && darkGridTheme.contains(name)){
        darkGridTheme[name] = value;
    }
    if (!darkTheme && lightGridTheme.contains(name)){
        lightGridTheme[name] = value;
    }
}

void ThemeColors::setActivityWidgetColor(QString name, bool darkTheme, QColor value) {
    if (darkTheme && darkActivityTheme.contains(name)){
        darkActivityTheme[name] = value;
    }
    if (!darkTheme && lightActivityTheme.contains(name)){
        lightActivityTheme[name] = value;
    }
}

void ThemeColors::setScheduleActivityColor(QString name, bool darkTheme, QColor value) {
    if (darkTheme && darkScheduleATheme.contains(name)){
        darkScheduleATheme[name] = value;
    }
    if (!darkTheme && lightScheduleATheme.contains(name)){
        lightScheduleATheme[name] = value;
    }
}

void ThemeColors::setAlphaValue(QString type, bool darkTheme, int value) {
    if (darkTheme && darkAlpha.contains(type)){
        darkAlpha[type] = value;
    }
    if (!darkTheme && lightAlpha.contains(type)){
        lightAlpha[type] = value;
    }
}

void ThemeColors::setGradientActivityPreset(QString name) {
    activityPresetName = name;
    setGradientTheme();
}

QString ThemeColors::getGradientActivityPresetName() {
    return activityPresetName;
}

void ThemeColors::addActivityClassification(QString classification) {
    if (colorMng != nullptr){
        colorMng->addNewClassification(classification);
    }
}

QColor ThemeColors::getActivityColor(QString classification) {
    return colorMng != nullptr? colorMng->getColor(classification) : Qt::white;
}

void ThemeColors::setGridTheme() {
    darkGridTheme["headers"] = Qt::gray;
    darkGridTheme["line"] = Qt::white;
    darkGridTheme["interline"] = QColor(55,55,55);
    darkGridTheme["headerText"] = Qt::white;
    darkGridTheme["hourText"] = Qt::white;
    darkAlpha["grid"] = 70;

    lightGridTheme["headers"] = Qt::gray;
    lightGridTheme["line"] = Qt::black;
    lightGridTheme["interline"] = QColor(200,200,200);
    lightGridTheme["headerText"] = Qt::black;
    lightGridTheme["hourText"] = Qt::black;
    lightAlpha["grid"] = 70;
}

void ThemeColors::setActivityTheme() {
    darkActivityTheme["font"] = Qt::white;
    darkAlpha["activity"] = 178;

    lightActivityTheme["font"] = Qt::black;
    lightAlpha["activity"] = 178;
}

void ThemeColors::setScheduleActivityTheme() {
    darkScheduleATheme["overlap"] = Qt::red;
    darkScheduleATheme["font"] = Qt::white;

    lightScheduleATheme["overlap"] = Qt::red;
    lightScheduleATheme["font"] = Qt::white;
}

void ThemeColors::setGradientTheme() {
    colorMng->setGradientRule(getGradientByPresetName(activityPresetName));
}

QGradient *ThemeColors::getGradientByPresetName(QString name) {
    return new QGradient(getGradientPresetByName(name));
}

QGradient::Preset ThemeColors::getGradientPresetByName(QString name) {
    QMetaEnum presetEnum = QMetaEnum::fromType<QGradient::Preset>();
    for(int i=0; i < presetEnum.keyCount(); i++){
        if (QLatin1String(presetEnum.key(i)).compare(name) == 0) return static_cast<QGradient::Preset>(presetEnum.value(i));
    }
    return static_cast<QGradient::Preset>(presetEnum.value(0));
}
