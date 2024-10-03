#include "Config.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <nlohmann/json.h>

using json = nlohmann::json;

Config::Config(const std::string& configFile) {
    loadConfigFile(configFile);
}

void Config::loadConfigFile(const std::string& configFile) {
    std::ifstream file(configFile);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open config file: " + configFile);
    }

    json root;
    file >> root;

    for (auto it = root.begin(); it != root.end(); ++it) {
        m_configData[it.key()] = it.value().get<std::string>();
    } 
}  

int Config::getInt(const std::string& key) const {
    return std::stoi(m_configData.at(key));
}

float Config::getFloat(const std::string& key) const {
    return std::stof(m_configData.at(key));
}

std::string Config::getString(const std::string& key) const {
    return m_configData.at(key);
}