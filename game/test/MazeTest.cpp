#include <catch2/catch.hpp>
#include "Maze.h"
#include "Config.h"
#include <iostream>

TEST_CASE("Maze initialization", "[Maze]") {
    Maze maze(15, 15, Difficulty::EASY);

    SECTION("Default values") {
        REQUIRE(maze.getItemsCollected() == 0);
        REQUIRE(maze.getTotalItems() > 0);
        REQUIRE(maze.getPlayerPosition() == std::make_pair(1, 1));
    }
}

TEST_CASE("Maze player position", "[Maze]") {
    Maze maze(Config::MAZE_WIDTH, Config::MAZE_HEIGHT, Difficulty::EASY);

    SECTION("Set and get player position") {
        // find free space
        int x = 0, y = 0;
        for (int i = 0; i < maze.getWidth(); i++) {
            for (int j = 0; j < maze.getHeight(); j++) {
                if (maze.getDisplayGrid()[i][j] == 0 || maze.getDisplayGrid()[i][j] == 3) {
                    x = i;
                    y = j;
                    break;
                }
            }
        }
        // set player position
        maze.setPlayerPosition(x, y);
        REQUIRE(maze.getPlayerPosition() == std::make_pair(x, y));
    }
}

TEST_CASE("Maze display grid", "[Maze]") {
    Maze maze(Config::MAZE_WIDTH, Config::MAZE_HEIGHT, Difficulty::EASY);

    SECTION("Display grid is not empty") {
        const auto& grid = maze.getDisplayGrid();
        REQUIRE(!grid.empty());
        REQUIRE(!grid[0].empty());
    }
}