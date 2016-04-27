CC=gcc
CFLAGS=-O2 -Wall
CLIBS=-lm
SRC=main.c
OBJ=$(SRC:%.c=%.o)
EXE=../main.x

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $^ $(CLIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -rf *.o


