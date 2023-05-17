#include "ss.h"
#include "numtheory.h"
#include "randstate.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gmp.h>

#define OPTIONS "hvi:o:n:"

//help message
void help(void) {
    fprintf(stderr, "SYNOPSIS\n"
                    "   Decrypts data using SS decryption.\n"
                    "   Encrypted data is encrypted by the encrypt program.\n"
                    "\n"
                    "USAGE\n"
                    "    ./decrypt [OPTIONS]\n"
                    "\n"
                    "OPTIONS\n"
                    "   -h              Display program help and usage.\n"
                    "   -v              Display verbose program output.\n"
                    "   -i infile       Input file of data to decrypt (default: stdin)\n"
                    "   -o outfile      Output file for decrypted data (default: stdout)\n"
                    "   -n pbfile       Private key file (default: ss.priv)\n");
}

int main(int argc, char **argv) {

    //defaults declared here
    int opt = 0;

    int verbose = 0;

    char *priv = "ss.priv";

    FILE *input = stdin;
    FILE *output = stdout;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {

        case 'v': verbose = 1; break;

        case 'i':
            input = fopen(optarg, "r");
            if (input == NULL) {
                printf("Error with input file\n");
                return 1;
            }
            break;

        case 'o':
            output = fopen(optarg, "w");
            if (output == NULL) {
                printf("Error with output file\n");
                return 1;
            }
            break;

        case 'n': priv = optarg; break;

        case 'h': help(); return 1;

        default: help(); break;
        }
    }

    //open private key file
    FILE *privfile = fopen(priv, "r");
    if (privfile == NULL) {
        printf("Failed to open private key file\n");
        return 1;
    }

    mpz_t pq, d;
    mpz_inits(pq, d, NULL);

    //read private key file
    ss_read_priv(pq, d, privfile);

    //if verbose output is triggered
    if (verbose == 1) {
        gmp_printf("pq (%d bits) = %Zd\n", mpz_sizeinbase(pq, 2), pq);
        gmp_printf("d  (%d bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }

    //decrypt the file
    ss_decrypt_file(input, output, d, pq);

    //close files
    fclose(privfile);
    fclose(input);
    fclose(output);

    //clear variables
    mpz_clears(pq, d, NULL);

    return 0;
}
