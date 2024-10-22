#include "Config.h"
#include <fstream>
#include <sstream>
#include <nlohmann/json.h>
#include "Check.h"

using json = nlohmann::json;

// Define static variables
float Config::PLAYER_MOVEMENT_SPEED;
int Config::MAZE_WIDTH;
int Config::MAZE_HEIGHT;
int Config::SCREEN_WIDTH;
int Config::SCREEN_HEIGHT;
int Config::MIN_HELP_WINDOW_HEIGHT;
int Config::EXPLORE_RADIUS;
int Config::COMPLETED_MAZES_TO_WIN;
int Config::MAX_ITEMS_IN_MAZE_SECTION;
int Config::MIN_ITEMS_IN_MAZE_SECTION;
int Config::MILLISECONDS_INPUT_DELAY;

// Define keybindings
std::string Config::MOVE_FORWARD;
std::string Config::MOVE_BACKWARD;
std::string Config::MOVE_LEFT;
std::string Config::MOVE_RIGHT;
std::string Config::TURN_LEFT;
std::string Config::TURN_RIGHT;
std::string Config::OPEN_MAP;

void Config::resetKeybindings() {
    MOVE_FORWARD = "W";
    MOVE_BACKWARD = "S";
    MOVE_LEFT = "A";
    MOVE_RIGHT = "D";
    TURN_LEFT = "Q";
    TURN_RIGHT = "E";
    OPEN_MAP = "M";
}

void Config::resetConst() {
    PLAYER_MOVEMENT_SPEED = 5.0f;
    MAZE_WIDTH = 10;
    MAZE_HEIGHT = 10;
    SCREEN_WIDTH = 120;
    SCREEN_HEIGHT = 30;
    MIN_HELP_WINDOW_HEIGHT = 5;
    EXPLORE_RADIUS = 5;
    COMPLETED_MAZES_TO_WIN = 3;
    MAX_ITEMS_IN_MAZE_SECTION = 10;
    MIN_ITEMS_IN_MAZE_SECTION = 1;
    MILLISECONDS_INPUT_DELAY = 350;
}

void Config::init(const std::string& configFile) {
    const Config config(configFile);

    try
    {
        // Note: Some lines are for future use
        PLAYER_MOVEMENT_SPEED = config.getFloat("PLAYER_MOVEMENT_SPEED");
        MAZE_WIDTH = config.getInt("MAZE_WIDTH");
        MAZE_HEIGHT = config.getInt("MAZE_HEIGHT");
        SCREEN_WIDTH = config.getInt("SCREEN_WIDTH");
        SCREEN_HEIGHT = config.getInt("SCREEN_HEIGHT");
        MIN_HELP_WINDOW_HEIGHT = config.getInt("MIN_HELP_WINDOW_HEIGHT");
        EXPLORE_RADIUS = config.getInt("EXPLORE_RADIUS");
        COMPLETED_MAZES_TO_WIN = config.getInt("COMPLETED_MAZES_TO_WIN");
        MAX_ITEMS_IN_MAZE_SECTION = config.getInt("MAX_ITEMS_IN_MAZE_SECTION");
        MIN_ITEMS_IN_MAZE_SECTION = config.getInt("MIN_ITEMS_IN_MAZE_SECTION");
        MILLISECONDS_INPUT_DELAY = config.getInt("MILLISECONDS_INPUT_DELAY");

        MOVE_FORWARD = config.getString("MOVE_FORWARD");
        MOVE_BACKWARD = config.getString("MOVE_BACKWARD");
        MOVE_LEFT = config.getString("MOVE_LEFT");
        MOVE_RIGHT = config.getString("MOVE_RIGHT");
        TURN_LEFT = config.getString("TURN_LEFT");
        TURN_RIGHT = config.getString("TURN_RIGHT");
        OPEN_MAP = config.getString("OPEN_MAP");

        if (!Check::checkConst())
        {
            resetConst();
        }

        if (!Check::checkKeybindings())
        {
            resetKeybindings();
        }
    }
    catch (const std::exception& e)
    {
        resetConst();
        resetKeybindings();
    }
}

Config::Config(const std::string& configFile) {
    resetConst();
    resetKeybindings();
    loadConfigFile(configFile);
}

void Config::loadConfigFile(const std::string& configFile) {
    std::ifstream file(configFile);
    if (!file.is_open()) {
        return;
    }

    json root;
    file >> root;

    for (const auto& [key, value] : root.items()) {
        if (value.is_string()) {
            m_configData[key] = value.get<std::string>();
        } else if (value.is_number()) {
            m_configData[key] = std::to_string(value.get<double>());
        }
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