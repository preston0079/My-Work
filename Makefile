EXECBIN  = life

CC       = clang
CFLAGS   = -Wall -Wpedantic -Werror -Wextra

SOURCES  = life.c universe.c


OBJECTS  = $(SOURCES:%.c=%.o)

all: $(EXECBIN)

$(EXECBIN): $(OBJECTS)
	$(CC) $(CFLAGS) -lncurses -o $(EXECBIN) $(OBJECTS)

$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) -c $(SOURCES)

clean:
	rm -f $(EXECBIN) $(OBJECTS)

format:
	clang-format -i -style=file *.[ch]