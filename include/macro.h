#ifndef MACRO_H
#define MACRO_H

//Définition des directions
#define LEFT 64
#define RIGHT 65
#define UP 66
#define DOWN 67
#define NEUTRAL 68

//Définition des tailles par défaut de la map, pour les tests uniquement
#define DEFAULT_CELL_SIZE 1.0
#define LEFT_MAP_X 0.0
#define RIGHT_MAP_X 8.0

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
#define GROUND_ROAD_CAR 91
#define GROUND_ROAD_TRUCKS 92
#define GROUND_WATER_LILY 93
#define GROUND_WATER_LOGS 94
#define GROUND_TRAIN 95

//Définition des objets

//Définition des attributs de voiture
#define CAR_LEN 1
#define CAR_MODEL '>'
#define CAR_TYPE 100
#define CAR_MAX_SPEED 4 //Vitesse maximum de la voiture en valeur absolue
#define CAR_MIN_SPEED 1 //Vitesse minimum de la voiture en valeur absolue
#define CAR_MIN_NUMBER 1 //Nombre minimum de voitures sur une ligne

//Définition des attributs de arbre
#define TREE_LEN 0.5
#define TREE_MODEL 'T'
#define TREE_TYPE 101

// Définition des types de collisions
#define COLLIDE_NONE 110
#define COLLIDE_DEADLY 111
#define COLLIDE_HARMLESS 112
#define COLLIDE_ERROR 113

// Définition des modèles de sol
#define MODEL_GRASS '_'
#define MODEL_ROAD '-'
#define MODEL_WATER '~'
#define MODEL_RAIL '='

// Définition des modèles d'objets
#define MODEL_CHICKEN 'i'
#define MODEL_TREE 'T'
#define MODEL_CAR '>'


//Définition des intervalles d'obstacles
#define INTER_CAR_MIN 3

//Définition de la taille de la map
#define MAP_WIDTH_GUI 19 // Largeur de la map en GUI
#define MAP_WIDTH_TXT 9 // Largeur de la map en TUI
#define MAP_LEN_GUI 25 // Hauteur de la map en GUI
#define MAP_LEN_TXT 13 // Hauteur de la map en TUI

#define MAP_WIDTH 9 // Largeur de la map actuelle
#define MAP_LEN 13 // Hauteur de la map actuelle

#define START_SIZE 7 // Taille de la zone de départ

#define V_POS MAP_LEN-3 // Position verticale du joueur sur le board

//Définition des seuils de difficulté
#define DIFF_EASY 25 // Le jeu devient plus dur au dessus de 25 de score
#define DIFF_NORMAL 50 // Le jeu devient plus dur au dessus de 50
#define DIFF_MEDIUM 75 // Le jeu devient plus dur au dessus de 75
#define DIFF_HARD 110 // Le jeu devient plus dur au dessus de 110

#endif
