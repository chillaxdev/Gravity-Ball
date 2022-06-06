# Gravity-Ball

Gravity ball is a 2d game which is inspired from [Gravity Guy](http://www.y8.com/games/gravity_guy) and created from the scratch. This game compiles only on C++ 98. This game doesn't use any 3rd party 2d game engines. I had to create my own small 2d physics engine.

## Features

1. Smooth UI & UX
2. Single Player
3. Multi Player (Upto 4 players) concurrently on the same system 
4. Points & score system
5. Teleports System (Only 1 so far & works only on single player)
6. Leaderboard System
7. Level Editor (Multiple modes available for easy editing)
8. Cheats System

## Getting Started

1. Set up your IDE (Integrated development environment) first. You can download the Turbo C++ 4.0 compiler for Windows 7/8/10 32bit program (you will require dosbox to run 16bit files) from [here](https://drive.google.com/file/d/0B_LCZZjtZ3WPUFVxanFNM2tNWk0/view?usp=sharing).
2. Now download the project to your computer. Click on Clone/Download button from this page in case you don't have GIT.
3. Now extract and paste the contents of Project folder inside the BIN directory of your Turbo C++ IDE.
4. Now open the Turbo C++ IDE and open GBALL.CPP
5. Use the linker from the Menu and enable BGI Graphics.
6. Now time to execute (CTRL + F9)

You can use your own IDE like Atom, Sublime Text, etc by cross-linking it with command line interface of 16bit compiler and execute the program. Feel free to try your own ways.

## Note

I've still used Heap for points system. Hence there is a limit for storing points.


![Screenshot](/Screenshot/Screenshot_17.png?raw=true)

## License

This repository uses [GNU General Public License v3.0](https://raw.githubusercontent.com/shiv-sankar/Gravity-Ball/master/LICENSE). Feel free to download, modify, and redistribute but make sure you link this repo along with yours.
