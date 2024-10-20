#include "Renderer_2d.h"
#include "Render2dFlags.h"
#include <iostream>
#include <Config.h>
#include <sstream>
#include <InputHandler.h>
#include <chrono>

void handleUserInput(const std::string& userInput, Renderer_2d& renderer, InputHandler& inputHandler) {
    if (userInput == "exit") {
        renderer.stop();
        exit(0);
    }
    if (userInput == "L") {
        inputHandler.setRequireEnter(!inputHandler.getRequireEnter());
    }
    renderer.addHelpString(userInput);
    Render2dFlags::showHelpInstructions = true;
    renderer.notify();

    if (userInput.rfind("maze", 0) == 0) {
        std::istringstream iss(userInput);
        std::string command, difficultyStr;
        iss >> command >> difficultyStr;

        Difficulty difficulty = Difficulty::EASY;
        if (difficultyStr == "MEDIUM") {
            difficulty = Difficulty::MEDIUM;
        }
        if (difficultyStr == "HARD") {
            difficulty = Difficulty::HARD;
        }
        Maze maze(Config::MAZE_WIDTH / 2, Config::MAZE_HEIGHT / 2, difficulty);

        renderer.setMaze(maze);
        Render2dFlags::resetConsole = true;
        renderer.notify();
    }
}

int main() {
    Config::init("../resources/config.json");

    Renderer_2d renderer;
    renderer.start();
    Render2dFlags::resetConsole = true;

    InputHandler inputHandler(false, [&renderer, &inputHandler](const std::string& userInput) {
        handleUserInput(userInput, renderer, inputHandler);
    });
    inputHandler.start();

    while (true) {
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    inputHandler.stop();
    renderer.stop();

    return 0;
}