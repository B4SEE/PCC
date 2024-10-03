#include <string>
#include <unordered_map>

class Config {
public:
    // static constexpr float PLAYER_MOVEMENT_SPEED;
    // static constexpr int MAZE_WIDTH;
    // static constexpr int MAZE_HEIGHT;
    // static constexpr int SCREEN_WIDTH;
    // static constexpr int SCREEN_HEIGHT;
    // static constexpr int EXPLORE_RADIUS;
    // static constexpr int COMPLETED_MAZES_TO_WIN;
    // static constexpr int MAX_ITEMS_IN_MAZE_SECTION;
    // static constexpr int MIN_ITEMS_IN_MAZE_SECTION;

    // static_assert(MAZE_WIDTH <= SCREEN_WIDTH, "Maze width cannot be greater than screen width");
    // static_assert(MAZE_HEIGHT <= SCREEN_HEIGHT, "Maze height cannot be greater than screen height");

    Config(const std::string& configFile);

    int getInt(const std::string& key) const;
    float getFloat(const std::string& key) const;
    std::string getString(const std::string& key) const;

    private:
        std::unordered_map<std::string, std::string> m_configData;

        void loadConfigFile(const std::string& configFile);
};