CC = gcc
BINARY = calc
INCDIR = ./include/
LIBDIR = ./lib/
FLAGS = -Wall -Wextra -pedantic -std=c99 -ggdb -Wunused-function -Wmissing-prototypes $(foreach D, $(INCDIR),-I$(D)) $(OPT)
LDFlags=-L$(LIBDIR) -lraylib -lm
CFILES = main.c calc.c
OBJECTS = main.o calc.o
OP =  -o0

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) $(FLAGS) -o $@ $^ $(LDFlags)

%.o:%.c
	$(CC) $(FLAGS) -c -o $@ $^

clean:
	rm -rf $(BINARY) $(OBJECTS)

