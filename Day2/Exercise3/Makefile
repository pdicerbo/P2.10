CC=gcc
CFLAGS=-O2 -Wall
LIBS=-lm
SRC=main.c
OBJ=$(SRC:%.c=%.o)
EXE=main.x

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@

%.o: %.c
	$(CC) -c $^

run: $(EXE)
	./main.x

clean:
	rm -rf *.o *.x *.dat
