// src/Renderer_2d.cpp
#include "Renderer_2d.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
#include <chrono>
#include <Config.h>

// max sub console size: 120x30

RenderFlags Renderer_2d::renderFlags;

Renderer_2d::Renderer_2d() : running(false), consoleWidth(0), consoleHeight(0), subConsoleWidth(0), subConsoleHeight(0), maze(nullptr) {}

Renderer_2d::~Renderer_2d() {
    stop();
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

void Renderer_2d::drawMaze() {
    //
}

void Renderer_2d::drawInputLine() {
    //
}

void Renderer_2d::clearInputLine() {
    //
}

void Renderer_2d::redrawMaze() {
    //
}

void Renderer_2d::showHelp() {
    //
}

void Renderer_2d::setMaze(Maze& maze) {
    std::lock_guard<std::mutex> lock(renderMutex);
    this->maze = &maze;
}

void Renderer_2d::renderLoop() {
    while (running) {
        std::unique_lock<std::mutex> lock(renderMutex);
        renderCondition.wait(lock, [this] { return !running || renderFlags.drawMaze || renderFlags.clearScreen || renderFlags.drawInputLine || renderFlags.showHelpInstructions; });

        if (renderFlags.clearScreen) {
            clearScreen();
            renderFlags.clearScreen = false;
        }

        if (renderFlags.drawMaze) {
            // Implement maze drawing logic here
            renderFlags.drawMaze = false;
        }

        if (renderFlags.drawInputLine) {
            // Implement input line drawing logic here
            renderFlags.drawInputLine = false;
        }

        if (renderFlags.showHelpInstructions) {
            // Implement help instructions display logic here
            renderFlags.showHelpInstructions = false;
        }

        lock.unlock();
    }
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

    // Calculate sub-console dimensions
    subConsoleWidth = consoleWidth;
    subConsoleHeight = consoleHeight;

    subConsoleStart = "\033[0;0H";
    subConsoleEnd = "\033[" + std::to_string(subConsoleHeight) + ";" + std::to_string(subConsoleWidth) + "H";

    // Calculate escape sequences for cursor positioning (sub console will have a border '*' character)


    // calculate escape sequences, relative to sub console
    mazeWindowWidth = Config::MAZE_WIDTH;
    mazeWindowHeight = Config::MAZE_HEIGHT;

    helpWindowHeight = subConsoleHeight - 2 - 1 - 2 - mazeWindowHeight;

    mazeWindowStart = "\033[2;2H";
    mazeWindowEnd = "\033[" + std::to_string(mazeWindowHeight + 1) + ";" + std::to_string(mazeWindowWidth + 1) + "H";

    inputLineStart = "\033[" + std::to_string(subConsoleHeight - 2 - helpWindowHeight) + ";2H";

    helpWindowFirstLine = "\033[" + std::to_string(subConsoleHeight - helpWindowHeight) + ";2H";
    helpWindowLastLine = "\033[" + std::to_string(subConsoleHeight - 1) + ";2H";

    // help window relation to maze window:
    // help window is rest of the console, and cannot be less than 5 lines
}

void Renderer_2d::drawSubConsole() {
    calculateSubConsole();
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

    // Draw maze window
    std::cout << mazeWindowStart;
    for (int i = 0; i < mazeWindowHeight; ++i) {
        for (int j = 0; j < mazeWindowWidth; ++j) {
            std::cout << "#";
        }
        std::cout << std::endl;
        std::cout << "\033[1C";
    }

    for (int i = 0; i < subConsoleWidth - 2; ++i) {
        std::cout << "-";
    }

    // Draw input line
    std::cout << inputLineStart;
    for (int i = 0; i < subConsoleWidth - 2; ++i) {
        std::cout << "#";
    }
    std::cout << std::endl;
    std::cout << "\033[1C";

    // Draw separator line
    for (int i = 0; i < subConsoleWidth - 2; ++i) {
        std::cout << "-";
    }

    // Draw help instructions
    std::cout << helpWindowFirstLine;
    for (int i = 0; i < helpWindowHeight; ++i) {
        for (int j = 0; j < subConsoleWidth - 2; ++j) {
            std::cout << "#";
        }
        std::cout << std::endl;
        std::cout << "\033[1C";
    }

    // check positionings
    std::cout << subConsoleStart << "S";
    std::cout << subConsoleEnd << "S";
    std::cout << mazeWindowStart << "M";
    std::cout << mazeWindowEnd << "M";
    std::cout << inputLineStart << "I";
    std::cout << helpWindowFirstLine << "H";
    std::cout << helpWindowLastLine << "H";

    // put cursor at the input line
    std::cout << inputLineStart;
}

void Renderer_2d::clearScreen() {
    // Implement screen clearing logic here
}