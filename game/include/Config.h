#pragma once
#include <string>
#include <unordered_map>

/**
 * @class Config
 * @brief Manages configuration settings for the game.
 *
 * The Config class is responsible for loading and storing configuration settings
 * from a file. It provides static members to access various configuration parameters
 * such as player movement speed, maze dimensions, screen dimensions, and other game settings.
 *
 * @note This class cannot be instantiated directly. Use the static init method to load
 *       configuration settings from a file.
 */
class Config {
public:
    /**
     * @var float Config::PLAYER_MOVEMENT_SPEED
     * The speed at which the player moves.
     */
    static float PLAYER_MOVEMENT_SPEED;

    /**
     * @var int Config::MAZE_WIDTH
     * The width of the maze.
     */
    static int MAZE_WIDTH;

    /**
     * @var int Config::MAZE_HEIGHT
     * The height of the maze.
     */
    static int MAZE_HEIGHT;

    /**
     * @var int Config::SCREEN_WIDTH
     * The width of the game screen.
     */
    static int SCREEN_WIDTH;

    /**
     * @var int Config::SCREEN_HEIGHT
     * The height of the game screen.
     */
    static int SCREEN_HEIGHT;

    /**
     * @var int Config::EXPLORE_RADIUS
     * The radius within which the player can explore.
     */
    static int EXPLORE_RADIUS;

    /**
     * @var int Config::COMPLETED_MAZES_TO_WIN
     * The number of mazes that need to be completed to win the game.
     */
    static int COMPLETED_MAZES_TO_WIN;

    /**
     * @var int Config::MAX_ITEMS_IN_MAZE_SECTION
     * The maximum number of items that can be present in a section of the maze.
     */
    static int MAX_ITEMS_IN_MAZE_SECTION;

    /**
     * @var int Config::MIN_ITEMS_IN_MAZE_SECTION
     * The minimum number of items that can be present in a section of the maze.
     */
    static int MIN_ITEMS_IN_MAZE_SECTION;

    /**
     * @fn void Config::init(const std::string& configFile)
     * @brief Initializes the configuration settings from the specified file.
     * @param configFile The path to the configuration file.
     */
    static void init(const std::string& configFile);    

private:
    /**
     * @fn Config::Config(const std::string& configFile)
     * @brief Private constructor to initialize the configuration settings from the specified file.
     * @param configFile The path to the configuration file.
     */
    explicit Config(const std::string& configFile);

    std::unordered_map<std::string, std::string> m_configData;

    /**
     * @fn void Config::loadConfigFile(const std::string& configFile)
     * @brief Loads the configuration settings from the specified file.
     * @param configFile The path to the configuration file.
     */
    void loadConfigFile(const std::string& configFile);

    /**
     * @fn int Config::getInt(const std::string& key) const
     * @brief Retrieves an integer value from the configuration data.
     * @param key The key associated with the integer value.
     * @return The integer value associated with the specified key.
     */
    int getInt(const std::string& key) const;

    /**
     * @fn float Config::getFloat(const std::string& key) const
     * @brief Retrieves a float value from the configuration data.
     * @param key The key associated with the float value.
     * @return The float value associated with the specified key.
     */
    float getFloat(const std::string& key) const;

    /**
     * @fn std::string Config::getString(const std::string& key) const
     * @brief Retrieves a string value from the configuration data.
     * @param key The key associated with the string value.
     * @return The string value associated with the specified key.
     */
    std::string getString(const std::string& key) const;

    /**
     * @fn void Config::resetConst()
     * @brief Resets all configuration constants to their default values.
     */
    static void resetConst();
};