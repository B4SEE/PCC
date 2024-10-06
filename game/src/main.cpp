#include "Config.h"
#include "logging/Log_func.h"
#include "Maze.h"

void walkInMaze(Maze& maze);

int main() {
    std::cout << "\033[31m" << "This is red text" << "\033[0m" << std::endl;
    initLogging();
    Config::init("../resources/config.json");

    Maze maze(10, 10, Maze::Difficulty::EASY);
    maze.displayMaze(maze.getMaze());

    walkInMaze(maze);
}

void redrawMaze(Maze& maze) {
    maze.displayMaze(maze.getMaze());
    // clear current line
    std::cout << "\033[2K";
    // clear all lines below
    std::cout << "\033[J";
}

void moveUp(Maze& maze) {
    std::pair<int, int> playerPos = maze.getPlayerPosition();
    int x = playerPos.first;
    int y = playerPos.second;

    if (x - 1 >= 0 && maze.getMaze()[x - 1][y] == 0) {
        maze.setPlayerPosition(x - 1, y);
    }
    redrawMaze(maze);
}

void moveDown(Maze& maze) {
    std::pair<int, int> playerPos = maze.getPlayerPosition();
    int x = playerPos.first;
    int y = playerPos.second;

    if (x + 1 < maze.getMaze()[0].size() && maze.getMaze()[x + 1][y] == 0) {
        maze.setPlayerPosition(x + 1, y);
    }
    redrawMaze(maze);
}

void moveLeft(Maze& maze) {
    std::pair<int, int> playerPos = maze.getPlayerPosition();
    int x = playerPos.first;
    int y = playerPos.second;

    if (y - 1 >= 0 && maze.getMaze()[x][y - 1] == 0) {
        maze.setPlayerPosition(x, y - 1);
    }
    redrawMaze(maze);
}

void moveRight(Maze& maze) {
    std::pair<int, int> playerPos = maze.getPlayerPosition();
    int x = playerPos.first;
    int y = playerPos.second;

    if (y + 1 < maze.getMaze().size() && maze.getMaze()[x][y + 1] == 0) {
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