#include "Config.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <nlohmann/json.h>

using json = nlohmann::json;

// Define static variables
float Config::PLAYER_MOVEMENT_SPEED;
int Config::MAZE_WIDTH;
int Config::MAZE_HEIGHT;
int Config::SCREEN_WIDTH;
int Config::SCREEN_HEIGHT;
int Config::EXPLORE_RADIUS;
int Config::COMPLETED_MAZES_TO_WIN;
int Config::MAX_ITEMS_IN_MAZE_SECTION;
int Config::MIN_ITEMS_IN_MAZE_SECTION;

void Config::resetConst() {
    PLAYER_MOVEMENT_SPEED = 5.0f;
    MAZE_WIDTH = 10;
    MAZE_HEIGHT = 10;
    SCREEN_WIDTH = 800;
    SCREEN_HEIGHT = 600;
    EXPLORE_RADIUS = 5;
    COMPLETED_MAZES_TO_WIN = 3;
    MAX_ITEMS_IN_MAZE_SECTION = 10;
    MIN_ITEMS_IN_MAZE_SECTION = 1;
}

void Config::init(const std::string& configFile) {
    const Config config(configFile);
    try
    {
        PLAYER_MOVEMENT_SPEED = config.getFloat("PLAYER_MOVEMENT_SPEED");
        MAZE_WIDTH = config.getInt("MAZE_WIDTH");
        MAZE_HEIGHT = config.getInt("MAZE_HEIGHT");
        SCREEN_WIDTH = config.getInt("SCREEN_WIDTH");
        SCREEN_HEIGHT = config.getInt("SCREEN_HEIGHT");
        EXPLORE_RADIUS = config.getInt("EXPLORE_RADIUS");
        COMPLETED_MAZES_TO_WIN = config.getInt("COMPLETED_MAZES_TO_WIN");
        MAX_ITEMS_IN_MAZE_SECTION = config.getInt("MAX_ITEMS_IN_MAZE_SECTION");
        MIN_ITEMS_IN_MAZE_SECTION = config.getInt("MIN_ITEMS_IN_MAZE_SECTION");

        if (MAZE_WIDTH > SCREEN_WIDTH || MAZE_HEIGHT > SCREEN_HEIGHT)
        {
            resetConst();
            throw std::runtime_error("Maze dimensions cannot be larger than screen dimensions!\nDefault values are set.");
        }

        std::cout << "Configuration loaded successfully!" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "There was an error loading the configuration file: " << e.what() << std::endl;
    }
}

Config::Config(const std::string& configFile) {
    resetConst();
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
        std::stringstream ss;
        ss << it.value();
        m_configData[it.key()] = ss.str();
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