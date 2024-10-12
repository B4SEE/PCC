#include "Check.h"
#include "Config.h"
#include <stdexcept>
#include <streambuf>
#include <fstream>
#include <set>

#define watch(x) std::cout << (#x) << " is " << (x) << std::endl

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
