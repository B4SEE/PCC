#pragma once
#include <vector>
#include <utility>

/**
 * @class Maze
 * @brief Represents a maze with various difficulty levels, items, and player position.
 * 
 * The Maze class allows for the creation and manipulation of a maze. It supports
 * different difficulty levels, item generation, and player positioning.
 * 
 * Each maze has one door that can be opened by collecting all items in the maze.
 * When the player collects all items, the door opens, and the player can exit the maze to get to the next maze.
 * When a specific number of mazes are completed, the game is won (can be set in the game configuration).
 */
class Maze {
public:
    /**
     * @enum Maze::Difficulty
     * @brief Defines the difficulty levels for the maze.
     * 
     * The difficulty levels determine the complexity of the maze.
     * - EASY: Simple maze with fewer dead ends.
     * - MEDIUM: Moderate maze with a balanced number of dead ends.
     * - HARD: Complex maze with many dead ends.
     */
    enum Difficulty { EASY, MEDIUM, HARD };

    /**
     * @fn Maze::Maze(int width, int height, Difficulty difficulty)
     * @brief Constructs a Maze object with specified dimensions and difficulty.
     * @param width The width of the maze.
     * @param height The height of the maze.
     * @param difficulty The difficulty level of the maze.
     */
    Maze(int width, int height, Difficulty difficulty);

    void displayMaze(const std::vector<std::vector<int>>& grid, bool raw = false);

    /**
     * @fn const std::vector<std::vector<char>>& Maze::getMaze() const
     * @brief Retrieves the current state of the maze.
     * @return A constant reference to a 2D vector representing the maze map.
     */
    const std::vector<std::vector<int>>& getMaze() const;

    /**
    * @fn void Maze::setPlayerPosition(int x, int y)
    * @brief Sets the player's position within the maze.
    * @param x The x-coordinate of the player's position.
    * @param y The y-coordinate of the player's position.
    */
    void setPlayerPosition(int x, int y);

    /**
     * @fn std::pair<int, int> Maze::getPlayerPosition() const
     * @brief Retrieves the player's current position in the maze.
     * @return A pair of integers representing the x and y coordinates of the player's position.
     */
    std::pair<int, int> getPlayerPosition() const;

private:
    /**
     * @fn void Maze::generateItems(int itemCount)
     * @brief Generates a specified number of items within the maze.
     * @param itemCount The number of items to generate in the maze.
     */
    void generateItems(int itemCount);

    /**
     * @fn void Maze::createMaze(Difficulty difficulty)
     * @brief Creates the maze based on the specified difficulty level.
     * @param difficulty The difficulty level to use for maze creation.
     */
    void createMaze(Difficulty difficulty);

    void createEasyMaze();
    void createMediumMaze();
    void createHardMaze();

    /**
     * @fn void Maze::placeItems(int itemCount)
     * @brief Places a specified number of items within the maze.
     * @param itemCount The number of items to place in the maze.
     */
    void placeItems(int itemCount);

    int width;
    int height;
    std::vector<std::vector<int>> mazeMap;
    std::vector<std::vector<int>> displayGrid;
    std::vector<std::pair<int, int>> itemsPosition;
    std::pair<int, int> playerPosition = std::make_pair(1, 1);
};