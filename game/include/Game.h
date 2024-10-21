#pragma once

#include "Renderer_2d.h"
#include "InputHandler.h"

/**
 * @enum Direction
 * @brief The Direction enum represents the four cardinal directions. Used for player movement.
 */
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
    /**
     * @fn Game
     * @brief The constructor for the Game class.
     */
    Game();

    /**
     * @fn ~Game
     * @brief The destructor for the Game class.
     */
    ~Game();

    /**
     * @fn start
     * @brief Starts the game.
     */
    void start();

    /**
     * @fn stop
     * @brief Stops the game.
     */
    void stop();

    /**
     * @fn resetGame
     * @brief Resets the game.
     */
    void resetGame();

    /**
     * @fn setup
     * @brief Sets up the game.
     */
    void setup();

    /**
     * @fn setDifficulty
     * @brief Sets the difficulty of the game.
     * @param difficulty The difficulty to set.
     */
    void setDifficulty(Difficulty difficulty);
    
private:
    Renderer_2d renderer;
    InputHandler inputHandler;
    void handleUserInput(const std::string& userInput);

    void showHelp();

    void handleCommand(const std::string& command);
    void handleKey(const std::string& key);
    void movePlayer(Direction direction);
    bool isRunning = true;
    bool canEnterInput = false;

    int completedMazes = 0;
    int mazesToComplete = 0;
    Difficulty difficulty = Difficulty::EASY;
    std::unique_ptr<Maze> currentMaze;
    std::unique_ptr<Maze> nextMaze;
};