#Define C compiler & flags
CC = gcc
CFLAGS = -Wall -g

#Define sets of source files and object files
SRC = driver.c data.c bit.c list.c
# OBJ is the same as SRC, just replace .c with .o
OBJ = $(SRC:.c=.o)

# Define executable name(s)
EXE = dict1

# the first target:
$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ)

clean:
	rm -f $(OBJ) $(EXE)
