#ifndef DEBUG_KIT_H
#define DEBUG_KIT_H


typedef struct debugKit_
{
   float game_speed; // sera utilisé dans board_update
   int god_mode; // 0 si pas activé et 1 si god_mod
   int display_grid_lines ; // 0 si pas actif 1 sinon
   int display_sprites; 
   int display_information; // active les infos de debug en haut a gauche
   int display_information_sprites; // active les infos des sprites
   int pouleria; // active l'ia (on pourra mettre son niveau)
   int deepness_ia; // profondeur de calcul
}debugKit;



#endif
