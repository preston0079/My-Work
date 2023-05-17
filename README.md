## Assignment 4 - The Game of Life

This program will simulate The Game of Life. It takes in a variety of inputs and outputs the results into a specified file (optional). Instructions are given below on what can be inputted.

## Building

Run the following in order to run the program:

Installs make, works with makefile
'''
$ sudo apt install make
'''

Input this into terminal right before inputing life.c
'''
make all
'''

## Running

Run life.c with this command in the terminal
'''
./life
'''

Followed by:

-t             Create your universe as a toroidal

-s             Silent - do not use animate the evolution

-n {number}    Number of generations [default: 100]

-i {file}      Input file [default: stdin]

-o {file}      Output file [default: stdout]

-h             Prints out help message
