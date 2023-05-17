#include <stdio.h>
#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "ss.h"
#include "numtheory.h"
#include "randstate.h"

//help recieved from my own work from previous time taking this class
//similar format to numtheory, using gmp, create variables when necessary and clear them at the end

//
// Generates the components for a new SS key.
//
// Provides:
//  p:  first prime
//  q: second prime
//  n: public modulus/exponent
//
// Requires:
//  nbits: minimum # of bits in n
//  iters: iterations of Miller-Rabin to use for primality check
//  all mpz_t arguments to be initialized
//
void ss_make_pub(mpz_t p, mpz_t q, mpz_t n, uint64_t nbits, uint64_t iters) {
    //create variables
    mpz_t qsub1;
    mpz_t psub1;
    mpz_inits(qsub1, psub1, NULL);

    mpz_sub_ui(qsub1, q, 1);
    mpz_sub_ui(psub1, p, 1);

    //bits for p and q
    uint64_t pbits = random() % (nbits / 5) + nbits / 5;
    uint64_t qbits = nbits - 2 * pbits;

    //creates primes p and q
    make_prime(p, pbits, iters);
    make_prime(q, qbits, iters);

    // n = (p * p) * q
    mpz_mul(n, p, p);
    mpz_mul(n, n, q);

    //check that p is not divisible by q-1 and q is not divisible by p-1
    if (mpz_divisible_p(p, qsub1) || mpz_divisible_p(q, psub1)) {
        //if p is divisible by q-1 or q is divisible by p-1, go again
        ss_make_pub(p, q, n, nbits, iters);
    }

    //clear initialized variables
    mpz_clears(qsub1, psub1, NULL);
}

//
// Generates components for a new SS private key.
//
// Provides:
//  d:  private exponent
//  pq: private modulus
//
// Requires:
//  p:  first prime number
//  q: second prime number
//  all mpz_t arguments to be initialized
//
void ss_make_priv(mpz_t d, mpz_t pq, const mpz_t p, const mpz_t q) {

    // Compute lambda(pq) = lcm(p - 1, q - 1)
    mpz_t lambda;
    mpz_t pcopy;
    mpz_t qcopy;
    mpz_t n;
    mpz_t top;
    mpz_t bot;
    mpz_inits(lambda, pcopy, qcopy, n, top, bot, NULL);

    // pcopy = p - 1
    // qcopy = q - 1
    mpz_sub_ui(pcopy, p, 1);
    mpz_sub_ui(qcopy, q, 1);

    mpz_mul(pq, p, q);

    //lcm(p-1, q-1) = (((p-1)*(q-1)) / (gcd((p-1)(q-1)))
    mpz_mul(top, pcopy, qcopy);
    gcd(bot, pcopy, qcopy);
    mpz_fdiv_q(lambda, top, bot);

    // n = p^2 * q
    mpz_mul(n, p, p);
    mpz_mul(n, n, q);
    mod_inverse(d, n, lambda);
    mpz_clears(lambda, pcopy, qcopy, n, top, bot, NULL);
}

//
// Export SS public key to output stream
//
// Requires:
//  n: public modulus/exponent
//  username: login name of keyholder ($USER)
//  pbfile: open and writable file stream
//
void ss_write_pub(const mpz_t n, const char username[], FILE *pbfile) {
    //n and username as hexstring with trailing newline
    gmp_fprintf(pbfile, "%Zx\n", n);
    fprintf(pbfile, "%s \n", username);
}

//
// Export SS private key to output stream
//
// Requires:
//  pq: private modulus
//  d:  private exponent
//  pvfile: open and writable file stream
//
void ss_write_priv(const mpz_t pq, const mpz_t d, FILE *pvfile) {
    //pq and d as hexstrings with trailing newline
    gmp_fprintf(pvfile, "%Zx\n", pq);
    gmp_fprintf(pvfile, "%Zx\n", d);
}

//
// Import SS public key from input stream
//
// Provides:
//  n: public modulus
//  username: $USER of the pubkey creator
//
// Requires:
//  pbfile: open and readable file stream
//  username: requires sufficient space
//  all mpz_t arguments to be initialized
//
void ss_read_pub(mpz_t n, char username[], FILE *pbfile) {
    //n and username as hexstring with trailing newline
    gmp_fscanf(pbfile, "%Zx\n", n);
    fscanf(pbfile, "%s \n", username);
}

