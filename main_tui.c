#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "player.h"
#include "game.h"
#include "macro.h"
#include "board.h"


int main(void) {
    
    // on commence par initialiser la game
    Game g = game_make(TO_LAUNCH);
    
    // on crée un joueur 
    Player *p = player_start();

    
    
    // Configuration de ncurses
    initscr();
    curs_set(0);   // cache le curseur
    noecho();      // affiche pas les caractères tapés
    cbreak();      // désactive le buffering des lignes
    raw();
    typeahead(-1);
    start_color();

    init_pair(1, COLOR_YELLOW, COLOR_BLACK); // jaune sur fond noir
    init_pair(2, COLOR_GREEN, COLOR_BLACK);  // vert sur fond noir
    init_pair(3, COLOR_RED, COLOR_BLACK);    // rouge sur fond noir
    init_color()

    clear();

    
    game_start(&g); // pour initialiser le tasbleau
    
    board_set_player(g.board,p);
    
    char ** tableau = grid_tui_make(g.board);
    printf("%c",tableau[0][0]);
    
    
    
    // Configuration des touches et non-blocage
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    
    int jump_back = 0; // on va limiter le nombre de retours en arriere 
    // Boucle principale
    while (true) {
        
        clear(); //on clean tout
        
        // Dessiner la zone visible par la caméra -> enlever les 3 premieres lignes car on a la memorisation
        for (int y=MEMORISATION-1;y<(MAP_LEN)-MEMORISATION;y++){
            for (int x=0;x<(MAP_WIDTH);x++){
                switch (tableau[y][x])
                {
                case MODEL_CHICKEN :
                    attron(COLOR_PAIR(1)); // active couleur jaune
                    mvaddch(y-MEMORISATION, x+1, tableau[y][x]);
                    attroff(COLOR_PAIR(1)); // desactive
                    break;
                case MODEL_GRASS:
                    attron(COLOR_PAIR(2)); // active couleur vert
                    mvaddch(y-MEMORISATION, x+1, tableau[y][x]);
                    attroff(COLOR_PAIR(2)); // desactive
                    break;

                case MODEL_CAR:
                    attron(COLOR_PAIR(3)); // active couleur rouge
                    mvaddch(y-MEMORISATION, x+1, tableau[y][x]);
                    attroff(COLOR_PAIR(3)); // desactive
                    break;
                
                default:
                    mvaddch(y-MEMORISATION,x+1,tableau[y][x]);
                    break;
                }
                
            }
        }
        for (int y=0;y<(MAP_LEN-2*MEMORISATION);y++){ // 2*memorisatio car on a la memo en haut et en bas
            mvaddch(y,0,ACS_VLINE);
            mvaddch(y,MAP_WIDTH+1,ACS_VLINE);
        }

        for (int x=0;x<(MAP_WIDTH+1);x++){
            mvaddch(0,x,ACS_HLINE);
            mvaddch(MAP_LEN-2*MEMORISATION,x,ACS_HLINE);
        }

        mvaddch(0,0,ACS_ULCORNER);
        mvaddch(0,MAP_WIDTH+1,ACS_URCORNER);
        mvaddch(MAP_LEN-2*MEMORISATION,0,ACS_LLCORNER);
        mvaddch(MAP_LEN-2*MEMORISATION,MAP_WIDTH+1,ACS_LRCORNER);

        
        refresh(); // on affiche la grille


        attron(A_BOLD); // GRAS
        mvprintw(1, MAP_WIDTH + 3, "POSITION JOUEUR : %f",p->h_position);
        attroff(A_BOLD);
        
        // Vérification des entrées utilisateur
        int ch = getch();
        if (ch != ERR) {
            if (ch == 'q' || ch == 'Q')
                break;
                
            
            
            switch(ch) {
                
                case KEY_UP:
                    if (jump_back>0){
                        jump_back--;
                    }
                    move_player(UP,p);
                    ground_move(g.board,UP);
                    
                    break;
                case KEY_DOWN:
                    if (jump_back < 3){
                        jump_back++;
                        move_player(DOWN,p);
                        ground_move(g.board,DOWN);
                    }
                    

                    break;
                case KEY_LEFT:
                    
                    move_player(LEFT,p);
                    ground_move(g.board,LEFT);
                    
                    break;
                case KEY_RIGHT:
                    
                    move_player(RIGHT,p);
                    ground_move(g.board,RIGHT);
                    break;
            }
            
            flushinp();
        }
        
        board_update(g.board,0.01); // pour instant = 1
        
        grid_tui_free(tableau);
        tableau=grid_tui_make(g.board);
        
        // Contrôle de la vitesse
        napms(5); // 10ms
    }
    
    // Nettoyage
    endwin();
    
    grid_tui_free(tableau);
    

    board_free(g.board);
    
    return 0;
}
