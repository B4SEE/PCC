#include "Config.h"
#include "logging/Log_func.h"

int main() {
    initLogging();
    Config::init("../resources/config.json");
}