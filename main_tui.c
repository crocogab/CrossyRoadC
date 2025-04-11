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
 * Affiche un écran de fin de partie avec l'option de quitter uniquement.
 * La fonction attend que l'utilisateur appuie sur 'q' pour quitter.
 * 
 * @param score Le score final du joueur
 * @return 1 si l'utilisateur quitte, 0 sinon (bien que cette fonction ne retourne que 1)
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
    
    
    attron(A_BOLD);
    mvprintw(center_y + 2, center_x - 14, "Appuyez sur 'q' pour quitter");
    attroff(A_BOLD);
    
    // Carré autour 
    for (int y = center_y - 6; y <= center_y + 4; y++) {
        for (int x = center_x - 16; x <= center_x + 16; x++) {
            if (y == center_y - 6 || y == center_y + 4) {
                mvaddch(y, x, ACS_HLINE);
            }
            else if (x == center_x - 16 || x == center_x + 16) {
                mvaddch(y, x, ACS_VLINE);
            }
        }
    }
    
    
    mvaddch(center_y - 6, center_x - 16, ACS_ULCORNER);
    mvaddch(center_y - 6, center_x + 16, ACS_URCORNER);
    mvaddch(center_y + 4, center_x - 16, ACS_LLCORNER);
    mvaddch(center_y + 4, center_x + 16, ACS_LRCORNER);
    
    
    refresh();
    
    
    nodelay(stdscr, FALSE); // mode bloquant
    
    
    int ch;
    do {
        ch = getch();
    } while (ch != 'q' && ch != 'Q');
    
    // no bloquanr avant de quitter
    nodelay(stdscr, TRUE);
    
    return 1; 
}

