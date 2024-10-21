#pragma once

/**
 * @brief Class that contains all the flags that are used to control the rendering of the 2D maze.
 */
class Render2dFlags {
public:
    static bool clearScreen;
    static bool resetConsole;
    static bool drawMaze;
    static bool drawInputLine;
    static bool showLogs;
    static bool showHelpInstructions;
    static bool helpInstructionsAreVisible;
    static bool updateCollectedItems;
    static bool showAllMaze;
    static bool printWinMessage;
    static bool redrawPlayer;

    /**
     * @brief Resets all the flags to their default values.
     */
    void reset();
};