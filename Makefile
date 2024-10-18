CC = gcc
CFLAGS = -Wall -g
C_FILES = parsing_interface.c execute.c quash.c builtin.c 
H_FILES = parsing_interface.h execute.h quash.h builtin.h 
OBJ_FILES = $(C_FILES:.c=.o)
all: quash 

quash: $(OBJ_FILES)
	$(CC) $(CFLAGS) -o quash $(OBJ_FILES)

%.o: %.c $(H_FILES)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o quash
