#include "universe.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    if (rows < 0 || cols < 0) {
        return false;
    }

    Universe *u = (Universe *) calloc(1, sizeof(Universe));

    u->rows = rows;
    u->cols = cols;
    u->toroidal = toroidal;

    //given in asgn pdf
    u->grid = (bool **) calloc(rows, sizeof(bool *));
    for (uint32_t r = 0; r < rows; r += 1) {
        u->grid[r] = (bool *) calloc(cols, sizeof(bool *));
    }
    return u;
}

void uv_delete(Universe *u) {
    //free memory
    for (uint32_t r = 0; r < u->rows; r++) {
        free(u->grid[r]);
    }
    free(u->grid);
    free(u);
}

uint32_t uv_rows(Universe *u) {
    //This function returns the number of rows
    return u->rows;
}

uint32_t uv_cols(Universe *u) {
    //returns the number of columns
    return u->cols;
}

void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    //This function simply marks the cell at row r and column c as alive.
    //bool for dead or alive
    //check bounds
    if ((r < u->rows) && (c < u->cols) && (r >= 0) && (c >= 0)) {
        u->grid[r][c] = true;
    }
}

void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    //This function marks the cell at row r and column c as dead.
    //check bounds
    if ((r < u->rows) && (c < u->cols) && (r >= 0) && (c >= 0)) {
        u->grid[r][c] = false;
    }
}

bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    //This function returns the value of the cell at row r and column c.
    //check bounds
    if ((r < u->rows) && (c < u->cols) && (r >= 0) && (c >= 0)) {
        return u->grid[r][c];
    } else {
        return false;
    }
}

bool uv_populate(Universe *u, FILE *infile) {
    //create loop and call fscanf
    //check bounds
    //if out of bounds - return false and error message
    //else - return true (it was successful)

    uint32_t row;
    uint32_t column;

    while (fscanf(infile, "%u%u", &row, &column) == 2) {
        if ((row > u->rows) || (column > u->cols) || (row < 0) || (column < 0)) {
            return false;
        }
        uv_live_cell(u, row, column);
    }
    return true;
}

uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    //check if toroidal
    //This function returns the number of live neighbors adjacent to the cell at row r and column c.

    uint32_t counter = 0;
    //these for loops will apply the math to look at neighbors
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            //skip 0
            if (i == 0 && j == 0) {
                continue;
            }
            int rowposition = r + i;
            int colposition = c + j;
            if (u->toroidal) {
                //modulus hint from Professor Long and others on Discord 2/7
                rowposition = (rowposition + u->rows) % u->rows;
                colposition = (colposition + u->cols) % u->cols;

                //tip for int32_t taken from TA on thursday 2/9
            } else if ((rowposition >= (int32_t) u->rows || colposition >= (int32_t) u->cols
                           || rowposition < 0 || colposition < 0)) {
                continue;
            }
            if (uv_get_cell(u, rowposition, colposition)) {
                counter++;
            }
        }
    }
    return counter;
}

void uv_print(Universe *u, FILE *outfile) {
    //print to outfile, use fputc() or fprintf()
    // "o" for alive
    // "." for dead

    if (u->toroidal == false) {
        for (uint32_t i = 0; i < u->rows; i++) {
            for (uint32_t j = 0; j < u->cols; j++) {
                if (uv_get_cell(u, i, j)) {
                    fputc('o', outfile);
                } else {
                    fputc('.', outfile);
                }
            }
            fputc('\n', outfile);
        }
    }
}
