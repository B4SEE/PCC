#pragma once

#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>
#include <Maze.h>

/**
 * @class Renderer_2d
 * @brief The Renderer_2d class is responsible for rendering 2D graphics in a separate thread.
 */
class Renderer_2d {
public:
    Renderer_2d();
    ~Renderer_2d();

    void start();
    void stop();

    void drawMaze();
    void drawItemCounter();
    void drawInputLine();
    void clearInputLine();
    void showHelp();

    void drawSubConsole();

    void setMaze(Maze& maze);

    void addHelpString(const std::string& helpString);

    void notify();

private:
    void drawSeparator(const std::string& cursorPosition);
    void drawBorders();
    void renderLoop();
    void setConsoleDimensions();
    void calculateSubConsole();
    void clearScreen();

    std::thread renderThread;
    bool running;
    Maze* maze;

    std::mutex renderMutex;
    std::condition_variable renderCondition;

    // Console dimensions
    int consoleWidth;
    int consoleHeight;
    int subConsoleWidth;
    int subConsoleHeight;

    int mazeWindowWidth;
    int mazeWindowHeight;

    int helpWindowHeight;

    std::string subConsoleStart;
    std::string subConsoleEnd;
    std::string mazeWindowStart;
    std::string mazeWindowEnd;
    std::string itemCounterStart;
    std::string inputLineStart;
    std::string helpWindowFirstLine;
    std::string helpWindowLastLine;

    std::vector<std::string> helpStrings;
};