# Maze Game with ASCII View

## Project Description

This project is a semester assignment for the PCC (Programming in C/C++) winter course 2024/2025. The game features an 'infinitely' generating maze with an ASCII view. The objective of the game is to collect all items in the maze to open the door and exit. Players must complete a specific number of mazes to win the game.

## Libraries Used
nlohmann/json - [https://github.com/nlohmann/json](https://github.com/nlohmann/json)

## Game Controls

- **Movement**: Use `W`, `A`, `S`, `D` keys to move the player.
- **Toggle Command Line Mode**: Press `L` to toggle command line mode.
- **Exit Game**: Type `exit` to quit the game.
- **Reset Console**: Type `reset` to reset the console if it is 'accidentally' broken or resized.
- **Show Help**: Type `help` to display the help message.
- **Change Difficulty**: Type `difficulty EASY`, `difficulty MEDIUM`, or `difficulty HARD` to change the difficulty level.
- **Show Maze**: Type `show` to display the entire maze.
- **Restart Game**: Type `restart` to restart the game.

## Game Settings

The game can be configured using the `config.json` file. Below are the available settings:

- **MAZE\_WIDTH**: Width of the maze.
- **MAZE\_HEIGHT**: Height of the maze.
- **SCREEN\_WIDTH**: Width of the game screen.
- **SCREEN\_HEIGHT**: Height of the game screen.
- **MIN\_HELP\_WINDOW\_HEIGHT**: Minimum height of the help window.
- **EXPLORE\_RADIUS**: Radius of exploration around the player e.g. visible area.
- **COMPLETED\_MAZES\_TO\_WIN**: Number of mazes to complete to win the game.
- **MAX\_ITEMS\_IN\_MAZE\_SECTION**: Maximum number of items in a maze section.
- **MIN\_ITEMS\_IN\_MAZE\_SECTION**: Minimum number of items in a maze section.
- **MILLISECONDS\_INPUT\_DELAY**: Delay in milliseconds for input processing.

Please use adequate values for the settings to avoid breaking the game. Default values are:
    
    ```json
    {
        "MAZE_WIDTH": 90,
        "MAZE_HEIGHT": 20,
        "SCREEN_WIDTH": 120,
        "SCREEN_HEIGHT": 30,
        "MIN_HELP_WINDOW_HEIGHT": 5,
        "EXPLORE_RADIUS": 5,
        "COMPLETED_MAZES_TO_WIN": 3,
        "MAX_ITEMS_IN_MAZE_SECTION": 15,
        "MIN_ITEMS_IN_MAZE_SECTION": 5,
        "MILLISECONDS_INPUT_DELAY": 300
    }
    ```

Note:
- The `config.json` is located in the `../resources` directory. There are other settings that are not used yet. Key 'L' and 'H' are reserved.
- Actual maze width and height can differ from the specified values in the `config.json` file due to the generation algorithm.
- If game encounters invalid maze width or height, it will set the values to the closest valid values.

### Key Bindings

Default key bindings can be set to alphanumeric characters (0-9a-zA-Z, excluding special characters) only:

- **MOVE\_FORWARD**: `W`
- **MOVE\_BACKWARD**: `S`
- **MOVE\_LEFT**: `A`
- **MOVE\_RIGHT**: `D`

## Installation Instructions

To set up the development environment, follow these steps:

1. Clone the repository:
    ```sh
    git clone https://github.com/B4SEE/PCC.git
    cd PCC
    ```

2. Install dependencies:
    ```sh
    # Example for a C++ project using CMake
    mkdir build
    cd build
    cmake ..
    make
    ```


## How to Run

To run the game, execute the following command:

```sh
./PCCGame [options]

Options:
  -h, --help      Show help message
  -v, --version   Show version information
```

## Known Issues

- **Input speed**: If the input speed is too fast, the game may not process the input correctly and may break the console. Use 'reset' to redraw the console.
- **Console resizing**: Resizing the console may break the console. Use 'reset' to redraw the console.
- **Border erase**: The border may be erased if the user presses the 'Enter' key. Use 'reset' to redraw the console.
- **Console size**: If the console size is too small, the game may not display correctly. If the console is too large, the game will redraw the console slower and chances of breaking the console with fast input are higher.
