#ifndef MACRO_H
#define MACRO_H

//Définition des directions
#define LEFT 64
#define RIGHT 65
#define UP 66
#define DOWN 67
#define NEUTRAL 68

//Définition des tailles par défaut de la map, pour les tests uniquement
#define DEFAULT_CELL_SIZE 10.0
#define LEFT_MAP_X 0.0
#define RIGHT_MAP_X 100.0

//Définition des status de la game
#define HOME 70 //Le menu d'acceuil est affiché
#define PLAYING 71 //La game est en cours
#define TO_LAUNCH 72 //Le jeu est en attente d'une entrée utilisateur (avant lancement de la partie)
#define DEAD 73 //L'écran de mort est affiché
#define PAUSED 74 // Le jeu est en pause
#define SETTINGS 75 //Le menu des paramètres est affiché

//Définition des types d'obstacles
#define TYPE_VIDE 80
#define TYPE_VOITURE 81

//Définition des types de sols
#define GROUND_GRASS 90
#define GROUND_ROAD 91
#define GROUND_WATER 92
#define GROUND_TRAIN 93

//Définition des intervalles d'obstacles
#define INTER_VOITURE_MAX 12

//Définition de la taille de la map
#define MAP_WIDTH_GUI 19 // Largeur de la map en GUI
#define MAP_WIDTH_TXT 9 // Largeur de la map en TUI
#define MAP_LEN_GUI 25 // Hauteur de la map en GUI
#define MAP_LEN_TXT 13 // Hauteur de la map en TUI
#define V_POS 15 // Position verticale du joueur sur la board


#endif
