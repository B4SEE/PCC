#pragma once

#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>
#include <Maze.h>

/**
 * @class Renderer_2d
 * @brief The Renderer_2d class is responsible for rendering 2D graphics.
 */
class Renderer_2d {
public:
    /**
     * @fn Renderer_2d::Renderer_2d()
     * @brief The constructor for the Renderer_2d class.
     */
    Renderer_2d();

    /**
     * @fn Renderer_2d::~Renderer_2d()
     * @brief The destructor for the Renderer_2d class.
     */
    ~Renderer_2d();

    /**
     * @fn Renderer_2d::start
     * @brief Starts the renderer.
     */
    void start();

    /**
     * @fn Renderer_2d::stop
     * @brief Stops the renderer.
     */
    void stop();

    /**
     * @fn Renderer_2d::drawAllMaze
     * @brief Draws the entire maze.
     */
    void drawAllMaze();

    /**
     * @fn Renderer_2d::drawMaze
     * @brief Draws the maze with only partial visibility. (e.g., fog of war)
     */
    void drawMaze();

    /**
     * @fn printWinMessage
     * @brief Prints a message indicating that the player has won the game.
     */
    void printWinMessage();

    /**
     * @fn pdrawItemCounter
     * @brief Draws the item counter on the screen.
     */
    void drawItemCounter();

    /**
     * @fn drawInputLine
     * @brief Draws the input line on the screen.
     */
    void drawInputLine();

    /**
     * @fn clearInputLine
     * @brief Clears the input line on the screen.
     */
    void clearInputLine();

    /**
     * @fn drawHelp
     * @brief Draws help strings on the screen.
     */
    void showHelp();

    /**
     * @fn drawSubConsole
     * @brief Draws the sub-console on the screen.
     */
    void drawSubConsole();

    /**
     * @fn setMaze
     * @brief Sets the maze to be rendered.
     * @param maze The maze to be rendered.
     */
    void setMaze(std::unique_ptr<Maze> maze);

    /**
     * @fn addHelpString
     * @brief Adds a help string to be displayed on the screen.
     * @param helpString The help string to be displayed.
     */
    void addHelpString(const std::string& helpString);

    /**
     * @fn clearHelpStrings
     * @brief Clears all help strings from the screen.
     */
    void clearHelpStrings();

    /**
     * @fn notify
     * @brief Notifies the renderer to render the screen.
     */
    void notify();

private:
    /**
     * @fn drawSeparator
     * @brief Draws a separator on the screen.
     * @param cursorPosition The position where separator should be drawn.
     */
    void drawSeparator(const std::string& cursorPosition);

    /**
     * @fn drawBorders
     * @brief Draws borders on the screen.
     */
    void drawBorders();

    /**
     * @fn renderLoop
     * @brief The main rendering loop.
     */
    void renderLoop();

    /**
     * @fn setConsoleDimensions
     * @brief Sets the dimensions of the console.
     */
    void setConsoleDimensions();

    /**
     * @fn calculateSubConsole
     * @brief Calculates the dimensions of the sub-console.
     */
    void calculateSubConsole();

    /**
     * @fn clearScreen
     * @brief Clears the screen.
     */
    void clearScreen();

    std::mutex renderMutex;

    std::thread renderThread;
    bool running;
    std::unique_ptr<Maze> maze;

    std::condition_variable renderCondition;

    // Console dimensions
    int consoleWidth;
    int consoleHeight;
    int subConsoleWidth;
    int subConsoleHeight;

    int mazeWindowWidth;
    int mazeWindowHeight;

    int helpWindowHeight;

    // Cursor positions (ANSI escape sequences)
    std::string subConsoleStart;
    std::string subConsoleEnd;
    std::string mazeWindowStart;
    std::string mazeWindowEnd;
    std::string itemCounterStart;
    std::string inputLineStart;
    std::string helpWindowFirstLine;
    std::string helpWindowLastLine;

    // Help strings
    std::vector<std::string> helpStrings;
};