#include <random>
#include <vector>
#include <utility>
#include <iostream>
#include <map>
#include <cstdlib>
#include <ctime>
#include <unistd.h> // for usleep
#include "Maze.h"
#include "logging/Log_func.h"

#include <algorithm>

// Direction constants
const int N = 1, S = 2, E = 4, W = 8;
const int IN_CELL = 0x10;

// Direction maps for movement
const std::map<int, int> DX = {{E, 1}, {W, -1}, {N, 0}, {S, 0}};
const std::map<int, int> DY = {{E, 0}, {W, 0}, {N, -1}, {S, 1}};
const std::map<int, int> OPPOSITE = {{E, W}, {W, E}, {N, S}, {S, N}};

Maze::Maze(int width, int height, Difficulty difficulty)
    : width(width), height(height) {
    LOG(INFO) << "Initializing maze with width: " << width << " and height: " << height;
    createMaze(difficulty);
}

void Maze::generateItems(int itemCount) {
    LOG(INFO) << "Generating " << itemCount << " items in the maze.";
    // Method implementation
}

void Maze::setPlayerPosition(int x, int y) {
    LOG(DEBUG) << "Setting player position to (" << x << ", " << y << ")";
    if (!displayGrid.empty()) {
        if (x > 0 && y > 0) {
            try {
                displayGrid[playerPosition.first][playerPosition.second] = 0;
                displayGrid[x][y] = 2;
                playerPosition = std::make_pair(x, y);
            } catch (const std::out_of_range& e) {
                LOG(ERROR) << "Player position out of range: " << e.what();
                exit(1);
            }
        }
    }
}

std::pair<int, int> Maze::getPlayerPosition() const {
    LOG(DEBUG) << "Getting player position.";
    return playerPosition;
}

const std::vector<std::vector<int>>& Maze::getMaze() const {
    LOG(DEBUG) << "Getting maze grid.";
    return displayGrid;
}

void Maze::createMaze(Difficulty difficulty) {
    LOG(INFO) << "Creating maze with difficulty: " << static_cast<int>(difficulty);
    // switch (difficulty) {
    //     case Difficulty::EASY:
    //         createEasyMaze();
    //         break;
    //     case Difficulty::MEDIUM:
    //         createMediumMaze();
    //         break;
    //     case Difficulty::HARD:
    //         createHardMaze();
    //         break;
    // }
    createEasyMaze();
    LOG(INFO) << "Maze creation completed.";
}

void Maze::displayMaze(const std::vector<std::vector<int>>& grid, bool raw) {
    std::cout << "\033[0;0H";

    if (raw) {
        displayGrid = std::vector<std::vector<int>>(grid.size() * 2 + 1, std::vector<int>(grid[0].size() * 2 + 1, 1));

        for (int y = 0; y < grid.size(); y++) {
            for (int x = 0; x < grid[y].size(); x++) {
                int displayY = y * 2 + 1;
                int displayX = x * 2 + 1;

                displayGrid[displayY][displayX] = 0;

                if (grid[y][x] & S) {
                    displayGrid[displayY + 1][displayX] = 0;
                }
                if (grid[y][x] & E) {
                    displayGrid[displayY][displayX + 1] = 0;
                }
            }
        }
    }

    for (int y = 0; y < displayGrid.size(); y++) {
        for (int x = 0; x < displayGrid[y].size(); x++) {
            if (displayGrid[y][x] == 2) {
                // player position (red)
                std::cout << "\033[1;31mo\033[0m";
            } else {
                std::cout << (displayGrid[y][x] == 1 ? "#" : " ");
            }
        }
        std::cout << std::endl;
    }
}

std::vector<std::tuple<int, int, int>> walk(std::vector<std::vector<int>>& grid) {
    LOG(DEBUG) << "Starting walk to generate maze path.";
    while (true) {
        int cx = rand() % grid[0].size();
        int cy = rand() % grid.size();
        if (grid[cy][cx] != 0) continue;

        std::map<std::pair<int, int>, int> visits;
        visits[{cx, cy}] = 0;

        int start_x = cx, start_y = cy;
        bool walking = true;

        while (walking) {
            usleep(20000); // sleep for 20ms

            walking = false;
            std::vector<int> directions = {N, S, E, W};
            std::shuffle(directions.begin(), directions.end(), std::mt19937(std::random_device()()));
            for (int dir : directions) {
                int nx = cx + DX.at(dir);
                int ny = cy + DY.at(dir);
                if (nx >= 0 && ny >= 0 && ny < grid.size() && nx < grid[ny].size()) {
                    visits[{cx, cy}] = dir;

                    if (grid[ny][nx] != 0) {
                        walking = false;
                        break;
                    } else {
                        cx = nx;
                        cy = ny;
                        walking = true;
                        break;
                    }
                }
            }
        }

        std::vector<std::tuple<int, int, int>> path;
        int x = start_x, y = start_y;
        while (true) {
            auto it = visits.find({x, y});
            if (it == visits.end()) break;
            int dir = it->second;
            path.emplace_back(x, y, dir);
            x += DX.at(dir);
            y += DY.at(dir);
        }

        LOG(DEBUG) << "Walk completed, returning path.";
        return path;
    }
}

// Use Wilson's algorithm to generate a maze
void Maze::createEasyMaze() {
    std::cout << "\033[2J\033[H";
    LOG(INFO) << "Creating easy maze...";

    srand(time(0));
    std::vector<std::vector<int>> mazeMapRaw = std::vector<std::vector<int>>(height, std::vector<int>(width, 0));

    mazeMapRaw[rand() % height][rand() % width] = IN_CELL;
    int remaining = width * height - 1;

    while (remaining > 0) {
        for (const auto& [x, y, dir] : walk(mazeMapRaw)) {
            int nx = x + DX.at(dir);
            int ny = y + DY.at(dir);

            mazeMapRaw[y][x] |= dir;
            mazeMapRaw[ny][nx] |= OPPOSITE.at(dir);

            remaining--;

            displayMaze(mazeMapRaw, true);
            usleep(20000);
        }
    }

    // Set exit
    displayGrid[displayGrid.size() - 2][displayGrid[0].size() - 1] = 0;
    // Set player position
    setPlayerPosition(1, 1);

    LOG(INFO) << "Easy maze creation completed.";
}

// Uses Iterative randomized Kruskal's algorithm to generate a maze
void createMediumMaze() {
    LOG(INFO) << "Creating medium maze...";
    // Method implementation
    LOG(INFO) << "Medium maze creation completed.";
}

// Uses Randomized depth-first search algorithm to generate a maze
void createHardMaze() {
    LOG(INFO) << "Creating hard maze...";
    // Method implementation
    LOG(INFO) << "Hard maze creation completed.";
}

void Maze::placeItems(int itemCount) {
    LOG(INFO) << "Placing " << itemCount << " items in the maze.";
    // Method implementation
}
