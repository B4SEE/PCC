#include "Game.h"
#include "Render2dFlags.h"
#include <Config.h>

Game::Game()
    : inputHandler(false, [this](const std::string& userInput) {
        handleUserInput(userInput);
    }), isRunning(false) {}

Game::~Game() {
    stop();
}

void Game::start() {
    Config::init("../resources/config.json");
    resetGame();
    setup();

    renderer.start();
    Render2dFlags::resetConsole = true;
    renderer.notify();

    inputHandler.start();
    inputHandler.setInputCooldown(Config::MILLISECONDS_INPUT_DELAY);

    isRunning = true;
    canEnterInput = true;

    run();
}

void Game::run() {
    while (isRunning) {
        // Nothing happens here
    }
}

void Game::stop() {
    inputHandler.stop();
    renderer.stop();
}

void Game::handleUserInput(const std::string& userInput) {
    if (canEnterInput) {
        renderer.addHelpString(userInput);

        if (inputHandler.getRequireEnter()) {
            handleCommand(userInput);
        } else {
            handleKey(userInput);
        }

        Render2dFlags::showHelpInstructions = true;
        renderer.notify();
    }
}

void Game::showHelp() {
    renderer.addHelpString("Use WASD to move the player, L to toggle command line mode");
    renderer.addHelpString("Type 'exit' to quit the game");
    renderer.addHelpString("Type 'reset' to reset the console if you 'accidentally' break it");

    std::string difficultyStr;
    switch (difficulty) {
        case Difficulty::EASY:
            difficultyStr = "EASY";
            break;
        case Difficulty::MEDIUM:
            difficultyStr = "MEDIUM";
            break;
        case Difficulty::HARD:
            difficultyStr = "HARD";
            break;
    }
    
    renderer.addHelpString("Current difficulty: " + difficultyStr + ", mazes to complete: " + std::to_string(mazesToComplete));
    renderer.addHelpString("Type 'difficulty EASY', 'difficulty MEDIUM' or 'difficulty HARD' to change difficulty");
    Render2dFlags::showHelpInstructions = true;
    renderer.notify();
}


void Game::handleKey(const std::string& key) {
    // make Key uppercase
    std::string keyUpper(1, static_cast<char>(std::toupper(key[0])));
    if (keyUpper == Config::MOVE_FORWARD) {
        movePlayer(Direction::UP);
    } else if (keyUpper == Config::MOVE_BACKWARD) {
        movePlayer(Direction::DOWN);
    } else if (keyUpper == Config::MOVE_LEFT) {
        movePlayer(Direction::LEFT);
    } else if (keyUpper == Config::MOVE_RIGHT) {
        movePlayer(Direction::RIGHT);
    } else if (keyUpper == "L") {
        inputHandler.setRequireEnter(!inputHandler.getRequireEnter());
    } else if (keyUpper == "H") {
        showHelp();
    } else {
        renderer.addHelpString("Use 'H' to show help");
    }
}

void  Game::handleCommand(const std::string &command) {
    if (command == "exit") {
        isRunning = false;
    } else if (command == "help") {
        showHelp();
    } else if (command == "L") {
        inputHandler.setRequireEnter(!inputHandler.getRequireEnter());
    } else if (command == "reset") {
        renderer.clearHelpStrings();
        Render2dFlags::resetConsole = true;
        renderer.notify();
    } else if (command.find("difficulty") == 0) {
        std::string difficultyStr = command.substr(command.find(" ") + 1);
        if (difficultyStr == "EASY") {
            setDifficulty(Difficulty::EASY);
        }
        if (difficultyStr == "MEDIUM") {
            setDifficulty(Difficulty::MEDIUM);
        }
        if (difficultyStr == "HARD") {
            setDifficulty(Difficulty::HARD);
        }
        resetGame();
        setup();
    } else if (command == "show") {
        Render2dFlags::showAllMaze = !Render2dFlags::showAllMaze;
        Render2dFlags::drawMaze = true;
        renderer.notify();
    } else if (command == "restart") {
        resetGame();
        setup();
    } else {
        renderer.addHelpString("Enter 'help' for a list of commands");
    }
}

void Game::setDifficulty(Difficulty difficulty) {
    this->difficulty = difficulty;
    resetGame();
    setup();
    mazesToComplete = Config::COMPLETED_MAZES_TO_WIN;
}

void Game::resetGame() {
    canEnterInput = false;
    completedMazes = 0;
    mazesToComplete = Config::COMPLETED_MAZES_TO_WIN;
    currentMaze = nullptr;
    renderer.clearHelpStrings();
    renderer.setMaze(nullptr);
    renderer.notify();
}

void Game::setup() {
    // generate new mazes
    Maze maze(Config::MAZE_WIDTH / 2, Config::MAZE_HEIGHT / 2, difficulty);
    currentMaze = std::make_unique<Maze>(maze);

    renderer.setMaze(std::make_unique<Maze>(*currentMaze));

    Render2dFlags::resetConsole = true;
    renderer.notify();
    canEnterInput = true;
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

        // Check if position changed
        if (playerPosition != currentMaze->getPlayerPosition()) {
            // Check if player reached the exit
            if (currentMaze->getPlayerPosition() == currentMaze->getExitPosition()) {
                completedMazes++;
                if (completedMazes < mazesToComplete) {
                    this->currentMaze = std::make_unique<Maze>(Config::MAZE_WIDTH / 2, Config::MAZE_HEIGHT / 2, difficulty);
                    Render2dFlags::drawMaze = true;
                    renderer.notify();
                } else {
                    resetGame();
                    Render2dFlags::printWinMessage = true;
                    renderer.notify();
                    // Wait for some time
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                    setup();
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