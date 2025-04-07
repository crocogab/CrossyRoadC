# Répertoires
HEADER_DIR = ./include
SRC_DIR = ./src
TEST_DIR = ./Tests

# Fichiers principaux -> pour instant main_tui
TARGET = main_tui
SRCS = main_tui.c
OBJS = $(SRCS:.c=.o)

# Fichiers de test -> rajouter à chaque nouveau test
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

TEST_TARGETS = $(TEST_PLAYER) $(TEST_OBSTACLE) $(TEST_GROUND) $(TEST_RANDOM_CUSTOM)

# Compilation
CC = clang
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g3 -I$(HEADER_DIR)

# Debug
CFLAGS  += -fsanitize=address -fno-omit-frame-pointer
LDFLAGS = -fsanitize=address

# Libs
LIBS = -lncurses

# CFLAGS  += $(shell pkg-config --cflags sdl2)
# LDFLAGS += $(shell pkg-config --libs   sdl2)
# CFLAGS  += $(shell pkg-config --cflags sdl2_image)
# LDFLAGS += $(shell pkg-config --libs   sdl2_image)

# Cibles par défaut
all: $(TARGET)

test: $(TEST_TARGETS)

$(TARGET): $(OBJS)
	$(CC) $^ $(LDFLAGS) $(LIBS) -o $@

# Compilation des tests
$(TEST_BOARD): $(TEST_BOARD_SRCS:.c=.o)
	$(CC) $^ $(LDFLAGS) $(LIBS) -o $@

$(TEST_PLAYER): $(TEST_PLAYER_SRCS:.c=.o)
	$(CC) $^ $(LDFLAGS) $(LIBS) -o $@

$(TEST_OBSTACLE): $(TEST_OBSTACLE_SRCS:.c=.o)
	$(CC) $^ $(LDFLAGS) $(LIBS) -o $@

$(TEST_GROUND): $(TEST_GROUND_SRCS:.c=.o)
	$(CC) $^ $(LDFLAGS) $(LIBS) -o $@

$(TEST_RANDOM_CUSTOM): $(TEST_RANDOM_CUSTOM_SRCS:.c=.o)
	$(CC) $^ $(LDFLAGS) $(LIBS) -o $@

# Règles de compilation génériques
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

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

# Exécuter tous les tests
run_tests: $(TEST_TARGETS)
	for test in $(TEST_TARGETS); do ./$$test; done

# Exécution du programme principal
run_tui: $(TARGET)
	./$(TARGET)

# Nettoyage
clean:
	rm -f $(OBJS) $(TARGET) $(TEST_BOARD) $(TEST_PLAYER) $(TEST_OBSTACLE) $(TEST_GROUND) $(TEST_RANDOM_CUSTOM) $(TEST_BOARD)
	rm -f $(TEST_BOARD_SRCS:.c=.o) $(TEST_PLAYER_SRCS:.c=.o) $(TEST_OBSTACLE_SRCS:.c=.o) $(TEST_GROUND_SRCS:.c=.o) $(TEST_RANDOM_CUSTOM_SRCS:.c=.o)
	rm -f *~ \#*\# .\#*

.PHONY: all clean test run_tests run_test_board run_test_player run_test_obstacle run_tui run_test_ground run_test_random_custom