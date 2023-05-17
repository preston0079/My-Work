CC       = clang
CFLAGS   = -Wall -Wpedantic -Werror -Wextra

all: keygen encrypt decrypt

keygen: keygen.o ss.o randstate.o numtheory.o
	$(CC) -o $@ $^ $(shell pkg-config --libs gmp)

encrypt: encrypt.o ss.o randstate.o numtheory.o
	$(CC) -o $@ $^ $(shell pkg-config --libs gmp)

decrypt: decrypt.o ss.o randstate.o numtheory.o
	$(CC) -o $@ $^ $(shell pkg-config --libs gmp)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f keygen encrypt decrypt *.o

format:
	clang-format -i -style=file *.[ch]