int main(void) {

    //Initialisation de l'aléatoire
    srand(time(NULL));

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

    //couleurs personnalisées
    init_color(8, 20, 255, 0); // vert_arbre
    init_color(9,500,500,500);


    init_pair(1, COLOR_YELLOW, COLOR_BLACK); // jaune sur fond noir
    init_pair(2, COLOR_GREEN, COLOR_BLACK);  // vert sur fond noir
    init_pair(3, COLOR_RED, COLOR_BLACK);    // rouge sur fond noir
    init_pair(4,8,COLOR_BLACK); // vert_arbre
    init_pair(5,9,COLOR_BLACK); //rocher
    
    clear();

    
    game_start(&g); // pour initialiser le tasbleau
    
    board_set_player(g.board,p);
    
    char ** tableau = grid_tui_make(g.board);
    printf("%c",tableau[0][0]);
    
    
    
    // Configuration des touches et non-blocage
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    
    int jump_back = 0; // on va limiter le nombre de retours en arriere
    int score_actu = 0;
    int score_maxi =0; 
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
                case MODEL_TREE:
                    attron(COLOR_PAIR(4)); // active couleur vert_arbre
                    mvaddch(y-MEMORISATION, x+1, tableau[y][x]);
                    attroff(COLOR_PAIR(4)); // desactive
                    break;
                case MODEL_ROCK:
                    
                    attron(COLOR_PAIR(5)); // active couleur roche
                    mvaddch(y-MEMORISATION, x+1, tableau[y][x]);
                    attroff(COLOR_PAIR(5)); // desactive
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
        mvprintw(1, MAP_WIDTH + 3, "[DEBUG] POSITION JOUEUR : %f",p->h_position);

        mvprintw(2, MAP_WIDTH + 3, "[DEBUG] JOUEUR EN VIE: %d",p->alive);
        mvprintw(3, MAP_WIDTH + 3, "[DEBUG] CHECK_FUTURE_COLLISION (TOP): %d",check_future_collision(g.board,UP));
        mvprintw(4, MAP_WIDTH + 3, "[DEBUG] CHECK_FUTURE_COLLISION (DOWN): %d",check_future_collision(g.board,DOWN));
        mvprintw(5, MAP_WIDTH + 3, "[DEBUG] CHECK_FUTURE_COLLISION (LEFT): %d",check_future_collision(g.board,LEFT));
        mvprintw(6, MAP_WIDTH + 3, "[DEBUG] CHECK_FUTURE_COLLISION (RIGHT): %d",check_future_collision(g.board,RIGHT));

        mvprintw(7, MAP_WIDTH + 3, "[DEBUG] SCORE : %d",score_maxi);


        attroff(A_BOLD);
        
        board_update(g.board,0.01); 
        
        // Vérification des entrées utilisateur
        int ch = getch();
        if (ch != ERR) {
            if (ch == 'q' || ch == 'Q')
                break;
                
            
            
            switch(ch) {
                
                case KEY_UP:

                    switch (check_future_collision(g.board,UP)){
                        case COLLIDE_NONE:
                            score_actu++;
                            if (jump_back>0){
                                jump_back--;
                            }
                            move_player(UP,p);
                            ground_move(g.board,UP);
                            break;
                        case COLLIDE_DEADLY:
                            
                            p->alive=false;
                            g.status=DEAD;
                            if (display_game_over(score_maxi)==1){
                                endwin(); // fermer ncurses
                                exit(0);

                            }
                            break;
                        case COLLIDE_HARMLESS:
                            // il ne se passe rien car on ne peut pas avancer
                            break;
                        
                        }
                    
                    
                    break;
                case KEY_DOWN:
                    

                    if (jump_back < 3){
                        switch (check_future_collision(g.board,DOWN))
                        {
                            case COLLIDE_NONE:
                                // on peut descendre
                                score_actu--;
                                jump_back++;
                                move_player(DOWN,p);
                                ground_move(g.board,DOWN);
                                break;
                    
                            case COLLIDE_DEADLY:
                                // A CHANGER -> CHARGER ECRAN MORT
                                
                                p->alive=false;
                                g.status=DEAD;
                                if (display_game_over(score_maxi)==1){
                                    endwin(); // fermer ncurses
                                    exit(0);

                                }
                                break;
                            case COLLIDE_HARMLESS:
                                // il ne se passe rien car on ne peut pas avancer
                                break;
                        }
                        
                    }
                    

                    break;
                case KEY_LEFT:
                    switch (check_future_collision(g.board,LEFT))
                    {
                    case COLLIDE_NONE:
                        move_player(LEFT,p);
                        ground_move(g.board,LEFT);
                        break;
                    
                    case COLLIDE_DEADLY:
                        
                        p->alive=false;
                        g.status=DEAD;
                        if (display_game_over(score_maxi)==1){
                            endwin(); // fermer ncurses
                            exit(0);

                        }
                        break;
                    case COLLIDE_HARMLESS :
                        // se passe rien
                        break;
                    }
                    
                    
                    
                    break;
                case KEY_RIGHT:
                    switch (check_future_collision(g.board,RIGHT))
                    {
                    case COLLIDE_NONE:
                        move_player(RIGHT,p);
                        ground_move(g.board,RIGHT);
                        break;
                    
                    case COLLIDE_DEADLY:
                        p->alive=false;
                        g.status=DEAD;
                        if (display_game_over(score_maxi)==1){
                            endwin(); // fermer ncurses
                            exit(0);

                        }
                        break;
                    case COLLIDE_HARMLESS :
                        // se passe rien
                        break;
                    }
                    break;
                default:
                    switch (check_future_collision(g.board,NEUTRAL))
                    {
                        case COLLIDE_DEADLY:
            
                            p->alive=false;
                            g.status=DEAD;
                            if (display_game_over(score_maxi)==1){
                                endwin(); // fermer ncurses
                                exit(0);
                            }
                            break;
        
                        default:
                            break;
                    }
                    break;

            }
            
            flushinp();
        }
        else{
            switch (check_future_collision(g.board,NEUTRAL))
            {
                case COLLIDE_DEADLY:
            
                    p->alive=false;
                    g.status=DEAD;
                    if (display_game_over(score_maxi)==1){
                        endwin(); // fermer ncurses
                        exit(0);
                    }
                    break;
        
                default:
                break;
            }

        }

        // UPDATE SI LE JOUEUR A PAS BOUGE
        

        // On met a jour le score
        if (score_actu>score_maxi){
            score_maxi=score_actu;
        }
        
        
        
        
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
