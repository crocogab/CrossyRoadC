# Tous les dossiers
HEADER_DIR = ./include
SRC_DIR = ./src
TEST_DIR = ./Tests

# cible GUI uniquement
TARGET_GUI = main_gui

# fichiers source GUI
SRCS_GUI = main_gui.c $(SRC_DIR)/gui.c $(SRC_DIR)/game.c $(SRC_DIR)/board.c $(SRC_DIR)/player.c $(SRC_DIR)/ground.c $(SRC_DIR)/obstacle.c $(SRC_DIR)/random_custom.c $(SRC_DIR)/ttf.c $(SRC_DIR)/menu.c
OBJS_GUI = $(SRCS_GUI:.c=.o)

# Tests
TEST_BOARD = test_board
TEST_BOARD_SRCS = $(TEST_DIR)/test_board.c $(SRC_DIR)/board.c $(SRC_DIR)/ground.c $(SRC_DIR)/obstacle.c $(SRC_DIR)/random_custom.c $(SRC_DIR)/player.c

TEST_PLAYER = test_player
TEST_PLAYER_SRCS = $(TEST_DIR)/test_player.c $(SRC_DIR)/player.c

TEST_OBSTACLE = test_obstacle
TEST_OBSTACLE_SRCS = $(TEST_DIR)/test_obstacle.c $(SRC_DIR)/obstacle.c

TEST_GROUND = test_ground
TEST_GROUND_SRCS = $(TEST_DIR)/test_ground.c $(SRC_DIR)/ground.c $(SRC_DIR)/obstacle.c $(SRC_DIR)/random_custom.c

TEST_RANDOM_CUSTOM = test_random_custom
TEST_RANDOM_CUSTOM_SRCS = $(TEST_DIR)/test_random_custom.c $(SRC_DIR)/random_custom.c

TEST_GUI = test_gui
TEST_GUI_SRCS = $(TEST_DIR)/test_gui.c $(SRC_DIR)/gui.c $(SRC_DIR)/board.c $(SRC_DIR)/ground.c $(SRC_DIR)/obstacle.c $(SRC_DIR)/random_custom.c $(SRC_DIR)/player.c

TEST_TTF = test_ttf
TEST_TTF_SRCS = $(TEST_DIR)/test_ttf.c $(SRC_DIR)/gui.c $(SRC_DIR)/board.c $(SRC_DIR)/ground.c $(SRC_DIR)/obstacle.c $(SRC_DIR)/random_custom.c $(SRC_DIR)/player.c $(SRC_DIR)/ttf.c

TEST_MENU = test_menu
TEST_MENU_SRCS = $(TEST_DIR)/test_menu.c $(SRC_DIR)/gui.c $(SRC_DIR)/board.c $(SRC_DIR)/ground.c $(SRC_DIR)/obstacle.c $(SRC_DIR)/random_custom.c $(SRC_DIR)/player.c $(SRC_DIR)/ttf.c $(SRC_DIR)/menu.c

TEST_TARGETS = $(TEST_PLAYER) $(TEST_OBSTACLE) $(TEST_GROUND) $(TEST_RANDOM_CUSTOM) $(TEST_BOARD) $(TEST_GUI) $(TEST_TTF) $(TEST_MENU)

# Compilation
CC = clang
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g3 -I$(HEADER_DIR)

# sanitizers
CFLAGS += -fsanitize=address -fno-omit-frame-pointer
LDFLAGS = -fsanitize=address

# libs
LIBS_GUI = $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf json-c)
CFLAGS_GUI = $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf json-c)

# Cible par défaut : GUI
all: $(TARGET_GUI)

# ---------- Compilation de la cible ----------
$(TARGET_GUI): $(OBJS_GUI)
	$(CC) $(OBJS_GUI) $(LDFLAGS) $(LIBS_GUI) -o $@

# ---------- Compilation des tests ----------
$(TEST_BOARD): $(TEST_BOARD_SRCS:.c=.o)
	$(CC) $^ $(LDFLAGS) -o $@

$(TEST_PLAYER): $(TEST_PLAYER_SRCS:.c=.o)
	$(CC) $^ $(LDFLAGS) -o $@

$(TEST_OBSTACLE): $(TEST_OBSTACLE_SRCS:.c=.o)
	$(CC) $^ $(LDFLAGS) -o $@

$(TEST_GROUND): $(TEST_GROUND_SRCS:.c=.o)
	$(CC) $^ $(LDFLAGS) -o $@

$(TEST_RANDOM_CUSTOM): $(TEST_RANDOM_CUSTOM_SRCS:.c=.o)
	$(CC) $^ $(LDFLAGS) -o $@

$(TEST_GUI): $(TEST_GUI_SRCS:.c=.o)
	$(CC) $^ $(LDFLAGS) $(LIBS_GUI) -o $@

$(TEST_TTF): $(TEST_TTF_SRCS:.c=.o)
	$(CC) $^ $(LDFLAGS) $(LIBS_GUI) -o $@

$(TEST_MENU): $(TEST_MENU_SRCS:.c=.o)
	$(CC) $^ $(LDFLAGS) $(LIBS_GUI) -o $@

# ---------- Règles génériques ----------
%.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS_GUI) -c $< -o $@

# ---------- Exécution ----------
run_gui: $(TARGET_GUI)
	./$(TARGET_GUI)

test: $(TEST_TARGETS)

run_test_board: $(TEST_BOARD)
	./$(TEST_BOARD)

run_test_player: $(TEST_PLAYER)
	./$(TEST_PLAYER)

run_test_obstacle: $(TEST_OBSTACLE)
	./$(TEST_OBSTACLE)

run_test_ground: $(TEST_GROUND)
	./$(TEST_GROUND)

run_test_random_custom: $(TEST_RANDOM_CUSTOM)
	./$(TEST_RANDOM_CUSTOM)

run_test_gui: $(TEST_GUI)
	./$(TEST_GUI)

run_test_ttf: $(TEST_TTF)
	./$(TEST_TTF)

run_test_menu: $(TEST_MENU)
	./$(TEST_MENU)

run_tests: $(TEST_TARGETS)
	for test in $(TEST_TARGETS); do ./$$test; done

# ---------- Nettoyage ----------
clean:
	rm -f $(OBJS_GUI) $(TARGET_GUI) $(TEST_TARGETS)
	rm -f $(TEST_BOARD_SRCS:.c=.o) $(TEST_PLAYER_SRCS:.c=.o) $(TEST_OBSTACLE_SRCS:.c=.o) $(TEST_GROUND_SRCS:.c=.o) $(TEST_RANDOM_CUSTOM_SRCS:.c=.o) $(TEST_GUI_SRCS:.c=.o) $(TEST_TTF_SRCS:.c=.o) $(TEST_MENU_SRCS:.c=.o)
	rm -f *~ \#*\# .\#*

.PHONY: all clean test run_tests run_gui run_test_*
