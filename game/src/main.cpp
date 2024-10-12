#include "Renderer_2d.h"
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>
#include <Config.h>

// void walkInMaze(Maze& maze);
// void redrawMaze(Maze& maze);

int main() {
    Config::init("../resources/config.json");

    Renderer_2d renderer;
    renderer.drawSubConsole();
    // wait for 5 seconds
    std::this_thread::sleep_for(std::chrono::seconds(60));

    return 0;
}

// void redrawMaze(Maze& maze) {
//     std::cout << "\033[H";
//     std::cout << "\033[J";
//     std::cout.flush();
//     // Display the maze
//     maze.displayMaze();
//     //
// }
//
// void clearLine() {
//     // move cursor to the line above
//     std::cout << "\033[1A";
//     // clear the line
//     std::cout << "\033[2K";
// }
//
// void moveUp(Maze& maze) {
//     std::pair<int, int> playerPos = maze.getPlayerPosition();
//     int x = playerPos.first;
//     int y = playerPos.second;
//
//     if (x - 1 >= 0 && maze.getMaze()[x - 1][y] != 1) {
//         maze.pickItem(x - 1, y);
//         maze.setPlayerPosition(x - 1, y);
//         redrawMaze(maze);
//     }
// }
//
// void moveDown(Maze& maze) {
//     std::pair<int, int> playerPos = maze.getPlayerPosition();
//     int x = playerPos.first;
//     int y = playerPos.second;
//
//     if (x + 1 < maze.getMaze()[0].size() && maze.getMaze()[x + 1][y] != 1) {
//         maze.pickItem(x + 1, y);
//         maze.setPlayerPosition(x + 1, y);
//         redrawMaze(maze);
//     }
// }
//
// void moveLeft(Maze& maze) {
//     std::pair<int, int> playerPos = maze.getPlayerPosition();
//     int x = playerPos.first;
//     int y = playerPos.second;
//
//     if (y - 1 >= 0 && maze.getMaze()[x][y - 1] != 1) {
//         maze.pickItem(x, y - 1);
//         maze.setPlayerPosition(x, y - 1);
//         redrawMaze(maze);
//     }
// }
//
// void moveRight(Maze& maze) {
//     std::pair<int, int> playerPos = maze.getPlayerPosition();
//     int x = playerPos.first;
//     int y = playerPos.second;
//
//     if (y + 1 < maze.getMaze().size() && maze.getMaze()[x][y + 1] != 1) {
//         maze.pickItem(x, y + 1);
//         maze.setPlayerPosition(x, y + 1);
//         redrawMaze(maze);
//     }
// }
//
// // will be replaced
// void walkInMaze(Maze& maze) {
//     // Get Key Press
//     char key = ' ';
//     while (key != 'q') {
//         key = std::cin.get();
//         switch (key) {
//             case 'w':
//                 moveUp(maze);
//                 break;
//             case 'a':
//                 moveLeft(maze);
//                 break;
//             case 's':
//                 moveDown(maze);
//                 break;
//             case 'd':
//                 moveRight(maze);
//                 break;
//             default:
//                 if (key == '\n') {
//                 } else {
//                     clearLine();
//                 }
//                 break;
//         }
//     }
// }