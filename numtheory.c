

#include <stdio.h>
#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>

#include "numtheory.h"
#include "randstate.h"

//help recieved from my own work from previous time taking this class and from TA/tutors

//follow psuedocode given in asgn pdf

//follow format:
//create copies of variables and set them to corresponding variables
//use temp variable for parallel assignment

//clear mpz variables at the end

void gcd(mpz_t g, const mpz_t a, const mpz_t b) {
    //this is the creation of the copies and extra variables needed. Repeat with the other functions
    mpz_t(acopy);
    mpz_t(bcopy);
    mpz_t(temp);

    //initialize the mpz variables
    mpz_inits(acopy, bcopy, temp, NULL);

    //set the copies to the corresponding variables
    mpz_set(acopy, a);
    mpz_set(bcopy, b);

    //work is taken from the asgn pdf and converted to C
    while (mpz_cmp_si(bcopy, 0) != 0) { //while b not eqaul to 0
        mpz_set(temp, bcopy); //t <- b
        mpz_mod(bcopy, acopy, bcopy); //b <- a mod b
        mpz_set(acopy, temp); //a <- t
    }
    mpz_set(g, acopy); //return a

    //clear the initialized varibles that were made before
    mpz_clears(acopy, bcopy, temp, NULL);
}

void mod_inverse(mpz_t o, const mpz_t a, const mpz_t n) {
    mpz_t r;
    mpz_t rprime;
    mpz_t t;
    mpz_t tprime;
    mpz_t q1;
    mpz_t q2;
    mpz_t temp1;
    mpz_t temp2;
    mpz_inits(r, rprime, t, tprime, q1, q2, temp1, temp2, NULL);

    mpz_set(r, n); //r <- n
    mpz_set(rprime, a); //r' <- a

    mpz_set_ui(t, 0); //t <- 0
    mpz_set_ui(tprime, 1); //t' <- 1

    while (mpz_cmp_ui(rprime, 0) != 0) { //while r' does not equal 0
        mpz_fdiv_q(q1, r, rprime); //q1 <- r/r'

        mpz_set(temp1, r); //temp1 equal to r
        mpz_set(temp2, rprime); //temp2 equal to r'
        mpz_set(r, temp2); //r equal to r'

        mpz_mul(q2, q1, temp2); //q * r'
        mpz_sub(rprime, temp1, q2); //r' <- r - (q * r')

        mpz_set(temp1, t); //temp1 equal to t
        mpz_set(temp2, tprime); //temp2 equal to t'
        mpz_set(t, temp2); //t <- t'

        mpz_mul(q1, q1, temp2); //q * t'
        mpz_sub(tprime, temp1, q1); //t' <- t - (q * t')
    }

    if (mpz_cmp_ui(r, 1) > 0) { //if r > 1
        mpz_set_ui(o, 0); //return no inverse
    } else {
        if (mpz_cmp_ui(t, 0) < 0) { //t < 0
            mpz_add(t, t, n); //t <- t + n
        }
        mpz_set(o, t); //return t
    }
    mpz_clears(r, rprime, t, tprime, q1, q2, temp1, temp2, NULL); //clear variables
}

void pow_mod(mpz_t o, const mpz_t a, const mpz_t d, const mpz_t n) {
    //same format/style is done for the rest of the functions as shown above
    //follow format and implement the provided psuedocode to replicate effect
    mpz_t acopy;
    mpz_t dcopy;
    mpz_t ncopy;
    mpz_t v;
    mpz_t p;
    mpz_t temp;
    mpz_inits(acopy, dcopy, ncopy, v, p, temp, NULL);

    mpz_set(acopy, a);
    mpz_set(dcopy, d);
    mpz_set(ncopy, n);

    mpz_set_ui(v, 1);
    mpz_set(p, a);

    while (mpz_cmp_si(dcopy, 0) > 0) {
        if (mpz_odd_p(dcopy) != 0) { //if d is odd
            mpz_mul(temp, v, p);
            mpz_mod(v, temp, ncopy);
        }
        mpz_mul(temp, p, p);
        mpz_mod(p, temp, ncopy);
        mpz_fdiv_q_ui(dcopy, dcopy, 2); //d <- d/2
    }
    mpz_set(o, v); //return v
    mpz_clears(acopy, dcopy, ncopy, v, p, temp, NULL);
}

//help recieved from tutor/TA, no code was given, all conceptual help
bool is_prime(const mpz_t n, uint64_t iters) {
    mpz_t nsub1;
    mpz_t nsubcopy;
    mpz_t nsub2;
    mpz_t ssub1;
    mpz_t s;
    mpz_t r;
    mpz_t a;
    mpz_t y;
    mpz_t j;
    mpz_t two;
    mpz_inits(nsub1, nsubcopy, nsub2, ssub1, s, r, a, y, j, two, NULL);

    mpz_sub_ui(nsub1, n, 1);
    mpz_sub_ui(nsubcopy, n, 1);
    mpz_sub_ui(nsub2, n, 2);
    mpz_sub_ui(ssub1, s, 1);
    mpz_set_ui(two, 2);

    //if n is 2 or 3
    if (mpz_cmp_ui(n, 2) == 0 || mpz_cmp_ui(n, 3) == 0) {
        mpz_clears(nsub1, nsubcopy, nsub2, ssub1, s, r, a, y, j, two, NULL);
        return true;
    }

    //if n is 1 or even
    if (mpz_cmp_ui(n, 1) == 0 || mpz_even_p(n)) {
        mpz_clears(nsub1, nsubcopy, nsub2, ssub1, s, r, a, y, j, two, NULL);
        return false;
    }

    //for line 1 of miller-rabin(n,k)
    while (mpz_even_p(nsubcopy)) {
        mpz_add_ui(s, s, 1);
        mpz_div_ui(nsubcopy, nsubcopy, 2);
    }
    //for r
    mpz_set(r, nsubcopy);

    //for loop section of psuedocode past this point, help also received from tutor/TA, no code, just conceptual help and pointed out things I could do
    for (uint64_t i = 1; i < iters; i++) {

        do {
            mpz_urandomm(a, state, n);
        } while (mpz_cmp_ui(a, 2) < 0 || mpz_cmp(a, nsub2) >= 0); //restrictions on random a

        pow_mod(y, a, r, n);

        if (mpz_cmp_ui(y, 1) != 0 && mpz_cmp(y, nsub1) != 0) {
            mpz_set_ui(j, 1);

            while (mpz_cmp(j, s) <= 0 && mpz_cmp(y, nsub1) != 0) {
                pow_mod(y, y, two, n);

                if (mpz_cmp_ui(y, 1) == 0) {
                    mpz_clears(nsub1, nsubcopy, nsub2, ssub1, s, r, a, y, j, two, NULL);
                    return false;
                }
                mpz_add_ui(j, j, 1);
            }
            if (mpz_cmp(y, nsub1) != 0) {
                mpz_clears(nsub1, nsubcopy, nsub2, ssub1, s, r, a, y, j, two, NULL);
                return false;
            }
        }
    }
    mpz_clears(nsub1, nsubcopy, nsub2, ssub1, s, r, a, y, j, two, NULL);
    return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    mpz_urandomb(p, state, bits);
    while (!(is_prime(p, iters))) { //while it is not prime, go again until it is
        mpz_urandomb(p, state, bits);
    }
}
