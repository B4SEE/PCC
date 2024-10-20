#pragma once

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

    void reset();
};