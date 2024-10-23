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
    // Constants
    static float PLAYER_MOVEMENT_SPEED;
    static int MAZE_WIDTH;
    static int MAZE_HEIGHT;
    static int SCREEN_WIDTH;
    static int SCREEN_HEIGHT;
    static int MIN_HELP_WINDOW_HEIGHT;
    static int EXPLORE_RADIUS;
    static int COMPLETED_MAZES_TO_WIN;
    static int MAX_ITEMS_IN_MAZE_SECTION;
    static int MIN_ITEMS_IN_MAZE_SECTION;
    static int MILLISECONDS_INPUT_DELAY;

    // Keybindings
    static std::string MOVE_FORWARD;
    static std::string MOVE_BACKWARD;
    static std::string MOVE_LEFT;
    static std::string MOVE_RIGHT;
    static std::string TURN_LEFT;
    static std::string TURN_RIGHT;
    static std::string OPEN_MAP;

    /**
     * @fn init
     * @brief Initializes the configuration settings from the specified file.
     * @param configFile The path to the configuration file.
     */
    static void init(const std::string& configFile);

    /**
     * @fn initDefault
     * @brief Initializes the configuration settings with default values.
     */
    static void initDefault();

private:
    /**
     * @fn Config
     * @brief Private constructor to initialize the configuration settings from the specified file.
     * @param configFile The path to the configuration file.
     */
    explicit Config(const std::string& configFile);

    // Configuration data
    std::unordered_map<std::string, std::string> m_configData;

    /**
     * @fn loadConfigFile
     * @brief Loads the configuration settings from the specified file.
     * @param configFile The path to the configuration file.
     */
    void loadConfigFile(const std::string& configFile);

    /**
     * @fn getInt
     * @brief Retrieves an integer value from the configuration data.
     * @param key The key associated with the integer value.
     * @return The integer value associated with the specified key.
     */
    int getInt(const std::string& key) const;

    /**
     * @fn getFloat
     * @brief Retrieves a float value from the configuration data.
     * @param key The key associated with the float value.
     * @return The float value associated with the specified key.
     */
    float getFloat(const std::string& key) const;

    /**
     * @fn getString
     * @brief Retrieves a string value from the configuration data.
     * @param key The key associated with the string value.
     * @return The string value associated with the specified key.
     */
    std::string getString(const std::string& key) const;

    /**
     * @fn resetConst()
     * @brief Resets all game constants to their default values.
     */
    static void resetConst();

    /**
     * @fn resetKeybindings()
     * @brief Resets all keybindings to their default values.
     */
    static void resetKeybindings();
};