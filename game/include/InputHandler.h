#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <thread>
#include <atomic>
#include <functional>
#include <chrono>
#include <string>

class InputHandler {
public:
    using KeyPressHandler = std::function<void(const std::string&)>;

    InputHandler(bool requireEnter = true, KeyPressHandler handler = nullptr);
    ~InputHandler();

    void start();
    void stop();
    void setRequireEnter(bool requireEnter);
    bool getRequireEnter() const;

private:
    void run();
    void handleKeyPress(char key);

    std::thread inputThread;
    std::atomic<bool> running;
    bool requireEnter;
    std::function<void(const std::string&)> callback;

    std::chrono::steady_clock::time_point lastInputTime;
    const std::chrono::milliseconds inputCooldown{200};

    KeyPressHandler keyPressHandler;

    std::string userInput;
};

#endif // INPUTHANDLER_H