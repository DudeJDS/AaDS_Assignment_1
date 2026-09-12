#Define C compiler & flags
CC = gcc
CFLAGS = -Wall -g

#Define sets of source files and object files
SRC1 = driver.c data.c bit.c list.c parse.c
# OBJ is the same as SRC, just replace .c with .o
OBJ1 = $(SRC1:.c=.o)

SRC2 = driver2.c data.c bit.c patricia.c parse.c
OBJ2 = $(SRC2:.c=.o)

# Define executable name(s)
EXE1 = dict1
EXE2 = dict2

all: $(EXE1) $(EXE2)

$(EXE1): $(OBJ1)
	$(CC) $(CFLAGS) -o $(EXE1) $(OBJ1)

$(EXE2): $(OBJ2)
	$(CC) $(CFLAGS) -o $(EXE2) $(OBJ2)

clean:
	rm -f $(OBJ1) $(OBJ2) $(EXE1) $(EXE2)
