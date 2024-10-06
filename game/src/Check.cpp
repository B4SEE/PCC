#include "Check.h"
#include "Config.h"
#include <stdexcept>
#include <streambuf>
#include <fstream>
#include <set>
#include "logging/Log_func.h"

#define watch(x) std::cout << (#x) << " is " << (x) << std::endl

bool Check::checkKeybindings() {
    LOG(DEBUG) << "Checking keybindings";

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
            LOG(WARNING) << "Keybinding is empty";
            break;
        }

        if (keybind.length() > 1) {
            LOG(WARNING) << "Keybinding is longer than one character";
            break;
        }

        if (!isalnum(keybind[0])) {
            LOG(WARNING) << "Keybinding is not alphanumeric";
            break;
        }

        if (!uniqueKeybinds.insert(keybind).second) {
            LOG(WARNING) << "Keybinding is not unique";
            break;
        }

        return true;
    }
    return false;
}
