# Tous les dossiers
HEADER_DIR = ./include
SRC_DIR = ./src
TEST_DIR = ./Tests

# cibles (GUI et TUI)
TARGET_GUI = main_gui
TARGET_TUI = main_tui

# fichiers source GUI
SRCS_GUI = main_gui.c $(SRC_DIR)/gui.c
OBJS_GUI = $(SRCS_GUI:.c=.o)

# fichiers sources TUI
SRCS_TUI = main_tui.c $(SRC_DIR)/game.c $(SRC_DIR)/board.c $(SRC_DIR)/player.c $(SRC_DIR)/ground.c $(SRC_DIR)/obstacle.c $(SRC_DIR)/random_custom.c
OBJS_TUI = $(SRCS_TUI:.c=.o)

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
TEST_GUI_SRCS = $(TEST_DIR)/test_gui.c $(SRC_DIR)/gui.c

TEST_TARGETS = $(TEST_PLAYER) $(TEST_OBSTACLE) $(TEST_GROUND) $(TEST_RANDOM_CUSTOM) $(TEST_BOARD) $(TEST_GUI)

# Compilation
CC = clang
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g3 -I$(HEADER_DIR)

# sanityzer
CFLAGS += -fsanitize=address -fno-omit-frame-pointer
LDFLAGS = -fsanitize=address

# libs
LIBS_TUI = -lncurses
LIBS_GUI = $(shell pkg-config --libs sdl2 SDL2_image json-c)
CFLAGS_GUI = $(shell pkg-config --cflags sdl2 SDL2_image json-c)

# Cible par défaut : GUI
all: $(TARGET_GUI)

# ---------- Compilation des cibles ----------

# GUI
$(TARGET_GUI): $(OBJS_GUI)
	$(CC) $(OBJS_GUI) $(LDFLAGS) $(LIBS_GUI) -o $@

# TUI
$(TARGET_TUI): $(OBJS_TUI)
	$(CC) $(OBJS_TUI) $(LDFLAGS) $(LIBS_TUI) -o $@

# ---------- Compilation des tests ----------
$(TEST_BOARD): $(TEST_BOARD_SRCS:.c=.o)
	$(CC) $^ $(LDFLAGS) $(LIBS_TUI) -o $@

$(TEST_PLAYER): $(TEST_PLAYER_SRCS:.c=.o)
	$(CC) $^ $(LDFLAGS) $(LIBS_TUI) -o $@

$(TEST_OBSTACLE): $(TEST_OBSTACLE_SRCS:.c=.o)
	$(CC) $^ $(LDFLAGS) $(LIBS_TUI) -o $@

$(TEST_GROUND): $(TEST_GROUND_SRCS:.c=.o)
	$(CC) $^ $(LDFLAGS) $(LIBS_TUI) -o $@

$(TEST_RANDOM_CUSTOM): $(TEST_RANDOM_CUSTOM_SRCS:.c=.o)
	$(CC) $^ $(LDFLAGS) $(LIBS_TUI) -o $@

$(TEST_GUI): $(TEST_GUI_SRCS:.c=.o)
	$(CC) $^ $(LDFLAGS) $(LIBS_GUI) -o $@

# ---------- Règles génériques ----------
%.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS_GUI) -c $< -o $@

# ---------- Exécution ----------
run_gui: $(TARGET_GUI)
	./$(TARGET_GUI)

run_tui: $(TARGET_TUI)
	./$(TARGET_TUI)

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

run_tests: $(TEST_TARGETS)
	for test in $(TEST_TARGETS); do ./$$test; done

# ---------- Nettoyage ----------
clean:
	rm -f $(OBJS_GUI) $(OBJS_TUI) $(TARGET_GUI) $(TARGET_TUI) $(TEST_TARGETS)
	rm -f $(TEST_BOARD_SRCS:.c=.o) $(TEST_PLAYER_SRCS:.c=.o) $(TEST_OBSTACLE_SRCS:.c=.o) $(TEST_GROUND_SRCS:.c=.o) $(TEST_RANDOM_CUSTOM_SRCS:.c=.o) $(TEST_GUI_SRCS:.c=.o)
	rm -f *~ \#*\# .\#*

.PHONY: all clean test run_tests run_tui run_gui run_test_* 
