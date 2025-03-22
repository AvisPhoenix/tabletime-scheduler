#include "configurationparser.h"

ConfigurationParser::ConfigurationParser(string filename, ThemeColors *theme){
    this->filename = filename;
    this->theme = theme;
}

void ConfigurationParser::save() {
    ofstream savedFile(filename);
    if (!savedFile) {
        throw std::runtime_error("Unable to open file for writing: " + filename);
    }
    savedFile << buildYAMLStructure();
    savedFile.close();

}

void ConfigurationParser::load() {
    YAML::Node root = YAML::LoadFile(filename);

    theme->setGradientActivityPreset(getString(root["activitiesGradient"], "SharpeyeEagle"));

    if (root["dark"].IsDefined()){
        if (root["dark"]["grid"].IsDefined()){
            theme->darkGridTheme["headers"] = getColor(root["dark"]["grid"]["headers"], Qt::gray);
            theme->darkGridTheme["line"] = getColor(root["dark"]["grid"]["line"], Qt::white);
            theme->darkGridTheme["interline"] = getColor(root["dark"]["grid"]["interline"], QColor(55,55,55));
            theme->darkGridTheme["headerText"] = getColor(root["dark"]["grid"]["headerText"], Qt::white);
            theme->darkGridTheme["hourText"] = getColor(root["dark"]["grid"]["hourText"], Qt::white);
            theme->darkAlpha["grid"] = getInt(root["dark"]["grid"]["alpha"], 70);
        }
        if (root["dark"]["activity"].IsDefined()){
            theme->darkActivityTheme["font"] = getColor(root["dark"]["activity"]["font"], Qt::white);
            theme->darkAlpha["activity"] = getInt(root["dark"]["activity"]["alpha"], 178);
        }
        if (root["dark"]["scheduleActivity"].IsDefined()){
            theme->darkScheduleATheme["overlap"] = getColor(root["dark"]["scheduleActivity"]["overlap"], Qt::red);
            theme->darkScheduleATheme["font"] = getColor(root["dark"]["scheduleActivity"]["font"], Qt::white);
        }
    }

    if (root["light"].IsDefined()){
        if (root["light"]["grid"].IsDefined()){
            theme->lightGridTheme["headers"] = getColor(root["light"]["grid"]["headers"], Qt::gray);
            theme->lightGridTheme["line"] = getColor(root["light"]["grid"]["line"], Qt::black);
            theme->lightGridTheme["interline"] = getColor(root["light"]["grid"]["interline"], QColor(200,200,200));
            theme->lightGridTheme["headerText"] = getColor(root["light"]["grid"]["headerText"], Qt::black);
            theme->lightGridTheme["hourText"] = getColor(root["light"]["grid"]["hourText"], Qt::black);
            theme->lightAlpha["grid"] = getInt(root["light"]["grid"]["alpha"], 70);
        }
        if (root["light"]["activity"].IsDefined()){
            theme->lightActivityTheme["font"] = getColor(root["light"]["activity"]["font"], Qt::black);
            theme->lightAlpha["activity"] = getInt(root["light"]["activity"]["alpha"], 178);
        }
        if (root["light"]["scheduleActivity"].IsDefined()){
            theme->lightScheduleATheme["overlap"] = getColor(root["light"]["scheduleActivity"]["overlap"], Qt::red);
            theme->lightScheduleATheme["font"] = getColor(root["light"]["scheduleActivity"]["font"], Qt::white);
        }
    }
}

YAML::Node ConfigurationParser::buildYAMLStructure() {
    YAML::Node settings;

    YAML::Node light;
    YAML::Node gridL;
    gridL["headers"] = theme->lightGridTheme["headers"].name().toStdString();
    gridL["line"] = theme->lightGridTheme["line"].name().toStdString();
    gridL["interline"] = theme->lightGridTheme["interline"].name().toStdString();
    gridL["headerText"] = theme->lightGridTheme["headerText"].name().toStdString();
    gridL["hourText"] = theme->lightGridTheme["hourText"].name().toStdString();
    gridL["alpha"] = theme->lightAlpha["grid"];
    light["grid"] = gridL;

    YAML::Node activityL;
    activityL["font"] = theme->lightActivityTheme["font"].name().toStdString();
    activityL["alpha"] = theme->lightAlpha["activity"];
    light["activity"] = activityL;

    YAML::Node schActL;
    schActL["overlap"] = theme->lightScheduleATheme["overlap"].name().toStdString();
    schActL["font"] = theme->lightScheduleATheme["font"].name().toStdString();
    light["scheduleActivity"] = schActL;

    YAML::Node dark;
    YAML::Node gridD;
    gridD["headers"] = theme->darkGridTheme["headers"].name().toStdString();
    gridD["line"] = theme->darkGridTheme["line"].name().toStdString();
    gridD["interline"] = theme->darkGridTheme["interline"].name().toStdString();
    gridD["headerText"] = theme->darkGridTheme["headerText"].name().toStdString();
    gridD["hourText"] = theme->darkGridTheme["hourText"].name().toStdString();
    gridD["alpha"] = theme->darkAlpha["grid"];
    dark["grid"] = gridD;

    YAML::Node activityD;
    activityD["font"] = theme->darkActivityTheme["font"].name().toStdString();
    activityD["alpha"] = theme->darkAlpha["activity"];
    dark["activity"] = activityD;

    YAML::Node schActD;
    schActD["overlap"] = theme->darkScheduleATheme["overlap"].name().toStdString();
    schActD["font"] = theme->darkScheduleATheme["font"].name().toStdString();
    dark["scheduleActivity"] = schActD;

    settings["light"] = light;
    settings["dark"] = dark;
    settings["activitiesGradient"] = theme->getGradientActivityPresetName().toStdString();

    return settings;
}

QColor ConfigurationParser::getColor(YAML::Node node, QColor defaultC) {
    return node.IsDefined()? QColor(node.as<string>().c_str()) : defaultC;
}

int ConfigurationParser::getInt(YAML::Node node, int defaultI) {
    return node.IsDefined()? node.as<int>() : defaultI;
}

QString ConfigurationParser::getString(YAML::Node node, QString defaultS) {
    return node.IsDefined()? QString(node.as<string>().c_str()) : defaultS;
}
