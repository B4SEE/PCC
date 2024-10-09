#include "Config.h"
#include "Log.h"
#include "Maze.h"
#include <iostream>

void walkInMaze(Maze& maze);

int main() {
    initFileLogger();
    // enable debug
    logger->set_level(spdlog::level::debug);

    LOG_INFO("COLOR TEST");

    // use escape codes to change text color
    std::cout << "This is white text for trace\n";
    std::cout << "\033[1;32mThis is green text for info\n";
    std::cout << "\033[1;34mThis is blue text for debug\n";
    std::cout << "\033[1;33mThis is yellow text for warn\n";
    std::cout << "\033[1;31mThis is red text for error\n";
    std::cout << "\033[1;35mThis is magenta text for critical\n";
    std::cout << "\033[0mThis is back to white text\n";

    LOG_TRACE("trace message");
    LOG_INFO("info message");
    LOG_DEBUG("debug message");
    LOG_WARN("warn message");
    LOG_ERROR("error message");
    LOG_CRITICAL("critical message");

    Config::init("../resources/config.json");

    Maze maze(20, 20, Difficulty::HARD);
    maze.displayMaze();

    walkInMaze(maze);
}

void redrawMaze(Maze& maze) {
    maze.displayMaze();
    // clear current line
    std::cout << "\033[2K";
    // clear all lines below
    std::cout << "\033[J";
}

void moveUp(Maze& maze) {
    std::pair<int, int> playerPos = maze.getPlayerPosition();
    int x = playerPos.first;
    int y = playerPos.second;

    if (x - 1 >= 0 && maze.getMaze()[x - 1][y] != 1) {
        maze.pickItem(x - 1, y);
        maze.setPlayerPosition(x - 1, y);
    }
    redrawMaze(maze);
}

void moveDown(Maze& maze) {
    std::pair<int, int> playerPos = maze.getPlayerPosition();
    int x = playerPos.first;
    int y = playerPos.second;

    if (x + 1 < maze.getMaze()[0].size() && maze.getMaze()[x + 1][y] != 1) {
        maze.pickItem(x + 1, y);
        maze.setPlayerPosition(x + 1, y);
    }
    redrawMaze(maze);
}

void moveLeft(Maze& maze) {
    std::pair<int, int> playerPos = maze.getPlayerPosition();
    int x = playerPos.first;
    int y = playerPos.second;

    if (y - 1 >= 0 && maze.getMaze()[x][y - 1] != 1) {
        maze.pickItem(x, y - 1);
        maze.setPlayerPosition(x, y - 1);
    }
    redrawMaze(maze);
}

void moveRight(Maze& maze) {
    std::pair<int, int> playerPos = maze.getPlayerPosition();
    int x = playerPos.first;
    int y = playerPos.second;

    if (y + 1 < maze.getMaze().size() && maze.getMaze()[x][y + 1] != 1) {
        maze.pickItem(x, y + 1);
        maze.setPlayerPosition(x, y + 1);
    }
    redrawMaze(maze);
}

// will be replaced
void walkInMaze(Maze& maze) {
    // Get Key Press
    char key = ' ';
    while (key != 'q') {
        key = std::cin.get();
        switch (key) {
            case 'w':
                moveUp(maze);
                // clear last line
                break;
            case 'a':
                moveLeft(maze);
                break;
            case 's':
                moveDown(maze);
                break;
            case 'd':
                moveRight(maze);
                break;
            default:
                std::cout << "\033[2K";
                break;
        }
    }
}