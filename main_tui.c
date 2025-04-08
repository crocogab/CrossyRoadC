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
    clear();

    
    game_start(&g); // pour initialiser le tasbleau
    
    board_set_player(g.board,p);
    
    char ** tableau = grid_tui_make(g.board);
    printf("%c",tableau[0][0]);
    
    
    
    // Configuration des touches et non-blocage
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    
    // Boucle principale
    while (true) {
        
        clear(); //on clean tout
        
        // Dessiner la zone visible par la caméra
        for (int y=0;y<(MAP_LEN);y++){
            for (int x=0;x<(MAP_WIDTH);x++){
                mvaddch(y+1,x+1,tableau[y][x]);
            }
        }
        for (int y=0;y<(MAP_LEN);y++){
            mvaddch(y+1,0,ACS_VLINE);
            mvaddch(y+1,MAP_WIDTH,ACS_VLINE);
        }

        for (int x=0;x<(MAP_WIDTH);x++){
            mvaddch(0,x,ACS_HLINE);
            mvaddch(MAP_LEN,x+1,ACS_HLINE);
        }

        mvaddch(0,0,ACS_ULCORNER);
        mvaddch(0,MAP_WIDTH,ACS_URCORNER);
        mvaddch(MAP_LEN,0,ACS_LLCORNER);
        mvaddch(MAP_LEN,MAP_WIDTH,ACS_LRCORNER);

        
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
                    ground_move(g.board,UP);
                    move_player(UP,p);
                    break;
                case KEY_DOWN:

                    break;
                case KEY_LEFT:
                    ground_move(g.board,LEFT);
                    move_player(LEFT,p);
                    break;
                case KEY_RIGHT:
                    
                    break;
            }
            
            flushinp();
        }
        
        board_update(g.board,1); // pour instant = 1
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