//
// Import SS private key from input stream
//
// Provides:
//  pq: private modulus
//  d:  private exponent
//
// Requires:
//  pvfile: open and readable file stream
//  all mpz_t arguments to be initialized
//
void ss_read_priv(mpz_t pq, mpz_t d, FILE *pvfile) {
    //pq and d as hexstrings with trailing newline
    gmp_fscanf(pvfile, "%Zx\n", pq);
    gmp_fscanf(pvfile, "%Zx\n", d);
}

//
// Encrypt number m into number c
//
// Provides:
//  c: encrypted integer
//
// Requires:
//  m: original integer
//  n: public exponent/modulus
//  all mpz_t arguments to be initialized
//
void ss_encrypt(mpz_t c, const mpz_t m, const mpz_t n) {
    //use pow_mod function that was made in numtheory
    pow_mod(c, m, n, n);
}

//
// Encrypt an arbitrary file
//
// Provides:
//  fills outfile with the encrypted contents of infile
//
// Requires:
//  infile: open and readable file stream
//  outfile: open and writable file stream
//  n: public exponent and modulus
//
void ss_encrypt_file(FILE *infile, FILE *outfile, const mpz_t n) {
    mpz_t m;
    mpz_t c;
    mpz_t squarerootn;
    mpz_inits(m, c, squarerootn, NULL);

    uint64_t j;
    mpz_sqrt(squarerootn, n);

    //block size k
    uint64_t k = (mpz_sizeinbase(squarerootn, 2) - 1) / 8;

    //allocate memory for the block
    uint8_t *block = (uint8_t *) malloc(k * sizeof(uint8_t));

    //prepend byte
    block[0] = 0xFF;

    //read and encrypt blocks
    while ((j = fread(&block[1], 1, k - 1, infile)) > 0) {
        //mpz_import to convert the read bytes
        mpz_import(m, j + 1, 1, sizeof(uint8_t), 1, 0, block);

        //encrypt m
        ss_encrypt(c, m, n);
        //hexstring with trailing newline
        gmp_fprintf(outfile, "%Zx\n", c);
    }

    // Free memory
    free(block);

    // Clear variables
    mpz_clears(m, c, squarerootn, NULL);
}

//
// Decrypt number c into number m
//
// Provides:
//  m: decrypted/original integer
//
// Requires:
//  c: encrypted integer
//  d: private exponent
//  pq: private modulus
//  all mpz_t arguments to be initialized
//
void ss_decrypt(mpz_t m, const mpz_t c, const mpz_t d, const mpz_t pq) {
    //use pow_mod which was created earlier
    pow_mod(m, c, d, pq);
}

//
// Decrypt a file back into its original form.
//
// Provides:
//  fills outfile with the unencrypted data from infile
//
// Requires:
//  infile: open and readable file stream to encrypted data
//  outfile: open and writable file stream
//  d: private exponent
//  pq: private modulus
//
void ss_decrypt_file(FILE *infile, FILE *outfile, const mpz_t d, const mpz_t pq) {
    //size of the block to hold k = (log2(pq) - 1)/8 bytes
    uint64_t k = (mpz_sizeinbase(pq, 2) - 1) / 8;
    uint8_t *block = (uint8_t *) malloc(k * sizeof(uint8_t));

    mpz_t c;
    mpz_t m;
    mpz_inits(c, m, NULL);

    size_t j;

    //scan in hexstring until end of file
    while (gmp_fscanf(infile, "%Zx\n", c) != EOF) {

        //Decrypt c to get m
        ss_decrypt(m, c, d, pq);

        //export m to bytes and store them in the block
        mpz_export(&block[0], &j, 1, sizeof(uint8_t), 1, 0, m);

        //Write out j-1 bytes from the block to outfile (start from 1)
        fwrite(&block[1], sizeof(uint8_t), j - 1, outfile);
    }

    //free the memory used by the block
    free(block);
    mpz_clears(c, m, NULL);
}
