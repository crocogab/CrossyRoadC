#include <ncurses.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct _pos
{
    int x;
    int y;
}pos;

typedef struct _player
{
    char repr;
    pos posi;
}player;



int main() {
    // A FAIRE -> AVOIR UNE LIGNE PLUS GRANDE ET DEPLACER LA CAM
    // GERER LA GRILLE COMME UNE GRILLE EN DESSINANT TOUT -> tout simplement on parcourt tout et on check ce qui ya dans la case

    int longueur = 19+2; // +2 pour les bordures
    int largeur = 13+2;

    player p;
    p.repr='@';
    
    pos posi;
    posi.x=10;
    posi.y=10;

    p.posi=posi;

    initscr();

    
    curs_set(0);  // cache le cursor
    noecho();     // montre pas les char tapes
    cbreak();     // on desatcive buffering des lignes

    raw();
    typeahead(-1);

    
    clear();
    
    
    for (int y = 0; y < largeur; y++) {
        for (int x = 0; x < longueur; x++) {
            if (y == 0 || y == largeur-1) {
                mvaddch(y, x, ACS_HLINE); // bordure horizontale
            } else if (x == 0 || x == longueur-1) {
                mvaddch(y, x, ACS_VLINE); // bordure verticale
            } else {
                mvaddch(y, x, '.'); // je pourrais utiliser le tableau a la place des points
            }
        }
    }


    
    
   
    mvaddch(0, 0, ACS_ULCORNER);               // des corners pour plus beau
    mvaddch(0, longueur-1, ACS_URCORNER);      
    mvaddch(largeur-1, 0, ACS_LLCORNER);       
    mvaddch(largeur-1, longueur-1, ACS_LRCORNER); 
    
    keypad(stdscr, TRUE); // pour avoir des input
    nodelay(stdscr, TRUE);
    
    while (true) {
        // on check si ya un input
        int ch = getch();
        
        if (ch != ERR) {  // ERR = pas de key
            if (ch == 'q' || ch == 'Q') 
                break;
            
            //on nettoie ancienne pos
            mvaddch(p.posi.y, p.posi.x, '.');
            
            
            switch(ch) {
                case KEY_UP:
                    if (p.posi.y > 1) p.posi.y--;
                    break;
                case KEY_DOWN:
                    if (p.posi.y < largeur-2) p.posi.y++;
                    break;
                case KEY_LEFT:
                    if (p.posi.x > 1) p.posi.x--;
                    break;
                case KEY_RIGHT:
                    if (p.posi.x < longueur-2) p.posi.x++;
                    break;
            }
            flushinp();
        }
        
        // on dessine
        mvaddch(p.posi.y, p.posi.x, p.repr);
        
        // boucle d'action
        // ...
        
        
        
        // vitesse
        usleep(10000);
        refresh();
    }
    
    
    
    // on nettoie
    endwin();
    return 0;
}