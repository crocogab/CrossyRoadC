# Crossy Road - AI 

This project is a reimplementation of the famous **Crossy Road** game, available in two versions:
- A playable terminal version (TUI) using the `ncurses` library
- A graphical user interface (GUI) version

![Crossy Road](assets/Menu.png)

## Objectives

- Reproduce the mechanics of **Crossy Road** in both command-line and graphical environments
- Use `ncurses` to manage display and interactions in the terminal
- Implement a modern graphical interface with SDL2
- Develop a modular architecture that allows easy game extension
- Create an AI to play automatically

## Features

- Real-time display in terminal or graphical interface
- Player movement (forward, backward, left, right)
- Road and obstacle management
- Dynamic screen refresh
- Keyboard controls (arrow keys)

### Game Mechanics
- Different terrain types (grass, road, water)
- Variety of obstacles (trees, rocks, cars, trucks, trains)
- Progressive scoring system
- Increasing difficulty with score
- Collision detection
- Movement animations

### GUI-specific Version
- 3D isometric graphical rendering

![Crossy Road](assets/Crossy_road.png)
- Sprites for all game elements
- Smooth animations
- Score and statistics display
- Debug mode to visualize hitboxes and technical data
![Debug](assets/debug1.png)
![Debug](assets/debug2.png)

In debug mode, it's possible to:
- Enter God Mode (F1)
- Display the movement grid (F2)
- Display debug information (F3)
- Show hitboxes (F4)
- Slow down time (F5)
- Speed up time (F6)
- Display obstacle positions (F7)
- Activate AI (F8)
- Show ground hitboxes (F9)
- Display AI path to follow (F10)

# How to Play

- Use arrow keys to move
- Avoid obstacles and vehicles
- Cross rivers by jumping on logs or lily pads
- Progress as far as possible to increase your score
- The game speeds up as your score increases

## Installation

### Prerequisites

Make sure you have the following packages installed:

```bash
sudo apt update
sudo apt install libncurses-dev build-essential
# For GUI version
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libjson-c-dev
```

### Compilation

Clone the project and compile with make:
```bash
git clone https://gibson.telecomnancy.univ-lorraine.fr/projets/2425/ppii-s6/ppii-s6-grp-26.git
cd ppii-s6-grp-26
make
```

### Tests

Run tests:
```bash
make run_tests
```

### Clean the project

Clean compilation files:
```bash
make clean
```

## Execution

### Launch TUI version

```bash
git checkout TUI # go to TUI-specific commit
make clean
make run_tui
```

### Launch GUI version

```bash
git checkout main
make clean
make run_gui
```

## Progressive Difficulties

The game adapts to the player's score with several difficulty levels:
- **Easy** (beginning)
- **Normal** (score > 10)
- **Medium** (score > 25)
- **Hard** (score > 50)
- **Expert** (score > 100)

At each level, the following parameters are modified:
- Number and speed of vehicles
- Train frequency
- Water crossing complexity
- Obstacle density

## Project Structure

```bash
.
├── assets
│   ├── Crossy_road.png
│   ├── DroidKufi-Regular.ttf
│   ├── editundo.ttf
│   ├── Hardpixel.otf
│   ├── high_scores.json
│   ├── spritesheet_coord.json
│   ├── spritesheet.png
│   ├── ui_spritesheet_coord.json
│   └── ui_spritesheet.png
├── Cahier des charges.xlsx
├── CR
│   ├── 21_05.pdf
│   ├── 27_05.pdf
│   ├── CR 06_05.pdf
│   ├── CR 12_03.pdf
│   ├── CR 13_05.pdf
│   ├── CR 19_03.pdf
│   ├── CR 23_04.pdf
│   ├── CR 26_03.pdf
│   ├── CR 29_04.pdf
│   ├── CR 4_04.pdf
│   ├── CR 5_03.pdf
│   └── Notes
│       ├── 21_05.docx
│       ├── 27_05.docx
│       ├── CR 06_05.docx
│       ├── CR 12_03.docx
│       ├── CR 13_05.docx
│       ├── CR 19_03.docx
│       ├── CR 23_04.docx
│       ├── CR 26_03.docx
│       ├── CR 29_04.docx
│       ├── CR 4_04.docx
│       └── CR 5_03.docx
├── Etat de l'art crossy road.docx
├── include
│   ├── board.h
│   ├── debugKit.h
│   ├── game.h
│   ├── ground.h
│   ├── gui.h
│   ├── ia.h
│   ├── macro.h
│   ├── obstacle.h
│   ├── player.h
│   ├── random_custom.h
│   ├── ttf.h
│   └── UI.h
├── main_gui.c
├── Makefile
├── README.md
├── src
│   ├── board.c
│   ├── game.c
│   ├── ground.c
│   ├── gui.c
│   ├── ia.c
│   ├── obstacle.c
│   ├── player.c
│   ├── random_custom.c
│   ├── ttf.c
│   └── UI.c
└── Tests
    ├── test_board.c
    ├── test_ground.c
    ├── test_gui.c
    ├── test_ia.c
    ├── test_obstacle.c
    ├── test_player.c
    ├── test_random_custom.c
    ├── test_ttf.c
    └── test_UI.c

```

## Contributors

This project was developed as part of the PP2I at Télécom Nancy.
