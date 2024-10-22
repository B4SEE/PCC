#include "Renderer_2d.h"
#include <iostream>
#include <Config.h>
#include <sstream>
#include <Game.h>
#include <fstream>

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--help") {
            std::ifstream logHeader("../resources/help.txt");
            if (logHeader.is_open()) {
                std::string line;
                while (std::getline(logHeader, line)) {
                    std::cout << line << std::endl;
                }
                logHeader.close();
            } else {
                std::cerr << "Someone deleted the help file!" << std::endl;
            }
            std::exit(0); // Exit after showing help
        }
        if (arg == "--version") {
            std::cout << "Version 1.0.0" << std::endl;
            std::exit(0); // Exit after showing version
        }
    }

    Game game;
    game.start();
    game.stop();
    std::exit(0);
}