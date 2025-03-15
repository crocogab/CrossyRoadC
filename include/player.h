#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>


typedef struct _Player {
    // Skin *skin;
    bool alive;
    int direction; //macro
    float h_position;
    bool is_jumping;

} Player;

#endif
