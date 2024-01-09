# Endless Runner Game: Run to you
# Project_CS162_APCS22: Data Structure Visulization

## Introduction

In this project, Object-Oriented Programming concepts are applied to build an endless runner
game called Run to you. This game is inspired by the famous game - Google’s Dinosaur Game,
and some other endless runner games such as: Temple Run, Subway Surfers, ... To complete this
project, fundamental knowledge in areas such as file handling, processes, basic data structures, and object-oriented programming, ... are needed.

## Demo Video

[This is the Youtube link](https://youtu.be/zW_lRESILyQ)

### Running

1. Download or clone the repository to your local machine.
2. Navigate to the directory of the project.
3. Run the executable file through terminal:

```console
./bin/main.exe
```

## Game play

The mission of this game is to help the main character to meet her crush. At first, when entering the game, the player will automatically run forward. There are obstacles and rewards on the road. The player uses up arrow key to jump, tries to avoid obstacles and get rewards. When the player collides with an obstacle, he/she will lose a life. Losing all lives makes the game over. The program will prompt the player to choose to continue (the program will reset the game data to its initial state) or to exit the game. When the player successfully crosses all the obstacles, they will advance to the next level. There are 3 levels. The last one is an endless level so that player can play until he/she died. The game’s difficulty is determined by the speed of the player.

## Features

1. Basic requirements:
   - Fixed screen
   - Mouse
   - Multi-threading
2. Menu with several options:
    - Play new game or the last saving game
    - Music setting
    - High score
    - More information about the game
3. Pause game:
    - Continue playing
    - Restart
    - Save and quit
    - Quit only
4. Save and load game
5. Game levels: 3 levels and endless mode
6. Game screens:
    - Introduction screen
    - Notification screen
    - Highscore screen
    - More information screen
    - 3 screens for 3 levels
    - Countdown screen before playing the last game
    - 2 level-up screens
7. Main character resources
8. Lives (energy)
9. Obstacles: 5 types of obstacles
    - Rock
    - Bunch of rocks
    - Tree
    - Low-flying bird
    - High-flying bird
10. Rewards - hearts: Collect ”heart” item to restore energy
11. Collision effects:
    - Stop running
    - Red color
    - Collision sound

## Languages and Technologies

- C++
- Git: a distributed version control system to manage source codes
- Make: a build automation tool that builds executable programs and libraries from source code by reading files called makefiles which specify how to derive the target program.
- SFML (Simple and Fast Multimedia Library): a cross-platform software development library designed to provide a simple application programming interface (API) to various multimedia components in computers.

## Credits

This program was created by Vi Nguyen-Thao Trinh (poteitou) as a project for CS202 - Programming Systems course at VNUHCM-University of Science.
