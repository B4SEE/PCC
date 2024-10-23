#include "Renderer_2d.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif
#include <fstream>
#include <Config.h>
#include <Render2dFlags.h>

Renderer_2d::Renderer_2d() : running(false), consoleWidth(0), consoleHeight(0), subConsoleWidth(0), subConsoleHeight(0), maze(nullptr) {}

Renderer_2d::~Renderer_2d() {
    stop();
}

void Renderer_2d::notify() {
    std::lock_guard<std::mutex> lock(renderMutex);
    renderCondition.notify_all();
}

void Renderer_2d::start() {
    running = true;
    renderThread = std::thread(&Renderer_2d::renderLoop, this);
}

void Renderer_2d::stop() {
    {
        std::lock_guard<std::mutex> lock(renderMutex);
        running = false;
    }
    renderCondition.notify_all();
    if (renderThread.joinable()) {
        renderThread.join();
    }
}

void switchThroughDisplayGrid(int num) {
    switch (num) {
        case 0:
            std::cout << " ";
            break;
        case 1:
            std::cout << "#";
            break;
        case 2:
            std::cout << "\033[1;31mo\033[0m";
            break;
        case 3:
            std::cout << "\033[1;32m*\033[0m";
            break;
        case 4:
            std::cout << "\033[1;34mX\033[0m";
            break;
        default:
            std::cout << "?";
            break;
    }
}

