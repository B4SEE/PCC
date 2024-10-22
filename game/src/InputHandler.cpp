#include "InputHandler.h"

#include <Config.h>
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

void InputHandler::setInputCooldown(int cooldown) {
    inputCooldown = std::chrono::milliseconds(cooldown);
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
    {
        std::lock_guard<std::mutex> lock(inputMutex);
        running = false;
    }
    inputCondition.notify_all();
    if (inputThread.joinable()) {
        inputThread.join();
    }
}

void InputHandler::run() {
    while (running) {
        std::unique_lock<std::mutex> lock(inputMutex);
        if (!running) break;

        if (requireEnter) {
            if (inputCondition.wait_for(lock, std::chrono::milliseconds(100), [this] { return !running; })) {
                break;
            }
            std::getline(std::cin, userInput);
            handleKeyPress(' ');
        } else {
            #ifdef _WIN32
            if (inputCondition.wait_for(lock, std::chrono::milliseconds(10), [this] { return !running; })) {
                break;
            }
            char key = _getch();
            #else
            if (inputCondition.wait_for(lock, std::chrono::milliseconds(10), [this] { return !running; })) {
                break;
            }
            char key = getchar();
            #endif
            handleKeyPress(key);
        }
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
            if (key == 'q') {
                running = false;
            }
        }
    } else {
        userInput.clear();
    }
}