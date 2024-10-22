#include "Check.h"
#include "Config.h"
#include <stdexcept>
#include <streambuf>
#include <fstream>
#include <set>

bool Check::checkConst() {
    if (Config::SCREEN_WIDTH < 0 || Config::SCREEN_HEIGHT < 0) {
        return false;
    }
    if (Config::MAZE_WIDTH <= 1 || Config::MAZE_HEIGHT <= 1) { // maze must be at least 2x2
        return false;
    }
    if (Config::MIN_ITEMS_IN_MAZE_SECTION < 0 || Config::MAX_ITEMS_IN_MAZE_SECTION <0) {
        Config::MIN_ITEMS_IN_MAZE_SECTION = 0;
        Config::MAX_ITEMS_IN_MAZE_SECTION = 0;
    }
    if (Config::MIN_ITEMS_IN_MAZE_SECTION > Config::MAX_ITEMS_IN_MAZE_SECTION) {
        int temp = Config::MIN_ITEMS_IN_MAZE_SECTION;
        Config::MIN_ITEMS_IN_MAZE_SECTION = Config::MAX_ITEMS_IN_MAZE_SECTION;
        Config::MAX_ITEMS_IN_MAZE_SECTION = temp;
    }
    if (Config::MIN_HELP_WINDOW_HEIGHT < 0) {
        Config::MIN_HELP_WINDOW_HEIGHT = 0;
    }
    // Might be useless
    if (Config::SCREEN_WIDTH > 120 || Config::SCREEN_HEIGHT > 30) {
        return false;
    }
    if (Config::MAZE_WIDTH > (Config::SCREEN_WIDTH * 0.75) || Config::MAZE_HEIGHT >= (Config::SCREEN_HEIGHT - 2 - 1 - 2 - Config::MIN_HELP_WINDOW_HEIGHT)) {
        // adjust maze size
        Config::MAZE_HEIGHT = Config::SCREEN_HEIGHT - 2 - 1 - 2 - Config::MIN_HELP_WINDOW_HEIGHT - 1;
        // width is always 75% of the console width
        Config::MAZE_WIDTH = Config::SCREEN_WIDTH * 0.75;
    }
    return true;
    // other constants check here
}


bool Check::checkKeybindings() {
    std::string keybinds[] = {
        Config::MOVE_FORWARD,
        Config::MOVE_BACKWARD,
        Config::MOVE_LEFT,
        Config::MOVE_RIGHT,
        Config::TURN_LEFT,
        Config::TURN_RIGHT,
        Config::OPEN_MAP
    };

    std::set<std::string> uniqueKeybinds;
    for (const std::string& keybind : keybinds) {
        if (keybind.empty()) {
            break;
        }

        if (keybind.length() > 1) {
            break;
        }

        if (!isalnum(keybind[0])) {
            break;
        }

        if (!uniqueKeybinds.insert(keybind).second) {
            break;
        }

        return true;
    }
    return false;
}
