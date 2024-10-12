#pragma once

class RenderFlags {
public:
    bool clearScreen = false;
    bool drawMaze = false;
    bool drawInputLine = false;
    bool showLogs = false;
    bool showHelpInstructions = false;
    bool updateCollectedItems = false;

    void reset() {
        clearScreen = false;
        drawMaze = false;
        drawInputLine = false;
        showLogs = false;
        showHelpInstructions = false;
        updateCollectedItems = false;
    }
};