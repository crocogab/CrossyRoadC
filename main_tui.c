#include <ncurses.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

typedef struct _pos {
    int x;
    int y;
} pos;

typedef struct _player {
    char repr;
    pos posi;
} player;

void draw_visible_area(char **tableau, player p,int cam_x, int cam_y, int longueur, int largeur_cam, int hauteur_cam,int score) {
    clear();
    
    attron(A_BOLD); // GRAS
    mvprintw(1, largeur_cam + 3, "SCORE : %d",score);
    attroff(A_BOLD);
    
    
    // Dessiner les bordures horizontales
    for (int x = 0; x < largeur_cam; x++) {
        mvaddch(0, x, ACS_HLINE);                // bordure supérieure
        mvaddch(hauteur_cam - 1, x, ACS_HLINE);  // bordure inférieure
    }
    
    // Dessiner les bordures verticales
    for (int y = 0; y < hauteur_cam; y++) {
        mvaddch(y, 0, ACS_VLINE);                // bordure gauche
        mvaddch(y, largeur_cam - 1, ACS_VLINE);  // bordure droite
    }
    
    // Dessiner les coins
    mvaddch(0, 0, ACS_ULCORNER);                      // coin superieur gauche
    mvaddch(0, largeur_cam - 1, ACS_URCORNER);        // coin supérieur droit
    mvaddch(hauteur_cam - 1, 0, ACS_LLCORNER);        // coin inférieur gauche
    mvaddch(hauteur_cam - 1, largeur_cam - 1, ACS_LRCORNER); // coin inferieur droit
    
    // Dessiner le contenu visible
    for (int y = 1; y < hauteur_cam - 1; y++) {
        for (int x = 1; x < largeur_cam - 1; x++) {
            int world_y = y + cam_y;
            int world_x = x + cam_x;
            
            
            mvaddch(y, x, tableau[world_y][world_x]);
            
        }
    }
    
    int screen_y = p.posi.y - cam_y;
    int screen_x = p.posi.x - cam_x;

    
    mvaddch(screen_y, screen_x, p.repr); // on ajoute le joueur sans edit la grille
    
    
    
    refresh();
}

int main(void) {
    // camera à l'origine - cam_x est fixe à 0

    srand(time(NULL));// pour la génération du terrain

    int decal_cam=4;
    

    // Dimensions
    int hauteur_monde = 26*4+2;       // Hauteur totale du monde -> +2 pour les bordures
    int longueur_monde = 38+2;      // Largeur totale du monde
    int largeur_cam = 38+2;         // Largeur de la caméra (fenêtre visible)
    int hauteur_cam = 26+2;         // Hauteur de la caméra (fenêtre visible)


    int cam_x = 0;
    int cam_y = hauteur_monde-hauteur_cam; // on se met tout en bas


    // mvprintw moved after ncurses initialization

    // INITIALISATIONS
    char **tableau = NULL;
    tableau = malloc(sizeof(char *) * hauteur_monde);
    if (tableau == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    for (int i = 0; i < hauteur_monde; i++) {
        tableau[i] = malloc(sizeof(char) * longueur_monde);
        if (tableau[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            
            for (int j = 0; j < i; j++) {
                free(tableau[j]);
            }
            free(tableau);
            return 1;
        }
        
        // initialisations avec des points -> random pour tester deplacement
        for (int j = 0; j < longueur_monde; j++) {
            if (rand()%17==0){
                tableau[i][j]='+';
            }else{
                tableau[i][j] = '.';
            }
            
        }
    }
    
    // Initialisation du joueur
    player p;
    p.repr = '@';
    p.posi.x = 10;
    p.posi.y = hauteur_monde-2;


    
    
    // Configuration de ncurses
    initscr();
    curs_set(0);   // cache le curseur
    noecho();      // affiche pas les caractères tapés
    cbreak();      // désactive le buffering des lignes
    raw();
    typeahead(-1);
    clear();

    // Example text output after ncurses initialization
    
    
    // Initialisation des bordures dans le tableau
    for (int y = 0; y < hauteur_monde; y++) {
        for (int x = 0; x < longueur_monde; x++) {
            if (y == 0 || y == hauteur_monde-1) {
                tableau[y][x] = ACS_HLINE;
            } else if (x == 0 || x == longueur_monde-1) {
                tableau[y][x] = ACS_VLINE;
            }
        }
    }
    
    // Coins du tableau
    tableau[0][0] = ACS_ULCORNER;
    tableau[0][longueur_monde-1] = ACS_URCORNER;
    tableau[hauteur_monde-1][0] = ACS_LLCORNER;
    tableau[hauteur_monde-1][longueur_monde-1] = ACS_LRCORNER;
    
    
    
    // Configuration des touches et non-blocage
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    int score=0;
    int ligne_actu=0;
    // Boucle principale
    while (true) {
        
        // Dessiner la zone visible par la caméra
        draw_visible_area(tableau, p,cam_x, cam_y, longueur_monde, largeur_cam, hauteur_cam,score);
        
        // Vérification des entrées utilisateur
        int ch = getch();
        if (ch != ERR) {
            if (ch == 'q' || ch == 'Q')
                break;
                
            
            
            switch(ch) {
                
                case KEY_UP:
                    if (p.posi.y > 1 && tableau[p.posi.y-1][p.posi.x]=='.') { // pour instant interdit que +
                        p.posi.y--;
                        ligne_actu++;

                        if (ligne_actu>=score){
                            score=ligne_actu; // on met a jour le score
                        }
                        
                        // Déplacement de caméra sur l'axe Y
                        if (p.posi.y - cam_y < decal_cam && cam_y > 0) {
                            cam_y--;
                            
                        }
                        
                    }
                    break;
                case KEY_DOWN:
                    if (p.posi.y < hauteur_monde-2 && tableau[p.posi.y+1][p.posi.x]=='.') {
                        p.posi.y++;
                        ligne_actu--;
                        
                        // Déplacement de caméra sur l'axe Y
                        if (p.posi.y - cam_y > hauteur_cam - decal_cam && cam_y < hauteur_monde - hauteur_cam) {
                            cam_y++;
                           
                        }
                    }
                    break;
                case KEY_LEFT:
                    if (p.posi.x > 1 && tableau[p.posi.y][p.posi.x-1]=='.') {
                        p.posi.x--;
                                            
                    }
                    break;
                case KEY_RIGHT:
                    if (p.posi.x < longueur_monde-2 && tableau[p.posi.y][p.posi.x+1]=='.') {
                        p.posi.x++;
                        
                    }
                    break;
            }
            
            flushinp();
        }
        
        // Mise à jour de la position du joueur
        //tableau[p.posi.y][p.posi.x] = p.repr;
        
        // Contrôle de la vitesse
        napms(1); // 10ms
    }
    
    // Nettoyage
    endwin();
    
    for (int i = 0; i < hauteur_monde; i++) {
        free(tableau[i]);
    }
    free(tableau);
    
    return 0;
}