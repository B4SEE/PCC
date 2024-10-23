#include "Render2dFlags.h"

bool Render2dFlags::clearScreen = false;
bool Render2dFlags::resetConsole = false;
bool Render2dFlags::drawMaze = false;
bool Render2dFlags::drawInputLine = false;
bool Render2dFlags::showLogs = false;
bool Render2dFlags::showHelpInstructions = false;
bool Render2dFlags::helpInstructionsAreVisible = false;
bool Render2dFlags::updateCollectedItems = false;
bool Render2dFlags::showAllMaze = false;
bool Render2dFlags::printWinMessage = false;
bool Render2dFlags::redrawPlayer = false;

void Render2dFlags::reset() {
    clearScreen = false;
    resetConsole = false;
    drawMaze = false;
    drawInputLine = false;
    showLogs = false;
    showHelpInstructions = false;
    helpInstructionsAreVisible = false;
    updateCollectedItems = false;
    showAllMaze = false;
    printWinMessage = false;
    redrawPlayer = false;
}