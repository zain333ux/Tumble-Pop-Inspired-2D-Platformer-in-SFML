# Tumble Pop Inspired 2D Platformer in C++ SFML

A 2D arcade-style platformer game developed in C++ using SFML. This project is inspired by the classic Tumble Pop gameplay style and includes player movement, jumping, enemies, vacuum-based enemy capture, shooting mechanics, scoring, health, save/load functionality, level selection, player selection, and a leaderboard system.

SFML was used because it provides useful modules for game development, including graphics, window handling, audio, system features, and networking.

## Features

* 2D platformer gameplay
* Player selection system
* Multiple levels
* Platform and slope-based movement
* Ghost and skeleton enemies
* Vacuum-based enemy capture system
* Shoot captured enemies using attack keys
* Health and score system
* Save and load game progress
* Pause menu
* High score leaderboard
* Menu, controls, and level selection screens
* Background music and game assets

## Technologies Used

* C++
* SFML
* Object-Oriented Programming
* File Handling
* Collision Detection
* Sprite Animation
* 2D Game Development

## Controls

| Action                     | Key         |
| -------------------------- | ----------- |
| Move Left                  | Left Arrow  |
| Move Right                 | Right Arrow |
| Jump                       | Up Arrow    |
| Drop Through Platform      | Down Arrow  |
| Vacuum Up                  | Space + W   |
| Vacuum Left                | Space + A   |
| Vacuum Down                | Space + S   |
| Vacuum Right               | Space + D   |
| Shoot One Captured Enemy   | X           |
| Shoot All Captured Enemies | Z           |
| Pause Game                 | P           |
| Exit Game                  | Esc         |

## Gameplay Overview

The player moves across platforms, avoids enemies, and uses a vacuum ability to capture ghosts and skeletons. Captured enemies can be shot back as attacks. The player earns points by capturing and defeating enemies. The game also includes health, save/load functionality, and high score tracking.

## Project Structure

```text
Project Folder/
│
├── Source.cpp
├── Assets/
│   ├── bg.png
│   ├── menu.png
│   ├── player1.png
│   ├── player2.png
│   ├── ghost.png
│   ├── ghostside.png
│   ├── skeleton.png
│   ├── skeletonmove.png
│   ├── skeletonside.png
│   ├── skeltonjump.png
│   ├── block1.png
│   ├── sloperight.png
│   ├── slopeleft.png
│   ├── vaccumright.png
│   ├── vaccumleft.png
│   ├── vaccumup.png
│   ├── vaccumdown.png
│   ├── heart.png
│   ├── mus.ogg
│   ├── times.ttf
│   ├── leaderboard.txt
│   └── save.txt
```

## How to Run

1. Install SFML on your system.
2. Open the project in your C++ IDE.
3. Add `Source.cpp` to your project.
4. Place the `Assets` folder in the same directory as the executable.
5. Link the required SFML libraries.
6. Build and run the project.

## Compile Command Example

```bash
g++ Source.cpp -o TumblePopGame -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
```

Then run:

```bash
./TumblePopGame
```

## Required SFML Libraries

```bash
-lsfml-graphics
-lsfml-window
-lsfml-system
-lsfml-audio
```

## Learning Outcomes

This project helped in understanding:

* Game loop structure
* Player movement and gravity
* Platform collision
* Slope movement
* Sprite animation
* Enemy AI movement
* Vacuum-based attack mechanics
* File handling for save/load
* Leaderboard system
* Menu system creation
* SFML-based 2D game development

## Future Improvements

* Add more levels
* Add boss enemies
* Add sound effects
* Improve enemy AI
* Add power-ups
* Add better animations
* Add multiplayer mode
* Add game settings screen

## Author

Developed as a C++ SFML 2D game project.
