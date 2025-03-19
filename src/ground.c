#include "ground.h"
#include <stdlib.h>

Ground *ground_make(char type) {
    Ground *g = malloc(sizeof(Ground));
    g->type = type;
    return g;
}

void ground_free(Ground *g) {
    free(g);
}

