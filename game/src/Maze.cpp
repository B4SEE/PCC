#include <random>
#include <vector>
#include <utility>
#include <iostream>
#include <map>
#include <cstdlib>
#include <ctime>
#include <stack>
#include "Maze.h"
#include "Log.h"

#include <algorithm>

Maze::Maze(int width, int height, Difficulty difficulty)
    : width(width), height(height) {
    createMaze(difficulty);
}

void Maze::generateItems(int itemCount) {
    // Randomly place items in the maze
    for (int i = 0; i < itemCount; i++) {
        int x = rand() % displayGrid[0].size();
        int y = rand() % displayGrid.size();
        if (displayGrid[y][x] == 0) {
            displayGrid[y][x] = 3;
            itemsPosition.push_back(std::make_pair(x, y));
        }
    }
}

void Maze::pickItem(int x, int y) {
    if (displayGrid[y][x] == 3) {
        displayGrid[y][x] = 0;
        LOG_INFO("Item picked up at position ({}, {})", x, y);
    }
}

void Maze::setPlayerPosition(int x, int y) {
    if (!displayGrid.empty()) {
        if (x > 0 && y > 0) {
            try {
                displayGrid[playerPosition.first][playerPosition.second] = 0;
                displayGrid[x][y] = 2;
                playerPosition = std::make_pair(x, y);
            } catch (const std::out_of_range& e) {
                exit(1);
            }
        }
    }
}

std::pair<int, int> Maze::getPlayerPosition() const {
    return playerPosition;
}

const std::vector<std::vector<int>>& Maze::getMaze() const {
    return displayGrid;
}

void Maze::createMaze(Difficulty difficulty) {
    switch (difficulty) {
        case Difficulty::EASY:
            createWilsonMaze();
            break;
        case Difficulty::MEDIUM:
            createKruskalMaze();
            break;
        case Difficulty::HARD:
            createDepthFirstMaze();
            break;
    }
}

void Maze::displayMaze() {
    // clear screen
    std::cout << "\033[2J";
    // move cursor to top left
    std::cout << "\033[0;0H";

    if (displayGrid.empty()) {
        return;
    }

    for (auto & y : displayGrid) {
        for (int x : y) {
            // use switch statement to set color based on value
            switch (x) {
                case 0:
                    // empty space
                    std::cout << " ";
                    break;
                case 1:
                    // wall
                    std::cout << "#";
                    break;
                case 2:
                    // red player
                    std::cout << "\033[1;31mo\033[0m";
                    break;
                case 3:
                    // yellow item
                    std::cout << "\033[1;33m*\033[0m";
                    break;
                default:
                    // unknown value
                    std::cout << "?";
                    break;
            }
        }
        std::cout << std::endl;
    }
}



// Use Wilson's algorithm to generate a maze
void Maze::createWilsonMaze() {
    const int N = 1, S = 2, E = 4, W = 8;
    const int IN_CELL = 0x10;

    const std::map<int, int> DX = {{E, 1}, {W, -1}, {N, 0}, {S, 0}};
    const std::map<int, int> DY = {{E, 0}, {W, 0}, {N, -1}, {S, 1}};
    const std::map<int, int> OPPOSITE = {{E, W}, {W, E}, {N, S}, {S, N}};

    auto walk = [&](std::vector<std::vector<int>>& grid) -> std::vector<std::tuple<int, int, int>> {

        // Randomly select a cell
        while (true) {
            int cx = rand() % grid[0].size();
            int cy = rand() % grid.size();
            if (grid[cy][cx] != 0) continue;

            std::map<std::pair<int, int>, int> visits;
            visits[{cx, cy}] = 0;

            int start_x = cx, start_y = cy;
            bool walking = true;

            // Random walk
            while (walking) {
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
                        }
                        cx = nx;
                        cy = ny;
                        walking = true;
                        break;
                    }
                }
            }

            std::vector<std::tuple<int, int, int>> path;
            int x = start_x, y = start_y;

            // Reconstruct path
            while (true) {
                auto it = visits.find({x, y});
                if (it == visits.end()) break;
                int dir = it->second;
                path.emplace_back(x, y, dir);
                x += DX.at(dir);
                y += DY.at(dir);
            }

            return path;
        }
    };

    // Generate raw maze map
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
        }
    }

    // Convert raw maze to display grid
    displayGrid = std::vector<std::vector<int>>(mazeMapRaw.size() * 2 + 1, std::vector<int>(mazeMapRaw[0].size() * 2 + 1, 1));

    for (int y = 0; y < mazeMapRaw.size(); y++) {
        for (int x = 0; x < mazeMapRaw[y].size(); x++) {
            int displayY = y * 2 + 1;
            int displayX = x * 2 + 1;

            displayGrid[displayY][displayX] = 0;

            if (mazeMapRaw[y][x] & S) {
                displayGrid[displayY + 1][displayX] = 0;
            }
            if (mazeMapRaw[y][x] & E) {
                displayGrid[displayY][displayX + 1] = 0;
            }
        }
    }

    // Place items in the maze
    generateItems(5);

    // Set exit
    displayGrid[displayGrid.size() - 2][displayGrid[0].size() - 1] = 0;

    // Set player position
    setPlayerPosition(1, 1);
}

