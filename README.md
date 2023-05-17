#Assignment 3 - I Have a Little Dreidel

This program will simulate a game of dreidel. The user can input a number of various commands to change the game such as how many players play and if they wish to input a seed in order to repeat a previous simulation. These will be explained down below. The program will work without those inputs as there are default options set in place.

## Building
Run the following in order to run the program.

Installs make, works with makefile 

'''
$ sudo apt install make
'''

## Running

First clear compiler files with:

'''
$ make clear
'''

Then compile with:

'''
$ make all
'''


Finally, run play-dreidel with this command in the terminal

'''
./play-dreidel -
'''

Followed by:

p (followed by a number between 2-8, inclusive, to state how many players you want to play)

c (followed by a number between 1-20, inclusive, to state how many coins each player starts out with)

s (followed by a number which will be the seed, used to repeat previous simulations)

v (v is the option to have a message print when a player is eliminated)

For example:

./play-dreidel -p 5

or 

./play-dreidel -v

