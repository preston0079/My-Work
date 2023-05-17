//-----------------------------------------------------------------------------
//Preston Nguyen
//prhunguy
//pa4
//-----------------------------------------------------------------------------

#include <stdio.h>
#include "Matrix.h"

int main(int argc, char *argv[]) {
    int n, nnza, nnzb, row, col;
    double val;
    Matrix A, B, C, D, E, F, G, H, I, J;
    // Matrix A, B;


    FILE *input_file;
    FILE *output_file;

    // Check command line for correct number of arguments, help recieved from example fileIO.c
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }

    // Open the input file for reading
    input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        fprintf(stderr, "Unable to open file %s for reading\n", argv[1]);
        exit(1);
    }

    // Open the output file for writing
    output_file = fopen(argv[2], "w");
    if (output_file == NULL) {
        fprintf(stderr, "Cannot open output file %s for writing\n", argv[2]);
        exit(1);
    }

    // scan and initialize Matrix 
    fscanf(input_file, " %d %d %d", &n, &nnza, &nnzb);

    // create two matrices A and B of size n
    A = newMatrix(n);
    B = newMatrix(n);


    // from 0 to nnza: 
    //      scan in the rows, columns and values
    //      assign them to matrix A
    for(int i = 1; i <= nnza; i++){
        fscanf(input_file, " %d %d %lf", &row, &col, &val);
        changeEntry(A, row, col, val);
    }
    
    // from 0 to nnzb: 
    //      scan in the rows, columns and values
    //      assign them to matrix B
    for(int i = 1; i <= nnzb; i++){
        fscanf(input_file, " %d %d %lf", &row, &col, &val);
        changeEntry(B, row, col, val);
    }

    fprintf(output_file, "A has %d non-zero entries:\n", NNZ(A));
    // print out the matrix
    printMatrix(output_file, A);

    fprintf(output_file, "\nB has %d non-zero entries:\n", NNZ(B));
    // print out the matrix
    printMatrix(output_file, B);
    
    fprintf(output_file, "\n(1.5)*A =\n");
    // C = multiply everything in A by 1.5
    C = scalarMult(1.5, A);
    printMatrix(output_file, C);

    fprintf(output_file, "\nA+B =\n");
    //D = sum of a and b
    D = sum(A, B);
    printMatrix(output_file, D);

    fprintf(output_file, "\nA+A =\n");
    // E = addition of A twice
    E = sum(A, A);
    printMatrix(output_file, E);

    fprintf(output_file, "\nB-A =\n");
    // F = subtraction of A from B;
    F = diff(B, A);
    printMatrix(output_file, F);

    fprintf(output_file, "\nA-A =\n");
    // G = subtraction of A from A (should be 0)
    G = diff(A, A);
    printMatrix(output_file, G);

    fprintf(output_file, "\nTranspose(A) =\n");
    // H = transpose of A
    H = transpose(A);
    printMatrix(output_file, H);

    fprintf(output_file, "\nA*B =\n");
    // I = product of the two matrices
    I = product(A, B);
    printMatrix(output_file, I);

    fprintf(output_file, "\nB*B =\n");
    // J = product of B twice
    J = product(B, B);
    printMatrix(output_file, J);

    // free all metrices

    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&C);
    freeMatrix(&D);
    freeMatrix(&E);
    freeMatrix(&F);
    freeMatrix(&G);
    freeMatrix(&H);
    freeMatrix(&I);
    freeMatrix(&J);

    // close files
    fclose(input_file);
    fclose(output_file);

    return 0;

    
}


