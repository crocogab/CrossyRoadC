#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "player.h"
#include "game.h"
#include "macro.h"
#include "board.h"
#include <time.h>

/**
 * Affiche un écran de fin de partie avec les options de rejouer ou quitter.
 *
 * @param score Le score final du joueur
 * @return 0 si l'utilisateur choisit de rejouer, 1 s'il quitte
 */
int display_game_over(int score) {
    clear();
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int center_y = max_y / 2;
    int center_x = max_x / 2;
    
    // message de fin de partie
    attron(A_BOLD | COLOR_PAIR(3));
    mvprintw(center_y - 4, center_x - 4, "GAME OVER");
    attroff(A_BOLD | COLOR_PAIR(3));
    
    // score
    mvprintw(center_y - 2, center_x - 8, "Votre score: %d", score);
    
    // options
    attron(A_BOLD);
    mvprintw(center_y + 1, center_x - 14, "Appuyez sur 'r' pour rejouer");
    mvprintw(center_y + 3, center_x - 14, "Appuyez sur 'q' pour quitter");
    attroff(A_BOLD);
    
    // Carré autour
    for (int y = center_y - 6; y <= center_y + 5; y++) {
        for (int x = center_x - 16; x <= center_x + 16; x++) {
            if (y == center_y - 6 || y == center_y + 5) {
                mvaddch(y, x, ACS_HLINE);
            }
            else if (x == center_x - 16 || x == center_x + 16) {
                mvaddch(y, x, ACS_VLINE);
            }
        }
    }
    mvaddch(center_y - 6, center_x - 16, ACS_ULCORNER);
    mvaddch(center_y - 6, center_x + 16, ACS_URCORNER);
    mvaddch(center_y + 5, center_x - 16, ACS_LLCORNER);
    mvaddch(center_y + 5, center_x + 16, ACS_LRCORNER);
    
    refresh();
    nodelay(stdscr, FALSE); // mode bloquant
    
    int ch;
    do {
        ch = getch();
    } while (ch != 'q' && ch != 'Q' && ch != 'r' && ch != 'R');
    
    // mode non bloquant avant de quitter
    nodelay(stdscr, TRUE);
    
    // retourne 0 pour rejouer, 1 pour quitter
    return (ch == 'q' || ch == 'Q') ? 1 : 0;
}

/**
 * Réinitialise le jeu pour une nouvelle partie
 */
void reset_game(Game *g, Player **p, Board **b, char ***tableau, int *score_actu, int *score_maxi) {
    
    grid_tui_free(*tableau);
    board_free(*b);
    
    
    *g = game_make(TO_LAUNCH);
    *p = player_start();
    game_start(g);
    *b = g->board;
    board_set_player(*b, *p);
    *tableau = grid_tui_make(*b);
    
    
    (*p)->score = 0;
    *score_actu = 0;
    *score_maxi = 0;
    
    g->status = PLAYING;
    clear();
}

/**
 * Gère l'affichage de la grille de jeu
 */
