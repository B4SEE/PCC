#include <catch2/catch.hpp>
#include "Renderer_2d.h"
#include <iostream>

TEST_CASE("Renderer_2d initialization", "[Renderer_2d]") {
    Renderer_2d renderer;

    SECTION("Default values") {
        REQUIRE(renderer.getIsRunning() == false);
        REQUIRE(renderer.getConsoleWidth() == 0);
        REQUIRE(renderer.getConsoleHeight() == 0);
        REQUIRE(renderer.getSubConsoleWidth() == 0);
        REQUIRE(renderer.getSubConsoleHeight() == 0);
        REQUIRE(renderer.getMazeWindowWidth() == 0);
        REQUIRE(renderer.getMazeWindowHeight() == 0);
        REQUIRE(renderer.getHelpWindowHeight() == 0);
        REQUIRE(renderer.getMaxHelpLineLength() == 0);
    }
}

TEST_CASE("Renderer_2d start and stop", "[Renderer_2d]") {
    Renderer_2d renderer;

    renderer.start();
    REQUIRE(renderer.getIsRunning() == true);

    renderer.stop();
    REQUIRE(renderer.getIsRunning() == false);
}