CFLAGS = -std=c99 -Wall -Wextra -pedantic -g3
LDFLAGS = -fsanitize=address
CC= clang
HEADER_LOCATION= ./include

all: main_tui

main_tui.o :
	$(CC) -I $(HEADER_LOCATION) -c $(CFLAGS) $(LDFLAGS) main_tui.c

main_tui: main_tui.o
	$(CC) -o main_tui $(LDFLAGS) main_tui.o

clean: 
	rm -rf main_tui.o main_tui