void render_grid(char **tableau, int score_maxi, float h_pos) {
    clear(); // On clean tout
    
    
    for (int y = MEMORISATION-1; y < MAP_LEN-MEMORISATION; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            switch (tableau[y][x]) {
                case MODEL_CHICKEN:
                    attron(COLOR_PAIR(1)); // Active couleur jaune
                    mvaddch(y-MEMORISATION, x+1, tableau[y][x]);
                    attroff(COLOR_PAIR(1)); // Désactive
                    break;
                case MODEL_GRASS:
                    attron(COLOR_PAIR(2)); // Active couleur vert
                    mvaddch(y-MEMORISATION, x+1, tableau[y][x]);
                    attroff(COLOR_PAIR(2)); // Désactive
                    break;
                case MODEL_CAR:
                    attron(COLOR_PAIR(3)); // Active couleur rouge
                    mvaddch(y-MEMORISATION, x+1, tableau[y][x]);
                    attroff(COLOR_PAIR(3)); // Désactive
                    break;
                case MODEL_TREE:
                    attron(COLOR_PAIR(4)); // Active couleur vert_arbre
                    mvaddch(y-MEMORISATION, x+1, tableau[y][x]);
                    attroff(COLOR_PAIR(4)); // Désactive
                    break;
                case MODEL_ROCK:
                    attron(COLOR_PAIR(5)); // Active couleur roche
                    mvaddch(y-MEMORISATION, x+1, tableau[y][x]);
                    attroff(COLOR_PAIR(5)); // Désactive
                    break;
                case MODEL_INCOMING_TRAIN:
                    attron(COLOR_PAIR(3)); // Active couleur rouge
                    mvaddch(y-MEMORISATION, x+1, MODEL_RAIL);
                    attroff(COLOR_PAIR(3)); // Désactive
                    break;
                default:
                    mvaddch(y-MEMORISATION, x+1, tableau[y][x]);
                    break;
            }
        }
    }
    
    // bordures
    for (int y = 0; y < (MAP_LEN-2*MEMORISATION); y++) {
        mvaddch(y, 0, ACS_VLINE);
        mvaddch(y, MAP_WIDTH+1, ACS_VLINE);
    }
    
    for (int x = 0; x < (MAP_WIDTH+1); x++) {
        mvaddch(0, x, ACS_HLINE);
        mvaddch(MAP_LEN-2*MEMORISATION, x, ACS_HLINE);
    }
    
    // coins
    mvaddch(0, 0, ACS_ULCORNER);
    mvaddch(0, MAP_WIDTH+1, ACS_URCORNER);
    mvaddch(MAP_LEN-2*MEMORISATION, 0, ACS_LLCORNER);
    mvaddch(MAP_LEN-2*MEMORISATION, MAP_WIDTH+1, ACS_LRCORNER);
    
    // Afficher le score
    attron(A_BOLD);
    mvprintw(1, MAP_WIDTH + 3, "SCORE : %d", score_maxi);
    mvprintw(2, MAP_WIDTH + 3, "h_pos : %f", h_pos);
    // Informations de débogage
    // mvprintw(2, MAP_WIDTH + 3, "[DEBUG] JOUEUR EN VIE: %d", g_board->player->alive);
    // mvprintw(3, MAP_WIDTH + 3, "[DEBUG] CHECK_FUTURE_COLLISION (TOP): %d", check_future_collision(g_board, UP));
    // mvprintw(4, MAP_WIDTH + 3, "[DEBUG] CHECK_FUTURE_COLLISION (DOWN): %d", check_future_collision(g_board, DOWN));
    // mvprintw(5, MAP_WIDTH + 3, "[DEBUG] CHECK_FUTURE_COLLISION (LEFT): %d", check_future_collision(g_board, LEFT));
    // mvprintw(6, MAP_WIDTH + 3, "[DEBUG] CHECK_FUTURE_COLLISION (RIGHT): %d", check_future_collision(g_board, RIGHT));
    attroff(A_BOLD);
    
    refresh(); // Affichage de la grille
}

/**
 * Traite les actions du joueur et met à jour l'état du jeu en conséquence
 */
int process_player_action(Game *g, Player *p, Board *b, int key, int *score_actu, int *jump_back) {
    if (key == 'q' || key == 'Q') {
        return 1; // quitter le jeu
    }
    
    int direction = NEUTRAL;
    
    switch (key) {
        case KEY_UP:
            direction = UP;
            break;
        case KEY_DOWN:
            if (*jump_back < 3) {
                direction = DOWN;
            }
            break;
        case KEY_LEFT:
            direction = LEFT;
            break;
        case KEY_RIGHT:
            direction = RIGHT;
            break;
        default:
            direction = NEUTRAL;
            break;
    }
    
    // verifier les collisons
    int collision_type = check_future_collision(b, direction);
    
    switch (collision_type) {
        case COLLIDE_NONE:
            //mouvement autorise
            if (direction == UP) {
                (*score_actu)++;
                if (*jump_back > 0) {
                    (*jump_back)--;
                }
            } else if (direction == DOWN) {
                (*score_actu)--;
                (*jump_back)++;
            }
            
            
            if (direction != NEUTRAL) {
                move_player(direction, p);
                ground_move(b, direction);
            }
            break;
            
        case COLLIDE_DEADLY:
            // Collision mortelle
            p->alive = false;
            g->status = DEAD;
            break;
            
        case COLLIDE_HARMLESS:
            // Collision sans effet, on ne fait rien
            break;
    }
    
    return 0; // Continuer le jeu
}

