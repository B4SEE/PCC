#include "Check.h"
#include "Config.h"
#include <stdexcept>
#include <streambuf>
#include <fstream>
#include <Log.h>
#include <set>
#include "spdlog/spdlog.h"

#define watch(x) std::cout << (#x) << " is " << (x) << std::endl

bool Check::checkKeybindings() {
    spdlog::info("Checking keybindings");

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
            LOG_WARN("Keybinding is empty");
            break;
        }

        if (keybind.length() > 1) {
            LOG_WARN("Keybinding is too long");
            break;
        }

        if (!isalnum(keybind[0])) {
            LOG_WARN("Keybinding is not alphanumeric");
            break;
        }

        if (!uniqueKeybinds.insert(keybind).second) {
            LOG_WARN("Keybinding is not unique");
            break;
        }

        return true;
    }
    return false;
}
