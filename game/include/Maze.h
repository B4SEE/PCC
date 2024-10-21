#pragma once

#include <vector>
#include <utility>
#include <atomic>
#include <condition_variable>
#include <thread>

/**
 * @enum Difficulty
 * @brief Represents the difficulty levels of a maze.
 * 
 * The Difficulty enum class represents the different difficulty levels that a maze can have.
 * Each difficulty level has a different maze generation algorithm and item count.
 * 
 * EASY: Uses Depth-First Search algorithm for maze generation.
 * MEDIUM: Uses Kruskal's algorithm for maze generation.
 * HARD: Uses Wilson's algorithm for maze generation.
 */
enum class Difficulty { 
    EASY, 
    MEDIUM, 
    HARD 
};

/**
 * @class Maze
 * @brief Represents a maze with various difficulty levels, items, and player position.
 * 
 * The Maze class allows for the creation and manipulation of a maze. It supports
 * different difficulty levels, item generation, and player positioning.
 * 
 * Each maze has one exit that can be opened by collecting all items in the maze.
 * When the player collects all items, the exit is opened and the player can move to the next maze.
 * When a specific number of mazes are completed, the game is won (can be set in the game configuration).
 */
class Maze {
public:
    /**
     * @fn Maze::Maze(int width, int height, Difficulty difficulty)
     * @brief Constructs a Maze object with specified dimensions and difficulty.
     * @param width The width of the maze.
     * @param height The height of the maze.
     * @param difficulty The difficulty level of the maze.
     */
    Maze(int width, int height, Difficulty difficulty);

    /**
     * @fn Maze::~Maze()
     * @brief Destroys the Maze object.
     */
    ~Maze();

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

    /**
     * @fn void Maze::getExitPosition() const
     * @brief Retrieves the exit position of the maze.
     * @return A pair of integers representing the x and y coordinates of the exit position.
     */
    std::pair<int, int> getExitPosition() const;

    /**
     * @fn void Maze::getItemsCollected() const
     * @brief Retrieves the number of items collected by the player.
     * @return The number of items collected by the player.
     */
    int getItemsCollected() const;

    /**
     * @fn void Maze::getTotalItems() const
     * @brief Retrieves the total number of items in the maze.
     * @return The total number of items in the maze.
     */
    int getTotalItems() const;
 
    /**
     * @fn void Maze::getDisplayGrid() const
     * @brief Retrieves the current state of the maze for display purposes.
     * @return A constant reference to a 2D vector representing the maze map for display.
     */
    const std::vector<std::vector<int>>& getDisplayGrid() const;

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

    /**
     * @fn void Maze::createWilsonMaze()
     * @brief Creates a maze using Wilson's algorithm.
     */
    void createWilsonMaze();

    /**
     * @fn void Maze::createKruskalMaze()
     * @brief Creates a maze using Kruskal's algorithm.
     */
    void createKruskalMaze();

    /**
     * @fn void Maze::createDepthFirstMaze()
     * @brief Creates a maze using Depth-First Search algorithm.
     */
    void createDepthFirstMaze();

    /**
     * @fn void Maze::endMazeGeneration()
     * @brief Ends the maze generation process: sets the exit position, player position, and generates items.
     */
    void endMazeGeneration();

    int width;
    int height;

    std::vector<std::vector<int>> mazeMap;
    std::vector<std::vector<int>> displayGrid;
    std::vector<std::pair<int, int>> itemsPosition;

    int itemsCollected = 0;
    int totalItems = 0;
    
    std::pair<int, int> playerPosition = std::make_pair(1, 1);
    std::pair<int, int> exitPosition;
};