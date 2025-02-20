# Game developer test for King (c++)

## Purpose 
Let's pretend that at King we have this old game with legacy code that we have decided to give a second life. In order to
make it possible, our designers want to add new cool features to improve the player experience and make the game more
fun to play, while ensuring that its codebase will last for many years to come. Here is where your exercise starts :)


### What is the game you will be working on?
The game is a standard [Arkanoid](https://en.wikipedia.org/wiki/Arkanoid) clone. The game is composed of several levels 
in a game area with some bricks to be destroyed, a paddle and a ball.

The objective of the player is to destroy all bricks reflecting the ball with the paddle, which in turn will give you 
points with each bounce and brick destroyed. If the ball falls under the paddle you will lose one ball. Once you lose 
the third ball the game ends. 

The winning condition is to clear all levels of the game.

The starting point is a very basic version of the game, in legacy code, that does not contain any power-ups. There are 
some cheat commands to speed up the development process, like:
* Press W key: Finish the level.


## Your Mission
The first feature this peer wants you to include is a power-up that makes the paddle larger for 10 seconds. The power-up
spec is as follows:

1. The power-up should drop from a new type of brick (pink) when it is destroyed.

2. The power-up item falls down from the brick original position, to be collected by the paddle. Then it triggers. Should not collide or bounce with anything else.

3. When the power-up item is triggered by the paddle, it enlarges the size of the paddle for 10 seconds. If it falls 
under the paddle it should be removed and do nothing. After the 10 seconds have passed or in case the player loses a 
ball, the paddle should return to the normal size.

4. If the paddle collects again the same type of power-up while still under the effect of it, just add 10 more 
seconds to the current countdown of the ongoing enlargement effect.

An artist has already prepared the sprites for the pink brick, the "_large_" version of the paddle, and for the new
power-up.

Browsing the code you will find out that from the second level of the game and onwards, there is a new brick type in the
Level definition that is not supported by the code. Use this new brick type as a starting point for the new pink brick
that drops the power-up.


### What your starting point is:
* A simple legacy engine developed on top of SDL2
* A build script based on premake
* Premake binaries
* Art Assets and some Levels
* The _Game code to be modified by you_

We are providing all frameworks that you need to be able to generate the project in either OSX (`generate.command`), Windows (`generate.bat`) or Linux (`generate.sh`), but you can 
use any OS you like, as long as you can extend the premake scripts to generate for the IDE/toolchain of your choice, if 
not directly supported. The Linux support is still in beta and you might need to install some dependencies (glm, glew, SDL2, pkgconfig, gcc/clang, zip).

## Delivery
We expect you to deliver a zip file with all the source code and new assets that you have developed (if any). We provide scripts to generate 
the zip file on OSX (`package.command`), Windows (`package.bat`) and Linux (`package.sh`). We should be able to build the project without any big issues. 
If there is something that we need to know about, please provide it into the **_reviewers.md_** file.

There is no need to make sure that the project compiles on all platforms, but please make us aware of which platform you 
worked on.

### Evaluation
Your test will be evaluated on the following criteria:

### Implementation criteria
* Program correctness (Both release and debug build)
* Code structure and design choices
* Readability, changeability, simplicity and testability
* Good memory and object lifetime management

### Constraints:
* The solution should be open enough to be feasible to add, modify or remove a power-up in an easy way and should be
generic enough to allow future development of many other new types of power-ups that may affect different elements of the game. 
* You are free to refactor and change any existing code - whatever you think makes your task easier and whatever you think makes 
the most sense can be done at your discretion. However you will only be evaluated on the changes made in the _**src**_ directory.

### Timeframe
The test was made to take no more than 8 hours to complete but you have one week to complete it. Use your time as you wish.
