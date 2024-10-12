// include/Renderer_2d.h
#pragma once

#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>
#include <Maze.h>

#include "Render2dFlags.h"

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
    void drawInputLine();
    void clearInputLine();
    void redrawMaze();
    void showHelp();

    void drawSubConsole();

    void setMaze(Maze& maze);

private:
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

    // Escape sequences (cursor positioning)
    std::string subConsoleStart;
    std::string subConsoleEnd;
    std::string mazeWindowStart;
    std::string mazeWindowEnd;
    std::string inputLineStart; //always one line
    std::string helpWindowFirstLine;
    std::string helpWindowLastLine;

    static RenderFlags renderFlags;
};