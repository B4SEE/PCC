#pragma once

#include <thread>
#include <atomic>
#include <functional>
#include <chrono>
#include <string>
#include <condition_variable>

/**
 * @class InputHandler
 * @brief The InputHandler class is responsible for handling user input.
 */
class InputHandler {
public:
    /**
     * @typedef KeyPressHandler
     * @brief The KeyPressHandler type is a callback function for handling key presses.
     */
    using KeyPressHandler = std::function<void(const std::string&)>;

    /**
     * @fn InputHandler
     * @brief The constructor for the InputHandler class.
     * @param requireEnter Whether or not to require the user to press enter after input.
     * @param handler The callback function to handle key presses.
     */
    InputHandler(bool requireEnter = true, KeyPressHandler handler = nullptr);

    /**
     * @fn ~InputHandler
     * @brief The destructor for the InputHandler class.
     */
    ~InputHandler();

    /**
     * @fn start
     * @brief Starts the input handler.
     */
    void start();

    /**
     * @fn stop
     * @brief Stops the input handler.
     */
    void stop();

    /**
     * @fn setRequireEnter
     * @brief Sets whether or not to require the user to press enter after input.
     * @param requireEnter Whether or not to require the user to press enter after input.
     */
    void setRequireEnter(bool requireEnter);

    /**
     * @fn getRequireEnter
     * @brief Gets whether or not the user is required to press enter after input.
     * @return Whether or not the user is required to press enter after input.
     */
    bool getRequireEnter() const;

private:
    /**
     * @fn run
     * @brief Starts the input handler thread.
     */
    void run();

    /**
     * @fn handleKeyPress
     * @brief Handles a key press.
     * @param key The key that was pressed.
     */
    void handleKeyPress(char key);

    std::mutex inputMutex;

    std::thread inputThread;
    std::atomic<bool> running;
    std::condition_variable inputCondition;

    bool requireEnter;
    std::function<void(const std::string&)> callback;

    std::chrono::steady_clock::time_point lastInputTime;
    const std::chrono::milliseconds inputCooldown{300};

    KeyPressHandler keyPressHandler;

    std::string userInput;


};