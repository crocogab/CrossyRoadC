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
#define RIGHT_MAP_X 17.0

//Définition des status de la game
#define HOME 70 //Le menu d'acceuil est affiché
#define PLAYING 71 //La game est en cours
#define TO_LAUNCH 72 //Le jeu est en attente d'une entrée utilisateur (avant lancement de la partie)
#define DEAD 73 //L'écran de mort est affiché
#define PAUSED 74 // Le jeu est en pause
#define SETTINGS 75 //Le menu des paramètres est affiché

//Définition des types d'obstacles
#define TYPE_VIDE 80

//Définition des types de sols
#define GROUND_GRASS 90
#define GROUND_ROAD_CAR 91
#define GROUND_ROAD_TRUCK 92
#define GROUND_WATER_LILY 93
#define GROUND_WATER_LOG 94
#define GROUND_TRAIN 95
#define GROUND_ROAD_BORDER 96 //Utile pour gui

//Définition des objets

//Définition des attributs de voiture
#define CAR_LEN 1
#define CAR_TYPE 100
#define CAR_MAX_SPEED 4 //Vitesse maximum de la voiture en valeur absolue
#define CAR_MIN_SPEED 1 //Vitesse minimum de la voiture en valeur absolue
#define CAR_MIN_NUMBER 1 //Nombre minimum de voitures sur une ligne

//Définition des attributs de arbre
#define TREE_LEN 0.5
#define TREE_TYPE 101

//Définition des attributs de rock
#define ROCK_LEN 0.5
#define ROCK_TYPE 102

//Définition des attributs de water lily
#define WATER_LILY_LEN 0.5
#define WATER_LILY_TYPE 103

//Définition des attributs de truck
#define TRUCK_LEN 3
#define TRUCK_TYPE 104
#define TRUCK_MAX_SPEED 4 //Vitesse maximum du camion en valeur absolue
#define TRUCK_MIN_SPEED 1 //Vitesse minimum du camion en valeur absolue
#define TRUCK_MIN_NUMBER 1 //Nombre minimum du camion sur une ligne

// Définition des attributs de log
#define LOG_LEN 4
#define LOG_TYPE 105
#define LOG_MAX_SPEED 3
#define LOG_MIN_SPEED 1 
#define LOG_MIN_NUMBER 2

// Définition des attributs de train
#define TRAIN_TYPE 106
#define TRAIN_MIN_SPEED 40
#define TRAIN_MAX_SPEED 40
#define TRAIN_MIN_TIME 700
#define TRAIN_MAX_TIME 700
#define TRAIN_LEN MAP_WIDTH*2 //Taille d'un wagon
#define TRAIN_ALERT_TIME 250 // temps pour lequel on avertit de l'arrivée d'un train

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
#define MODEL_CHICKEN '@'
#define MODEL_TREE 'T'
#define MODEL_CAR '>'
#define MODEL_ROCK '#'
#define MODEL_WATER_LILY 'W'
#define MODEL_TRUCK 'D'
#define MODEL_LOG '&'
#define MODEL_TRAIN ')'
#define MODEL_INCOMING_TRAIN '}'


//Définition des intervalles d'obstacles
#define INTER_CAR_MIN 6
#define INTER_TRUCK_MIN 8
#define INTER_LOG_MIN 8

//Définition de la taille de la map
#define MAP_WIDTH_GUI 19 // Largeur de la map en GUI
#define MAP_WIDTH_TXT 9 // Largeur de la map en TUI
#define MAP_LEN_GUI 25 // Hauteur de la map en GUI
#define MAP_LEN_TXT 13 // Hauteur de la map en TUI

#define MAP_WIDTH 18 // Largeur de la map actuelle
#define MAP_LEN 26 // Hauteur de la map actuelle

#define START_SIZE 7 // Taille de la zone de départ

#define V_POS 7 // Position verticale du joueur sur le board

//Définition des seuils de difficulté
#define DIFF_EASY 25 // Le jeu devient plus dur au dessus de 25 de score
#define DIFF_NORMAL 50 // Le jeu devient plus dur au dessus de 50
#define DIFF_MEDIUM 75 // Le jeu devient plus dur au dessus de 75
#define DIFF_HARD 110 // Le jeu devient plus dur au dessus de 110

//Définition de la "taille" de la mémorsation
#define MEMORISATION 3

//Définition des positions de la sprite_sheet
#define BLUE_CAR_ID 0
#define BLUE_TRUCK_ID 1
#define CHICKEN_ID 2
#define COIN_ID 3
#define GAZ_TRUCK_ID 4
#define LILYPAD_ID 5
#define LOG_ID 6
#define ORANGE_CAR_ID 7
#define PURPLE_CAR_ID 8
#define RAIL_ID 9
#define RED_TRUCK_ID 10
#define ROCK_ID 11
#define TRAIN_ID 12
#define TRAIN_POLE_ID 13
#define TREE_ID 14

#define BLUE_CAR_NB 2
#define BLUE_TRUCK_NB 2
#define CHICKEN_NB 4
#define COIN_NB 1
#define GAZ_TRUCK_NB 2
#define LILYPAD_NB 3
#define LOG_NB 4
#define ORANGE_CAR_NB 2
#define PURPLE_CAR_NB 2
#define RAIL_NB 1
#define RED_TRUCK_NB 2
#define ROCK_NB 2
#define TRAIN_NB 1
#define TRAIN_POLE_NB 2
#define TREE_NB 5


#endif