void Renderer_2d::drawMaze() {
    if (maze == nullptr) {
        return;
    }

    std::cout << mazeWindowStart;

    auto playerPosition = maze->getPlayerPosition();
    int playerX = playerPosition.first;
    int playerY = playerPosition.second;
    int exploreRadius = Config::EXPLORE_RADIUS;

    auto displayGrid = maze->getDisplayGrid();

    if (displayGrid.empty() || displayGrid[0].empty()) {
        return;
    }

    try {
        for (size_t x = 0; x < displayGrid.size(); ++x) {
            for (size_t y = 0; y < displayGrid[0].size(); ++y) {
                if (std::abs(playerX - static_cast<int>(x)) <= exploreRadius && std::abs(playerY - static_cast<int>(y)) <= exploreRadius) {
                    switchThroughDisplayGrid(displayGrid[x][y]);
                } else {
                    std::cout << "X";
                }
            }
            std::cout << std::endl;
            std::cout << "\033[1C";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error drawing maze: " << e.what() << std::endl;
    }
}

void Renderer_2d::printWinMessage() {
    clearScreen();
    drawBorders();

    std::string winMessage = "Congratulations! You Win!";
    int messageLength = winMessage.length();

    int centerX = (subConsoleWidth - messageLength) / 2;
    int centerY = subConsoleHeight / 2;

    std::cout << "\033[" << centerY << ";" << centerX << "H";
    std::cout << "\033[1;32m" << winMessage << "\033[0m" << std::endl;
}

void Renderer_2d::drawAllMaze() {
    std::cout << mazeWindowStart;
    if (maze == nullptr) {
        for (int i = 0; i < mazeWindowHeight; ++i) {
            for (int j = 0; j < mazeWindowWidth; ++j) {
                std::cout << "#";
            }
            std::cout << std::endl;
            std::cout << "\033[1C";
        }
        return;
    }

    auto displayGrid = maze->getDisplayGrid();

    if (displayGrid.empty() || displayGrid[0].empty()) {
        return;
    }

    try {
        for (size_t x = 0; x < displayGrid.size(); ++x) {
            for (size_t y = 0; y < displayGrid[0].size(); ++y) {
                switchThroughDisplayGrid(displayGrid[x][y]);
            }
            std::cout << std::endl;
            std::cout << "\033[1C";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error drawing maze: " << e.what() << std::endl;
    }
}

void Renderer_2d::redrawPlayer() {
    int radius = Config::EXPLORE_RADIUS + 1;
    if (maze == nullptr) {
        return;
    }
    if (Render2dFlags::showAllMaze) {
        radius = 1;
    }

    auto displayGrid = maze->getDisplayGrid();
    auto playerPosition = maze->getPlayerPosition();
    int playerX = playerPosition.first;
    int playerY = playerPosition.second;

    // Calculate rectangle to redraw
    int startX = playerX - radius;
    int startY = playerY - radius;
    int endX = playerX + radius;
    int endY = playerY + radius;

    // Go through displayGrid and redraw the rectangle
    for (int x = startX; x <= endX; ++x) {
        for (int y = startY; y <= endY; ++y) {
            if (x < 0 || y < 0 || x >= static_cast<int>(displayGrid.size()) || y >= static_cast<int>(displayGrid[0].size())) {
                continue;
            }

            std::cout << "\033[" << x + 1 + 1 << ";" << y + 1 + 1<< "H";
            if (!Render2dFlags::showAllMaze && (std::abs(playerX - x) >= radius || std::abs(playerY - y) >= radius)) {
                std::cout << "X";
            } else {
                switchThroughDisplayGrid(displayGrid[x][y]);
            }
        }
    }
}

void Renderer_2d::drawInputLine() {
    std::cout << inputLineStart;
    for (int i = 0; i < subConsoleWidth - 2; ++i) {
        std::cout << " ";
    }
    std::cout << std::endl;
    std::cout << "\033[1C";
}

void Renderer_2d::clearInputLine() {
    if (helpStrings.empty()) {
        return;
    }

    std::string lastHelpString = helpStrings.back();

    std::cout << inputLineStart;
    for (size_t i = 0; i < lastHelpString.size(); ++i) {
        std::cout << " ";
    }
    std::cout << inputLineStart;
}

void Renderer_2d::drawItemCounter() {
    int itemsCollected = 0;
    int totalItems = 0;
    if (maze != nullptr) {
        itemsCollected = maze->getItemsCollected();
        totalItems = maze->getTotalItems();
    }

    // Clear previous item counter
    std::cout << itemCounterStart;
    // Get max width of item counter
    // turn Config::MAX_ITEMS into a string
    std::string maxItemsStr = std::to_string(Config::MAX_ITEMS_IN_MAZE_SECTION);
    std::string itemCounterStr = "I: ";
    int maxItemsStrLength = maxItemsStr.size() * 2 + 1 + itemCounterStr.size(); // 1 for the slash
    for (int i = 0; i < maxItemsStrLength; ++i) {
        std::cout << " ";
    }

    std::cout << itemCounterStart << "I: " << itemsCollected << "/" << totalItems;
}

void Renderer_2d::showHelp() {

    //TODO: fix
    int maxHelpStringLength = 0;
    for (const auto& helpString : helpStrings) {
        maxHelpStringLength = std::max(maxHelpStringLength, static_cast<int>(helpString.size()));
    }

    if (maxHelpStringLength >= maxHelpLineLength) { // If not print string first
        maxHelpLineLength = maxHelpStringLength;
    }

    std::cout << helpWindowFirstLine;
    for (int i = 0; i < helpWindowHeight; ++i) {
        // Clear line first
        for (int j = 0; j < maxHelpLineLength; ++j) {
            std::cout << " ";
        }
        std::cout << "\033[2G";

        if (i < helpStrings.size()) {
            for (int j = 0; j < maxHelpLineLength; ++j) {
                if (j < helpStrings[i].size()) {
                    std::cout << "\033[36m" << helpStrings[i][j] << "\033[0m";
                }
            }
        }

        std::cout << std::endl;
        std::cout << "\033[1C";
    }

    maxHelpLineLength = maxHelpStringLength; // update maxHelpLineLength
}

void Renderer_2d::setMaze(std::unique_ptr<Maze> newMaze) {
    maze = std::move(newMaze);
}

void Renderer_2d::calculateSubConsole() {
    #ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    } else {
        std::cerr << "Error getting console buffer info" << std::endl;
        consoleWidth = 120;
        consoleHeight = 30;
    }
    #else
    struct winsize size;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == 0) {
        consoleWidth = size.ws_col;
        consoleHeight = size.ws_row;
        logRen("Console dimensions (Unix): Width = " + std::to_string(consoleWidth) + ", Height = " + std::to_string(consoleHeight));
    } else {
        std::cerr << "Error getting console window size" << std::endl;
        logRen("Error getting console window size");
        consoleWidth = 120;
        consoleHeight = 30;
    }
    #endif

    std::cout << subConsoleStart;

    // check if console is too small
    if (consoleWidth < Config::SCREEN_WIDTH || consoleHeight < Config::SCREEN_HEIGHT) {
        std::cerr << "Error: Console is too small" << std::endl;
        return;
    }

    subConsoleWidth = consoleWidth;
    subConsoleHeight = consoleHeight;

    subConsoleStart = "\033[0;0H";
    subConsoleEnd = "\033[" + std::to_string(subConsoleHeight) + ";" + std::to_string(subConsoleWidth) + "H";

    if (subConsoleWidth <= 0 || subConsoleHeight <= 0) {
        return;
    }

    if (maze == nullptr) {
        mazeWindowWidth = Config::MAZE_WIDTH;
        mazeWindowHeight = Config::MAZE_HEIGHT;
    } else {
        auto displayGrid = maze->getDisplayGrid();

        if (displayGrid.empty() || displayGrid[0].empty()) {
            std::cerr << "Error: displayGrid is empty" << std::endl;
            return;
        }

        mazeWindowWidth = displayGrid[0].size();
        mazeWindowHeight = displayGrid.size();
    }

    helpWindowHeight = subConsoleHeight - 2 - 1 - 2 - mazeWindowHeight;

    mazeWindowStart = "\033[2;2H";
    mazeWindowEnd = "\033[" + std::to_string(mazeWindowHeight + 1) + ";" + std::to_string(mazeWindowWidth + 1) + "H";

    itemCounterStart = "\033[2;" + std::to_string(mazeWindowWidth + 5) + "H";

    inputLineStart = "\033[" + std::to_string(subConsoleHeight - 2 - helpWindowHeight) + ";2H";

    helpWindowFirstLine = "\033[" + std::to_string(subConsoleHeight - helpWindowHeight) + ";2H";
    helpWindowLastLine = "\033[" + std::to_string(subConsoleHeight - 1) + ";2H";

    helpStrings.resize(helpWindowHeight);
    for (auto& helpString : helpStrings) {
        if (helpString.empty()) {
            helpString = "";
        }
    }
}

