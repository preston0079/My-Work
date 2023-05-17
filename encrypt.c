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
                    "   Encrypts data using SS encryption.\n"
                    "   Encrypted data is decrypted by the decrypt program.\n"
                    "\n"
                    "USAGE\n"
                    "    ./encrypt [OPTIONS]\n"
                    "\n"
                    "OPTIONS\n"
                    "   -h              Display program help and usage.\n"
                    "   -v              Display verbose program output.\n"
                    "   -i infile       Input file of data to encrypt (default: stdin)\n"
                    "   -o outfile      Output file for encrypted data (default: stdout)\n"
                    "   -n pbfile       Public key file (default: ss.pub)\n");
}

int main(int argc, char **argv) {

    //defaults declared here
    int opt = 0;

    int verbose = 0;

    char *pub = "ss.pub";

    char *username = NULL;

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

        case 'n': 
            pub = optarg; 
            break;

        case 'h': help(); return 1;

        default: help(); break;
        }
    }

    //open public key file
    FILE *pubfile = fopen(pub, "r");
    if (pubfile == NULL) {
        printf("Failed to open public key file\n");
        return 1;
    }

    mpz_t n;
    mpz_inits(n, NULL);

    //for username
    username = getenv("USER");

    //read public key from public key file
    ss_read_pub(n, username, pubfile);

    //if verbose output is triggered
    if (verbose == 1) {
        printf("user = %s\n", username);
        gmp_printf("n  (%d bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
    }

    //encrypt the file
    ss_encrypt_file(input, output, n);

    //close files
    fclose(pubfile);
    fclose(input);
    fclose(output);

    //clear variables
    mpz_clears(n, NULL);

    return 0;
}
