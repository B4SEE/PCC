#pragma once

/**
 * @class Check
 * @brief A utility class for performing various checks.
 *
 * The Check class provides static methods to perform checks related to keybindings and constants.
 */
class Check {
public:
    /**
     * @fn checkKeybindings
     * @brief Checks the keybindings.
     *
     * This method verifies the current keybindings and ensures they are correctly set.
     *
     * @return true if the keybindings are correct, false otherwise.
     */
    static bool checkKeybindings();

    /**
     * @fn checkConst
     * @brief Checks the constants.
     *
     * This method verifies the constants used in the application to ensure they are correctly defined.
     *
     * @return true if the constants are correct, false otherwise.
     */
    static bool checkConst();
};