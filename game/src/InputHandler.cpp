#include "InputHandler.h"
#include <iostream>
#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

InputHandler::InputHandler(bool requireEnter, KeyPressHandler handler)
    : requireEnter(requireEnter), running(false), lastInputTime(std::chrono::steady_clock::now()), keyPressHandler(handler) {}

InputHandler::~InputHandler() {
    stop();
}

void InputHandler::setRequireEnter(const bool requireEnter) {
    InputHandler::requireEnter = requireEnter;
}

bool InputHandler::getRequireEnter() const {
    return requireEnter;
}

void InputHandler::start() {
    running = true;
    inputThread = std::thread(&InputHandler::run, this);
}

void InputHandler::stop() {
    running = false;
    if (inputThread.joinable()) {
        inputThread.join();
    }
}

void InputHandler::run() {
    while (running) {
        char key;
        if (requireEnter) {
            key = std::getline(std::cin, userInput).get();
        } else {
#ifdef _WIN32
            key = _getch();
#else
            key = getchar();
#endif
        }
        handleKeyPress(key);
    }
}

void InputHandler::handleKeyPress(char key) {
    auto now = std::chrono::steady_clock::now();
    if (now - lastInputTime >= inputCooldown) {
        lastInputTime = now;
        if (keyPressHandler) {
            if (requireEnter) {
                keyPressHandler(userInput);
                userInput.clear();
            } else {
                keyPressHandler(std::string(1, key));
            }
        } else {
            // Default behavior
        }
    }
}