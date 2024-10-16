#include "Renderer_2d.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
#include <chrono>
#include <Config.h>
#include <Render2dFlags.h>

Renderer_2d::Renderer_2d() : running(false), consoleWidth(0), consoleHeight(0), subConsoleWidth(0), subConsoleHeight(0), maze(nullptr) {}

Renderer_2d::~Renderer_2d() {
    stop();
}

void Renderer_2d::notify() {
    std::unique_lock<std::mutex> lock(renderMutex);
    renderCondition.notify_all();
}

void Renderer_2d::start() {
    running = true;
    renderThread = std::thread(&Renderer_2d::renderLoop, this);
}

void Renderer_2d::stop() {
    {
        std::unique_lock<std::mutex> lock(renderMutex);
        running = false;
    }
    renderCondition.notify_all();
    if (renderThread.joinable()) {
        renderThread.join();
    }
}

void Renderer_2d::drawMaze() {
    std::cout << mazeWindowStart;
    if (maze == nullptr) {
        // Draw empty maze
        for (int i = 0; i < mazeWindowHeight; ++i) {
            for (int j = 0; j < mazeWindowWidth; ++j) {
                std::cout << "#";
            }
            std::cout << std::endl;
            std::cout << "\033[1C";
        }
        return;
    }

    std::vector<std::vector<int>> displayGrid;
    {
        displayGrid = maze->getDisplayGrid();
    }

    if (displayGrid.empty() || displayGrid[0].empty()) {
        return;
    }

    for (const auto& row : displayGrid) {
        for (int cell : row) {
            // use switch statement to set color based on value
            switch (cell) {
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
                    // green item
                        std::cout << "\033[1;32m*\033[0m";
                break;
                default:
                    // unknown value
                        std::cout << "?";
                break;
            }
        }
        std::cout << std::endl;
        std::cout << "\033[1C";
    }

    // flush the output stream
    std::cout.flush();
}

void Renderer_2d::drawInputLine() {
    std::cout << inputLineStart;
    for (int i = 0; i < subConsoleWidth - 2; ++i) {
        std::cout << "#";
    }
    std::cout << std::endl;
    std::cout << "\033[1C";
}

void Renderer_2d::clearInputLine() {
    std::cout << inputLineStart;
    for (int i = 0; i < subConsoleWidth - 2; ++i) {
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
    std::cout << itemCounterStart << "I: " << itemsCollected;
    std::cout << itemCounterStart << "\033[1B" << "T: " << totalItems;
}

void Renderer_2d::showHelp() {
    std::cout << helpWindowFirstLine;
    for (int i = 0; i < helpWindowHeight; ++i) {
        if (i < helpStrings.size()) {
            for (int j = 0; j < subConsoleWidth - 2; ++j) {
                if (j < helpStrings[i].size()) {
                    // write lines in color
                    std::cout << "\033[36m" << helpStrings[i][j] << "\033[0m";
                } else {
                    std::cout << " ";
                }
            }
            std::cout << std::endl;
            std::cout << "\033[1C";
        }
    }
}

void Renderer_2d::setMaze(Maze& maze) {
    this->maze = &maze;
}

void Renderer_2d::calculateSubConsole() {
    #ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    } else {
        std::cerr << "Error getting console buffer info" << std::endl;
        consoleWidth = 120; // Default width
        consoleHeight = 30; // Default height
    }
    #else
    struct winsize size;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == 0) {
        consoleWidth = size.ws_col;
        consoleHeight = size.ws_row;
    } else {
        std::cerr << "Error getting console window size" << std::endl;
        consoleWidth = 120; // Default width
        consoleHeight = 30; // Default height
    }
    #endif

    std::cout << subConsoleStart;

    // Calculate sub-console dimensions
    subConsoleWidth = consoleWidth;
    subConsoleHeight = consoleHeight;

    subConsoleStart = "\033[0;0H";
    subConsoleEnd = "\033[" + std::to_string(subConsoleHeight) + ";" + std::to_string(subConsoleWidth) + "H";

    // Calculate escape sequences for cursor positioning
    if (maze == nullptr) {
        mazeWindowWidth = Config::MAZE_WIDTH;
        mazeWindowHeight = Config::MAZE_HEIGHT;
    } else {
        const auto& displayGrid = maze->getDisplayGrid();

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

    // set size for help strings
    helpStrings.resize(helpWindowHeight);
    //init help strings with empty strings
    for (int i = 0; i < helpStrings.size(); ++i) {
        if (helpStrings[i].empty()) { // to avoid deleting existing help strings when resizing
            helpStrings[i] = "";
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
    // Draw top border
    std::cout << subConsoleStart;
    for (int i = 0; i < subConsoleWidth; ++i) {
        std::cout << "*";
    }
    // Draw side borders
    for (int i = 1; i < subConsoleHeight - 1; ++i) {
        std::cout << "\033[" << i + 1 << ";1H*";
        std::cout << "\033[" << i + 1 << ";" << subConsoleWidth << "H*";
    }
    // Draw bottom border
    std::cout << "\033[" << subConsoleHeight << ";1H";
    for (int i = 0; i < subConsoleWidth; ++i) {
        std::cout << "*";
    }
    std::cout.flush();
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

    // check positioning
    std::cout << subConsoleStart << "S";
    std::cout << subConsoleEnd << "S";
    std::cout << mazeWindowStart << "M";
    std::cout << mazeWindowEnd << "M";
    std::cout << inputLineStart << "I";
    std::cout << helpWindowFirstLine << "H";
    std::cout << helpWindowLastLine << "H";

    clearInputLine();
}

void Renderer_2d::clearScreen() {
    std::cout << "\033[0;0H" << "\033[2J"; // Clear screen and move cursor to home position
    std::cout.flush();
}

void Renderer_2d::addHelpString(const std::string& helpString) {
    // move all messages left (remove first, add new at the end)
    for (int i = 0; i < helpStrings.size() - 1; ++i) {
        helpStrings[i] = helpStrings[i + 1];
    }
    helpStrings[helpStrings.size() - 1] = helpString;
    notify();
}

void Renderer_2d::renderLoop() {
    while (running) {
        std::unique_lock<std::mutex> lock(renderMutex);
        renderCondition.wait(lock, [] {
            return Render2dFlags::clearScreen || Render2dFlags::resetConsole || Render2dFlags::updateCollectedItems || Render2dFlags::drawMaze || Render2dFlags::drawInputLine || Render2dFlags::showHelpInstructions;
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

        if (Render2dFlags::drawMaze) {
            drawMaze();
            clearInputLine();
            Render2dFlags::drawMaze = false;
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
    }
}