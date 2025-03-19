CFLAGS = -std=c99 -Wall -Wextra -pedantic -g3
LDFLAGS = -fsanitize=address
CC= clang
HEADER_LOCATION= ./include
SRC_DIR = ./src

all: main_tui

test: test_player



player.o: $(SRC_DIR)/player.c $(HEADER_LOCATION)/player.h
	$(CC) -c $(CFLAGS) $(LDFLAGS) -I $(HEADER_LOCATION) $(SRC_DIR)/player.c

test_player.o: $(SRC_DIR)/test_player.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) -I $(HEADER_LOCATION) $(SRC_DIR)/test_player.c

test_player: test_player.o player.o
	$(CC) -o test_player $(LDFLAGS) test_player.o player.o

run_test_player: test_player
	./test_player

game.o:
	$(CC) -c $(CFLAGS) $(LDFLAGS) -I $(HEADER_LOCATION) $(SRC_DIR)/game.c

obstacle.o: 
	$(CC) $(CFLAGS) $(LDFLAGS) -I $(HEADER_LOCATION) -c $(SRC_DIR)/obstacle.c

main_tui.o : main_tui.c
	$(CC) -I $(HEADER_LOCATION) -c $(CFLAGS) $(LDFLAGS) main_tui.c

main_tui: main_tui.o game.o
	$(CC) -o main_tui $(LDFLAGS) main_tui.o game.o

clean: 
	rm -rf *.o main_tui test_player