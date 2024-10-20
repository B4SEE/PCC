#pragma once

#include "Renderer_2d.h"
#include "InputHandler.h"

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

/**
 * @class Game
 * @brief The Game class is responsible for controlling the game logic.
 */
class Game {
public:
    Game();
    ~Game();
    void start();
    void stop();

    void resetGame();
    void setDifficulty(Difficulty difficulty);
private:
    Renderer_2d renderer;
    InputHandler inputHandler;
    void handleUserInput(const std::string& userInput);
    void handleCommand(const std::string& command);
    void handleKey(const std::string& key);
    void movePlayer(Direction direction);
    bool isRunning = true;

    int completedMazes = 0;
    int mazesToComplete = 0;
    Difficulty difficulty = Difficulty::EASY;
    std::unique_ptr<Maze> currentMaze;
    std::unique_ptr<Maze> nextMaze;
};