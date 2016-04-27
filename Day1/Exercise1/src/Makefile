CC=gcc
CFLAGS=-O2 -Wall
SRC=main.c
OBJ=$(SRC:%.c=%.o)
EXE=../main.x

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -rf *.o


