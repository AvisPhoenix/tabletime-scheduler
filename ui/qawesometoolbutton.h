#ifndef QAWESOMETOOLBUTTON_H
#define QAWESOMETOOLBUTTON_H

#include <QObject>
#include <QPushButton>
#include <QFontDatabase>
#include <QFont>

class QAwesomeToolButton : public QPushButton
{
public:
    QAwesomeToolButton(QString icon, int size=32){
        if (QFontDatabase::addApplicationFont(":/fonts/icons/FontAwesome6Free-Regular-400.otf") < 0)
            qWarning() << "FontAwesome cannot be loaded !";
        if (QFontDatabase::addApplicationFont(":/fonts/icons/FontAwesome6Free-Solid-900.otf") < 0)
            qWarning() << "FontAwesome cannot be loaded !";
        QFont font;
        font.setFamily("FontAwesome");
        font.setPixelSize(size);
        setFont(font);
        setText(icon);
    }
};

#endif // QAWESOMETOOLBUTTON_H
