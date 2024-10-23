#include <catch2/catch.hpp>
#include "InputHandler.h"

TEST_CASE("InputHandler initialization", "[InputHandler]") {
    InputHandler handler(true, nullptr);

    SECTION("Default values") {
        REQUIRE(handler.getRequireEnter() == true);
    }

    SECTION("Set and get requireEnter") {
        handler.setRequireEnter(false);
        REQUIRE(handler.getRequireEnter() == false);
    }
}

TEST_CASE("InputHandler start and stop", "[InputHandler]") {
    InputHandler handler(false, nullptr);

    handler.start();
    handler.stop();

    REQUIRE(handler.getIsRunning() == false);
}

TEST_CASE("InputHandler handle key press (q)", "[InputHandler]") {
    InputHandler handler(false, nullptr);

    handler.start();
    handler.handleKeyPress('a');

    REQUIRE(handler.getIsRunning() == true);

    handler.handleKeyPress('q');

    REQUIRE(handler.getIsRunning() == false);
}