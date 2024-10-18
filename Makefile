CC = gcc
CFLAGS = -Wall -g
C_FILES = command.c execute.c quash.c builtin.c 
H_FILES = comand.h execute.h quash.h builtin.h 
OBJ_FILES = $(C_FILES:.c=.o)
all: quash 

quash: $(OBJ_FILES)
	$(CC) $(CFLAGS) -o quash $(OBJ_FILES)

%.o: %.c $(H_FILES)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o quash
