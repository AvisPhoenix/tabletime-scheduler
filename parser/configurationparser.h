#ifndef CONFIGURATIONPARSER_H
#define CONFIGURATIONPARSER_H

#include <yaml-cpp/yaml.h>
#include <fstream>
#include <QColor>
#include "../models/themecolors.h"

using std::string;
using std::ofstream;

class ConfigurationParser {
public:
    ConfigurationParser(string filename, ThemeColors *theme);
    void save();
    void load();
private:
    ThemeColors *theme;
    string filename;
    YAML::Node buildYAMLStructure();
    QColor getColor(YAML::Node node, QColor defaultC);
    int getInt(YAML::Node node, int defaultI);
    QString getString(YAML::Node node, QString defaultS);
};


#endif // CONFIGURATIONPARSER_H
