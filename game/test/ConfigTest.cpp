#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "Config.h"

TEST_CASE("Config initialization", "[Config]") {
    SECTION("Default values") {
        Config::initDefault();

        REQUIRE(Config::PLAYER_MOVEMENT_SPEED == 5.0f);
        REQUIRE(Config::MAZE_WIDTH == 10);
        REQUIRE(Config::MAZE_HEIGHT == 10);
        REQUIRE(Config::SCREEN_WIDTH == 120);
        REQUIRE(Config::SCREEN_HEIGHT == 30);
        REQUIRE(Config::MIN_HELP_WINDOW_HEIGHT == 5);
        REQUIRE(Config::EXPLORE_RADIUS == 5);
        REQUIRE(Config::COMPLETED_MAZES_TO_WIN == 3);
        REQUIRE(Config::MAX_ITEMS_IN_MAZE_SECTION == 10);
        REQUIRE(Config::MIN_ITEMS_IN_MAZE_SECTION == 1);
        REQUIRE(Config::MILLISECONDS_INPUT_DELAY == 350);

        REQUIRE(Config::MOVE_FORWARD == "W");
        REQUIRE(Config::MOVE_BACKWARD == "S");
        REQUIRE(Config::MOVE_LEFT == "A");
        REQUIRE(Config::MOVE_RIGHT == "D");
        REQUIRE(Config::TURN_LEFT == "Q");
        REQUIRE(Config::TURN_RIGHT == "E");
        REQUIRE(Config::OPEN_MAP == "M");
    }
}