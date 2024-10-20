#include "Game.h"
#include "Render2dFlags.h"
#include <Config.h>
#include <sstream>

Game::Game()
    : inputHandler(false, [this](const std::string& userInput) {
        handleUserInput(userInput);
    }), isRunning(false) {} // Initialize isRunning to false

Game::~Game() {
    stop();
}

void Game::start() {
    Config::init("../resources/config.json");
    resetGame();

    renderer.start();
    Render2dFlags::resetConsole = true;

    inputHandler.start();

    isRunning = true;
    while (isRunning) {
        // Game loop logic
    }
}

void Game::stop() {
    inputHandler.stop();
    renderer.stop();
}

void Game::handleUserInput(const std::string& userInput) {
    renderer.addHelpString(userInput);

    if (inputHandler.getRequireEnter()) {
        handleCommand(userInput);
    } else {
        handleKey(userInput);
    }

    Render2dFlags::showHelpInstructions = true;
    renderer.notify();
}

void Game::handleKey(const std::string& key) {
    if (key == "w") {
        movePlayer(Direction::UP);
    } else if (key == "s") {
        movePlayer(Direction::DOWN);
    } else if (key == "a") {
        movePlayer(Direction::LEFT);
    } else if (key == "d") {
        movePlayer(Direction::RIGHT);
    } else if (key == "L") {
        inputHandler.setRequireEnter(!inputHandler.getRequireEnter());
    } else {
        renderer.addHelpString("Invalid key");
    }
}

void  Game::handleCommand(const std::string &command) {
    if (command == "exit") {
        isRunning = false;
    } else if (command == "L") {
        inputHandler.setRequireEnter(!inputHandler.getRequireEnter());
    } else if (command == "reset") {
        Render2dFlags::resetConsole = true;
        renderer.notify();
    } else if (command.find("difficulty") == 0) {
        // get difficulty from command
        std::string difficultyStr = command.substr(command.find(" ") + 1);
        if (difficultyStr == "MEDIUM") {
            setDifficulty(Difficulty::MEDIUM);
        }
        if (difficultyStr == "HARD") {
            setDifficulty(Difficulty::HARD);
        }
    } else if (command == "show") {
        Render2dFlags::showAllMaze = true;
        Render2dFlags::redrawPlayer = true;
        renderer.notify();
    } else if (command == "restart") {
        resetGame();
    } else {
        renderer.addHelpString("Invalid command");
    }
}

void Game::setDifficulty(Difficulty difficulty) {
    this->difficulty = difficulty;
    resetGame();
    mazesToComplete = Config::COMPLETED_MAZES_TO_WIN;
}

void Game::resetGame() {
    completedMazes = 0;
    mazesToComplete = Config::COMPLETED_MAZES_TO_WIN;
    currentMaze = nullptr;
    nextMaze = nullptr;
    // generate new mazes
    Maze maze(Config::MAZE_WIDTH / 2, Config::MAZE_HEIGHT / 2, difficulty);
    currentMaze = std::make_unique<Maze>(maze);
    Maze nextMaze = Maze(Config::MAZE_WIDTH / 2, Config::MAZE_HEIGHT / 2, difficulty);
    this->nextMaze = std::make_unique<Maze>(nextMaze);

    renderer.setMaze(std::make_unique<Maze>(*currentMaze));
    Render2dFlags::resetConsole = true;
    renderer.notify();
}

void Game::movePlayer(Direction direction) {
    if (currentMaze) {
        // save current player position
        std::pair<int, int> playerPosition = currentMaze->getPlayerPosition();
        int newX = playerPosition.first;
        int newY = playerPosition.second;

        switch (direction) {
            case Direction::UP:
                newX -= 1;
            break;
            case Direction::DOWN:
                newX += 1;
            break;
            case Direction::LEFT:
                newY -= 1;
            break;
            case Direction::RIGHT:
                newY += 1;
            break;
        }

        currentMaze->setPlayerPosition(newX, newY);

        // check if position changed
        if (playerPosition != currentMaze->getPlayerPosition()) {
            // check if player reached the exit
            if (currentMaze->getPlayerPosition() == currentMaze->getExitPosition()) {
                completedMazes++;
                if (completedMazes < mazesToComplete) {
                    currentMaze = std::move(nextMaze);
                    Maze newMaze = Maze(Config::MAZE_WIDTH / 2, Config::MAZE_HEIGHT / 2, difficulty);
                    nextMaze = std::make_unique<Maze>(newMaze);
                } else {
                    resetGame();
                    Render2dFlags::printWinMessage = true;
                    renderer.notify();
                    // wait for some time
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                    Render2dFlags::resetConsole = true;
                    renderer.notify();
                    return;
                }
            }
        }

        renderer.setMaze(std::make_unique<Maze>(*currentMaze));
        Render2dFlags::redrawPlayer = true;
        renderer.notify();
    }
}