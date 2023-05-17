## Assignment 5 - Public Key Cryptography

This program is composed of three main files: keygen.c, encrypt.c, and decrypt.c. Keygen will create the keys needed for the encryption and decryption process. Using the Schmidt-Samoa (SS) Algorithm, these files will encrypt and decrypt the file the user gives them.

## Building

Run the following in order to run the program:

Installs make, works with makefile
'''
$ sudo apt install make
'''

Input this into terminal right before running any of the files
'''
make all
'''

## Running keygen

Run keygen.c with this command in the terminal
'''
./keygen
'''

Followed by:

-h              Display program help and usage.

-v              Display verbose program output.

-b [bits]         Minimum bits needed for public key n (default: 256)

-i [iterations]   Miller-Rabin iterations for testing primes (default: 50)

-n [pbfile]       Public key file (default: ss.pub)

-d [pvfile]       Private key file (default: ss.priv)

-s [seed]         Random seed for testing.

## Running encrypt

Run encrypt.c with this command in the terminal
'''
./encrypt
'''

Followed by:

-h              Display program help and usage.

-v              Display verbose program output.

-i [infile]       Input file of data to encrypt (default: stdin)

-o [outfile]      Output file for encrypted data (default: stdout)

-n [pbfile]       Public key file (default: ss.pub)

## Running decrypt

Run decrypt.c with this command in the terminal
'''
./decrypt
'''

Followed by:

-h              Display program help and usage.

-v              Display verbose program output.

-i [infile]       Input file of data to decrypt (default: stdin)

-o [outfile]      Output file for decrypted data (default: stdout)

-n [pbfile]       Private key file (default: ss.priv)
