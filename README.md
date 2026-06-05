# Tumble Pop Inspired 2D Platformer in C++ SFML

A 2D arcade-style platformer game developed in C++ using SFML. This project is inspired by the classic Tumble Pop gameplay style and includes player movement, jumping, enemies, vacuum-based enemy capture, shooting mechanics, scoring, health, save/load functionality, level selection, player selection, and a leaderboard system.

## Features

- 2D platformer gameplay
- Player selection system
- Multiple levels
- Platform and slope-based movement
- Ghost and skeleton enemies
- Vacuum-based enemy capture
- Shoot captured enemies using attack keys
- Health and score system
- Save and load game progress
- Pause menu
- High score leaderboard
- Menu, controls, and level selection screens
- Background music and game assets

## Technologies Used

- C++
- SFML
- Object-Oriented Programming
- File Handling
- Collision Detection
- 2D Game Development

SFML is used because it provides modules for graphics, audio, windows, input handling, system features, and networking, making it suitable for multimedia and game development projects. :contentReference[oaicite:0]{index=0}

## Controls

| Action | Key |
|---|---|
| Move Left | Left Arrow |
| Move Right | Right Arrow |
| Jump | Up Arrow |
| Drop Through Platform | Down Arrow |
| Vacuum Up | Space + W |
| Vacuum Left | Space + A |
| Vacuum Down | Space + S |
| Vacuum Right | Space + D |
| Shoot One Captured Enemy | X |
| Shoot All Captured Enemies | Z |
| Pause Game | P |
| Exit Game | Esc |

## Gameplay Overview

The player moves across platforms, avoids enemies, and uses a vacuum ability to capture ghosts and skeletons. Captured enemies can be shot back as attacks. The player earns points by capturing and defeating enemies. The game also includes health, lives, high score tracking, and save/load functionality.

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
│   ├── skeleton.png
│   ├── block1.png
│   ├── mus.ogg
│   ├── times.ttf
│   ├── leaderboard.txt
│   └── save.txt
