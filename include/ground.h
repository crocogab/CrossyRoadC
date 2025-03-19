#ifndef GROUND_H
#define GROUND_H


typedef struct _Ground {
    char type;
} Ground;

Ground *ground_make(char);

void ground_free(Ground *);

#endif