void Renderer_2d::drawSeparator(const std::string& cursorPosition) {
    std::cout << cursorPosition;
    for (int i = 0; i < subConsoleWidth - 2; ++i) {
        std::cout << "-";
    }
}

void Renderer_2d::drawBorders() {
    std::cout << subConsoleStart;
    for (int i = 0; i < subConsoleWidth; ++i) {
        std::cout << "*";
    }
    for (int i = 1; i < subConsoleHeight - 1; ++i) {
        std::cout << "\033[" << i + 1 << ";1H*";
        std::cout << "\033[" << i + 1 << ";" << subConsoleWidth << "H*";
    }
    std::cout << "\033[" << subConsoleHeight << ";1H";
    for (int i = 0; i < subConsoleWidth; ++i) {
        std::cout << "*";
    }
}

void Renderer_2d::drawSubConsole() {
    clearScreen();
    calculateSubConsole();

    drawBorders();
    drawMaze();
    drawItemCounter();
    drawSeparator(inputLineStart + "\033[1A");
    drawInputLine();
    drawSeparator(inputLineStart + "\033[1B");
    showHelp();

    clearInputLine();
}

void Renderer_2d::clearScreen() {
    std::cout << subConsoleStart;
    // Use OS-specific method to clear the screen
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

void Renderer_2d::addHelpString(const std::string& helpString) {
    for (size_t i = 0; i < helpStrings.size() - 1; ++i) {
        helpStrings[i] = helpStrings[i + 1];
    }
    helpStrings.back() = helpString;
    clearInputLine();
    notify();
}

void Renderer_2d::clearHelpStrings() {
    for (auto& helpString : helpStrings) {
        helpString = "";
    }
    clearInputLine();
    notify();
}

void Renderer_2d::renderLoop() {
    while (running) {
        std::unique_lock<std::mutex> lock(renderMutex);
        renderCondition.wait(lock, [this] {
            return !running || 
               Render2dFlags::clearScreen || 
               Render2dFlags::resetConsole || 
               Render2dFlags::updateCollectedItems || 
               Render2dFlags::drawMaze || 
               Render2dFlags::drawInputLine || 
               Render2dFlags::showHelpInstructions || 
               Render2dFlags::redrawPlayer || 
               Render2dFlags::printWinMessage;
        });

        if (Render2dFlags::clearScreen) {
            clearScreen();
            Render2dFlags::clearScreen = false;
        }

        if (Render2dFlags::resetConsole) {
            clearScreen();
            calculateSubConsole();
            drawSubConsole();
            Render2dFlags::resetConsole = false;
        }

        if (Render2dFlags::updateCollectedItems) {
            drawItemCounter();
            clearInputLine();
            Render2dFlags::updateCollectedItems = false;
        }

        if (Render2dFlags::drawInputLine) {
            clearInputLine();
            Render2dFlags::drawInputLine = false;
        }

        if (Render2dFlags::showHelpInstructions) {
            showHelp();
            clearInputLine();
            Render2dFlags::showHelpInstructions = false;
        }

        if (Render2dFlags::drawMaze) {
            if (Render2dFlags::showAllMaze) {
                drawAllMaze();
            } else {
                drawMaze();
            }
            clearInputLine();
            Render2dFlags::drawMaze = false;
        }

        if (Render2dFlags::redrawPlayer) {
            redrawPlayer();
            drawItemCounter();
            clearInputLine();
            Render2dFlags::redrawPlayer = false;
        }

        if (Render2dFlags::printWinMessage) {
            printWinMessage();
            Render2dFlags::printWinMessage = false;
        }
    }
}