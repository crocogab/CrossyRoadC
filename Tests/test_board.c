#include "board.h"
#include <assert.h>

int main(void) {
    Board *b = board_make();

    // tester des trucs ici

    board_free(b);

    return 0;
}