// Uses Iterative randomized Kruskal's algorithm to generate a maze
void Maze::createKruskalMaze() {
    srand(time(0));
    int rows = height;
    int cols = width;

    // Initialize the maze with walls
    displayGrid = std::vector<std::vector<int>>(rows * 2 + 1, std::vector<int>(cols * 2 + 1, 1));

    // Union-Find data structure
    std::vector<int> parent(rows * cols);
    std::iota(parent.begin(), parent.end(), 0);

    std::function<int(int)> find = [&](int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    };

    auto unite = [&](int x, int y) {
        parent[find(x)] = find(y);
    };

    // List of all walls
    std::vector<std::tuple<int, int, int, int>> walls;
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            if (x < cols - 1) walls.emplace_back(y, x, y, x + 1);
            if (y < rows - 1) walls.emplace_back(y, x, y + 1, x);
        }
    }

    // Shuffle walls
    std::shuffle(walls.begin(), walls.end(), std::mt19937(std::random_device()()));

    // Process walls
    for (const auto& [y1, x1, y2, x2] : walls) {
        int cell1 = y1 * cols + x1;
        int cell2 = y2 * cols + x2;

        if (find(cell1) != find(cell2)) {
            unite(cell1, cell2);

            int displayY1 = y1 * 2 + 1;
            int displayX1 = x1 * 2 + 1;
            int displayY2 = y2 * 2 + 1;
            int displayX2 = x2 * 2 + 1;

            displayGrid[displayY1][displayX1] = 0;
            displayGrid[displayY2][displayX2] = 0;
            displayGrid[(displayY1 + displayY2) / 2][(displayX1 + displayX2) / 2] = 0;
        }
    }

    // Place items in the maze
    generateItems(5);

    // Set exit
    displayGrid[displayGrid.size() - 2][displayGrid[0].size() - 1] = 0;

    // Set player position
    setPlayerPosition(1, 1);
}

// Uses Randomized depth-first search algorithm to generate a maze
void Maze::createDepthFirstMaze() {
    srand(time(0));
    int rows = height;
    int cols = width;

    // Initialize the maze with walls
    displayGrid = std::vector<std::vector<int>>(rows * 2 + 1, std::vector<int>(cols * 2 + 1, 1));

    // Direction vectors for N, S, E, W
    const std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    const std::vector<std::pair<int, int>> displayDirections = {{-2, 0}, {2, 0}, {0, -2}, {0, 2}};

    // Stack for DFS
    std::stack<std::pair<int, int>> stack;
    stack.push({1, 1});
    displayGrid[1][1] = 0;

    while (!stack.empty()) {
        auto [cx, cy] = stack.top();
        std::vector<int> neighbors;

        // Find all valid neighbors
        for (int i = 0; i < directions.size(); ++i) {
            int nx = cx + directions[i].first * 2;
            int ny = cy + directions[i].second * 2;
            if (nx > 0 && ny > 0 && nx < rows * 2 && ny < cols * 2 && displayGrid[nx][ny] == 1) {
                neighbors.push_back(i);
            }
        }

        if (!neighbors.empty()) {
            // Choose a random neighbor
            int randIndex = rand() % neighbors.size();
            int dir = neighbors[randIndex];

            // Remove the wall between the current cell and the chosen neighbor
            int wx = cx + directions[dir].first;
            int wy = cy + directions[dir].second;
            int nx = cx + displayDirections[dir].first;
            int ny = cy + displayDirections[dir].second;

            displayGrid[wx][wy] = 0;
            displayGrid[nx][ny] = 0;

            // Push the neighbor to the stack
            stack.push({nx, ny});
        } else {
            stack.pop();
        }
    }

    // Place items in the maze
    generateItems(5);

    // Set exit
    displayGrid[displayGrid.size() - 2][displayGrid[0].size() - 1] = 0;

    // Set player position
    setPlayerPosition(1, 1);
}