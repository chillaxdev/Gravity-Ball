# Gravity-Ball
Gravity ball is a game which is inspired from [Gravity Guy](http://www.y8.com/games/gravity_guy). This game compiles only on Borland discontinued Turbo C++ compiler. I've seen many schools and colleges (especially India) still using this ancient compiler. When I googled for turbo c++ projects. I didn't find any good Object oriented programming with this compiler. Most students are often confused with OOP (assuming students are very clear about procedural programming). Hence I made it public with the only intention to do knowledge sharing.

In case you are going to use this for your project. Just keep these things in mind. First of all analyse this project completely. Don't simply submit this project as it is. Instead create your own from scratch. Learn the beauty of object oriented programming. Learn the way I've merged classes. Learn the way I’ve done parallel programming. Learn the way I've formatted the code. Learn the way I've declared simple, camelCased, and PascalCased variables. You have to think, plan, and analyse before you create any classes. You have to think only in terms of objects. I’ve made as many comments as possible. But its upto you, good luck!

This game doesn't use any 3rd party 2d game engines. Long back we didn't have any open source game engines (Every game company had their own game engines). So I had to create my own small 2d physics engine. Okay now lets get started with the project.

## Getting Started

1. Set up your IDE (Integrated development environment) first. You can download the Turbo C++ 4.0 compiler for Windows 7/8/10 32bit program (you will require dosbox to run 16bit files) from [here](https://drive.google.com/file/d/0B_LCZZjtZ3WPUFVxanFNM2tNWk0/view?usp=sharing).
2. Now download the project to your computer. Click on Clone/Download button from this page in case you don't have GIT.
3. Now extract and paste the contents of Project folder inside the BIN directory of your Turbo C++ IDE.
4. Now open the Turbo C++ IDE and open GBALL.CPP
5. Use the linker from the Menu and enable BGI Graphics.
6. Now time to execute (CTRL + F9)

You can use your own IDE like Atom, Sublime Text, etc and cross link with this command line 16bit compiler and execute the program. There are infinitely many ways. I'm taking this ancient IDE for example because many colleges/schools are scared about other IDE's hahahaha (I'm still curious about it). They won't allow us to use other IDE's. 

## Note

There might be small glitches in the game. Please feel free to report in the Issues and I'll get it fixed ASAP. Actually I've done this project for my boardz in the last minute (1 week I think) hahahaha. One more thing, this ancient compiler doesn't allow us to allocate many heap memory. So I was forced to use OOP with normal class instances. I've still used Heap for points system. Hence there is a limit for storing points. Finally, it isin't the best efficient code but I'll make it better in the future versions.
