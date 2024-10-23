# Maze Game with ASCII View

## Project Description

This project is a semester assignment for the PCC (Programming in C/C++) winter course 2024/2025. The game features an 'infinitely' generating maze with an ASCII view. The objective of the game is to collect all items in the maze to open the door and exit. Players must complete a specific number of mazes to win the game.

---

## Libraries Used
nlohmann/json - [https://github.com/nlohmann/json](https://github.com/nlohmann/json)

---

## Game Controls

- **Movement**: Use `W`, `A`, `S`, `D` keys to move the player.
- **Help**: Press `H` to toggle help window.
- **Toggle Command Line Mode**: Press `L` to toggle command line mode.

Available commands in command line mode:

| Command             | Description                         |
|---------------------|-------------------------------------|
| `exit`              | Exit the game                       |
| `reset`             | Reset the console                   |
| `help`              | Show help message                   |
| `show`              | Show the entire maze                |
| `restart`           | Restart the game                    |
| `difficulty EASY`   | Change difficulty to EASY (default) |
| `difficulty MEDIUM` | Change difficulty to MEDIUM         |
| `difficulty HARD`   | Change difficulty to HARD           |
| `L`                 | Exit command line mode              |

---

## Game Settings

The game can be configured using the `config.json` file. Below are the available settings:

| Setting Name               | Description                                | Default Value |
|----------------------------|--------------------------------------------|---------------|
| `PLAYER_MOVEMENT_SPEED`    | Speed of player movement                   | 5.0           |
| `MAZE_WIDTH`               | Width of the maze                          | 90            |
| `MAZE_HEIGHT`              | Height of the maze                         | 20            |
| `SCREEN_WIDTH`             | Width of the game screen                   | 120           |
| `SCREEN_HEIGHT`            | Height of the game screen                  | 30            |
| `MIN_HELP_WINDOW_HEIGHT`   | Minimum height of the help window          | 5             |
| `EXPLORE_RADIUS`           | Radius of exploration around the player    | 5             |
| `COMPLETED_MAZES_TO_WIN`   | Number of mazes to complete to win the game| 3             |
| `MAX_ITEMS_IN_MAZE_SECTION`| Maximum number of items in a maze section  | 15            |
| `MIN_ITEMS_IN_MAZE_SECTION`| Minimum number of items in a maze section  | 5             |
| `MILLISECONDS_INPUT_DELAY` | Delay in milliseconds for input processing | 300           |

### Key Bindings

Default key bindings can be set to alphanumeric characters (0-9a-zA-Z, excluding special characters) only:

| Action          | Default Key |
|-----------------|--------------|
| `MOVE_FORWARD`  | `W`          |
| `MOVE_BACKWARD` | `S`          |
| `MOVE_LEFT`     | `A`          |
| `MOVE_RIGHT`    | `D`          |

Please use adequate values for the settings to avoid breaking the game.

Note:
- Use uppercase letters for key bindings. Otherwise, the game will not recognize the key.
- The `config.json` is located in the `../resources` directory. There are other settings that are not used yet. Key 'L' and 'H' are reserved.
- Actual maze width and height can differ from the specified values in the `config.json` file due to the generation algorithm.
- If game encounters invalid maze width or height, it will set the values to the closest valid values.

---

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

---

## How to Run

To run the game, execute the following command:

```sh
./PCCGame [options]

Options:
  --help      Show help message
  --version   Show version information
```

---

## Known Issues

- **Input speed**: If the input speed is too fast, the game may not process the input correctly and may break the console. Use 'reset' to redraw the console.
- **Console resizing**: Resizing the console may break the console. Use 'reset' to redraw the console.
- **Border erase**: The border may be erased if the user presses the 'Enter' key. Use 'reset' to redraw the console.
- **Console size**: If the console size is too small, the game may not display correctly. If the console is too large, the game will redraw the console slower and chances of breaking the console with fast input are higher.