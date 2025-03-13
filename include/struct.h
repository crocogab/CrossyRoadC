#include <stdbool.h>

// typedef struct _Skin
// {
//     char *name;
//     char *model_path;
// } Skin;

typedef struct _Player {
    // Skin *skin;
    bool alive;
    int direction; //macro
    float h_position;
    bool is_jumping;

} Player;

typedef struct _Obstacle {
    char type; //macro
    char model;
    float h_position;
    float velocity;
    float length;
} Obstacle;

typedef struct _Ground {
    char type;
} Ground;

typedef struct _Board {
    Obstacle **grid_obstacle;
    Player *player; // * ou pas * ?
    Ground *grid_ground;  
} Board;

typedef struct _Game {
    Board *board; // * ou pas * ?
    int score;
    int status; //macro
} Game;
