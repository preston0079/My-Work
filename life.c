#include "universe.h"

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For usleep ()

#define DELAY 50000

#define OPTIONS "tsn:i:o:h"

//help message
void help(void) {
    fprintf(stderr, "SYNOPSIS\n"
                    "    Conway's Game of Life\n"
                    "\n"
                    "USAGE\n"
                    "    ./life tsn:i:o:h\n"
                    "\n"
                    "OPTIONS\n"
                    "    -t             Create your universe as a toroidal\n"
                    "    -s             Silent - do not use animate the evolution\n"
                    "    -n {number}    Number of generations [default: 100]\n"
                    "    -i {file}      Input file [default: stdin].\n"
                    "    -o {file}      Output file [default: stdout]\n");
}

//facilitates the swapping of universes
void swap(Universe **A, Universe **B) {
    Universe *temp = *A;
    *A = *B;
    *B = temp;
}

int main(int argc, char **argv) {

    //defaults declared here
    int opt = 0;

    bool silence = false;

    uint32_t gens = 100;
    bool toroidal = false;

    FILE *input = stdin;
    FILE *output = stdout;

    bool outfile = false;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {

        //-t Specify that the Game of Life is to be played on a toroidal universe.
        case 't': toroidal = true; break;

        //-s Silence ncurses. Enabling this option means that nothing should be displayed by ncurses.
        case 's': silence = true; break;

        //-n generations: Specify the number of generations that the universe goes through. The default number of generations is 100.
        case 'n': gens = strtoul(optarg, NULL, 10); break;

        //-i input : Specify the input file to read in order to populate the universe. By default the input should be stdin.
        case 'i':
            input = fopen(optarg, "r");
            if (input == NULL) {
                printf("./life: option requires an argument -- 'i'\n");
                help();
                return 1;
            }
            break;

        //-o output : Specify the output file to print the final state of the universe to. By default the output should be stdout.
        case 'o':
            output = fopen(optarg, "w");
            if (output == NULL) {
                printf("./life: option requires an argument -- 'o'\n");
                help();
                return 1;
            }
            outfile = true;
            break;

        case 'h': help(); return 1;

        default: help(); break;
        }
    }

    //follow the asgn pdf 6. Specifics

    uint32_t row;
    uint32_t column;

    //initial call fscanf
    fscanf(input, "%u%u", &row, &column);

    //create universe A and B
    Universe *UvA = uv_create(row, column, toroidal);
    Universe *UvB = uv_create(row, column, toroidal);

    //populate A
    if (uv_populate(UvA, input) == false) {
        printf("Failed to populate\n");
        return 1;
    }

    uv_populate(UvA, input);
    fclose(input);

    if (silence == false) {
        initscr(); //Initialize the screen.
        curs_set(FALSE); //Hide the cursor.
    }

    //go through specified generations
    for (uint32_t i = 0; i < gens; i++) {

        // Clear the window.
        if (silence == false) {
            clear();
        }

        //go through rows and columns
        for (uint32_t row = 0; row < uv_rows(UvA); row++) {
            for (uint32_t col = 0; col < uv_cols(UvA); col++) {

                //ncurses
                if (uv_get_cell(UvA, row, col)) {
                    mvprintw(row, col, "o");
                }

                //rules of the game
                if (uv_get_cell(UvA, row, col)
                    && (uv_census(UvA, row, col) == 2 || uv_census(UvA, row, col) == 3)) {
                    uv_live_cell(UvB, row, col);
                } else if (!uv_get_cell(UvA, row, col) && uv_census(UvA, row, col) == 3) {
                    uv_live_cell(UvB, row, col);
                } else {
                    uv_dead_cell(UvB, row, col);
                }
            }
        }
        //clear universe
        for (uint32_t row = 0; row < uv_rows(UvA); row++) {
            for (uint32_t col = 0; col < uv_cols(UvA); col++) {
                uv_dead_cell(UvA, row, col);
            }
        }

        //swap
        swap(&UvA, &UvB);

        if (silence == false) {
            //Refresh the window.
            refresh();

            //Sleep for 50000 microseconds.
            usleep(DELAY);
        }
    }
    // Close the screen.
    if (silence == false) {
        endwin();
    }

    //for -o
    if (outfile == true) {
        uv_print(UvA, output);
        fclose(output);
    }

    //prints the universe when done with animation
    for (uint32_t i = 0; i < uv_rows(UvA); i++) {
        for (uint32_t j = 0; j < uv_cols(UvA); j++) {
            if (uv_get_cell(UvA, i, j)) {
                printf("o");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }

    //delete universe - free memory
    uv_delete(UvA);
    uv_delete(UvB);
    return 0;
}