// Variable globale pour faciliter l'affichage
Board *g_board;

int main(void) {
    // Initialisation de l'aléatoire
    srand(time(NULL));
    
    // Initialisation du jeu
    Game g = game_make(TO_LAUNCH);
    Player *p = player_start();
    Board *b;
    g_board = NULL;
    
    // Configuration de ncurses
    initscr();
    curs_set(0);   // Cache le curseur
    noecho();      // N'affiche pas les caractères tapés
    cbreak();      // Désactive le buffering des lignes
    raw();
    typeahead(-1);
    start_color();
    
    // Couleurs personnalisées
    init_color(8, 20, 255, 0); // vert_arbre
    init_color(9, 500, 500, 500); // gris
    
    init_pair(1, COLOR_YELLOW, COLOR_BLACK); // jaune sur fond noir (joueur)
    init_pair(2, COLOR_GREEN, COLOR_BLACK);  // vert sur fond noir (herbe)
    init_pair(3, COLOR_RED, COLOR_BLACK);    // rouge sur fond noir (voiture)
    init_pair(4, 8, COLOR_BLACK);            // vert_arbre sur fond noir (arbre)
    init_pair(5, 9, COLOR_BLACK);            // gris sur fond noir (rocher)
    
    clear();
    
    // Configuration des touches et non-blocage
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    
    // Initialisation du plateau
    game_start(&g);
    b = g.board;
    g_board = b;
    
    board_set_player(b, p);
    
    char **tableau = grid_tui_make(b);
    
    int jump_back = 0;    // Limitation des retours en arrière
    int score_actu = 0;   // Score actuel
    int score_maxi = 0;   // Score maximum
    int quit_game = 0;    // Signal pour quitter le jeu
    
    
    while (g.status == PLAYING) {
        // 1. action du joueur
        int key = getch();
        
        // 2. traiter action
        if (key != ERR) {
            quit_game = process_player_action(&g, p, b, key, &score_actu, &jump_back);
            if (quit_game) {
                break;
            }
            flushinp(); // Flush des entrées pour éviter l'accumulation
        }
        
        // 3. mettre a jour les mobs
        board_update(b, 0.01);
        
        // 4. verifier les collisions
        if (key == ERR) {
            int passive_collision = check_future_collision(b, NEUTRAL);
            if (passive_collision == COLLIDE_DEADLY) {
                p->alive = false;
                g.status = DEAD;
            }
        }
        
        // 5. maj de l'etat du jeu
        if (g.status == DEAD) {
            int exit_status = display_game_over(score_maxi);
            if (exit_status) {
                break; // Quitter le jeu
            } else {
                // restart
                reset_game(&g, &p, &b, &tableau, &score_actu, &score_maxi);
                g_board = b;
                continue;
            }
        }
        
        // maj du score
        if (score_actu > score_maxi) {
            score_maxi = score_actu;
        }
        
        // 6. maj de l'etat graphique
        grid_tui_free(tableau);
        tableau = grid_tui_make(b);
        
        // 7. affichage
        render_grid(tableau, score_maxi,p->h_position);
        
        // Contrôle de la vitesse de la boucle de jeu
        napms(5); // Pause de 5ms
    }
    
    // Nettoyage avant de quitter
    endwin();
    grid_tui_free(tableau);
    board_free(b);
    
    return 0;
}