#include "ss.h"
#include "randstate.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gmp.h>
#include <time.h>
#include <sys/stat.h> //For fchmod()

#define OPTIONS "hvb:i:n:d:s:"

//help message
void help(void) {
    fprintf(stderr, "SYNOPSIS\n"
                    "   Generates an SS public/private key pair\n"
                    "\n"
                    "USAGE\n"
                    "    ./keygen [OPTIONS]\n"
                    "\n"
                    "OPTIONS\n"
                    "   -h              Display program help and usage.\n"
                    "   -v              Display verbose program output.\n"
                    "   -b bits         Minimum bits needed for public key n (default: 256)\n"
                    "   -i iterations   Miller-Rabin iterations for testing primes (default: 50)\n"
                    "   -n pbfile       Public key file (default: ss.pub)\n"
                    "   -d pvfile       Private key file (default: ss.priv)\n"
                    "   -s seed         Random seed for testing.\n");
}

//follow getopt format which was given in a previous assignment

int main(int argc, char **argv) {

    //defaults declared here
    int opt = 0;

    int verbose = 0;

    int bits = 256;
    int iters = 50;

    char *pub = "ss.pub";
    char *priv = "ss.priv";

    uint64_t seed = time(NULL);

    int permission = 0600;

    char *username = NULL;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {

        case 'v':
            //normally 0, if -v is triggered, it will print verbose response
            verbose = 1;
            break;

        case 'b': bits = (uint64_t) strtoul(optarg, NULL, 10); break;

        case 'i': iters = (uint64_t) strtoul(optarg, NULL, 10); break;

        case 'n': pub = optarg; break;

        case 'd': priv = optarg; break;

        case 's': seed = (uint64_t) strtoul(optarg, NULL, 10); break;

        case 'h': help(); return 1;

        default: help(); break;
        }
    }

    //fopen public key file and private key file, prints message if error
    FILE *pubfile = fopen(pub, "w");
    if (pubfile == NULL) {
        printf("Failed to open file\n");
        return 1;
    }
    FILE *privfile = fopen(priv, "w");
    if (privfile == NULL) {
        printf("Failed to open file\n");
        return 1;
    }

    //set private key permission to 0600, indicates read and write permissions for the user, and no permissions for anyone else.
    fchmod(fileno(privfile), permission);

    //initialize random state with given or default seed
    randstate_init(seed);

    mpz_t p, q, n, d, pq;
    mpz_inits(p, q, n, d, pq, NULL);

    mpz_mul(pq, p, q);

    //create the public and private keys
    ss_make_pub(p, q, n, bits, iters);
    ss_make_priv(d, pq, p, q);

    //get username
    username = getenv("USER");

    //write keys to respective files
    ss_write_pub(n, username, pubfile);
    ss_write_priv(pq, d, privfile);

    //if verbose output is triggered
    if (verbose == 1) {
        printf("user = %s\n", username);
        gmp_printf("p  (%d bits) = %Zd\n", mpz_sizeinbase(p, 2), p);
        gmp_printf("q  (%d bits) = %Zd\n", mpz_sizeinbase(q, 2), q);
        gmp_printf("n  (%d bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("d  (%d bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
        gmp_printf("pq (%d bits) = %Zd\n", mpz_sizeinbase(pq, 2), pq);
    }

    //close files
    fclose(pubfile);
    fclose(privfile);

    //clear random state
    randstate_clear();

    //clear mpz variables
    mpz_clears(p, q, n, d, pq, NULL);

    return 0;
}
