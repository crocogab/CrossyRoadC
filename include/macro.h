#ifndef MACRO_H
#define MACRO_H

//Définition des directions
#define LEFT 64
#define RIGHT 65
#define UP 66
#define DOWN 67
#define NEUTRAL 68

//Définition de la taille de la map

#define MAP_WIDTH 24 // Largeur de la map actuelle
#define MAP_LEN 35 // Hauteur de la map actuelle

#define MAP_WIDTH_CAM 10
//Définition des tailles par défaut de la map, pour les tests uniquement
#define DEFAULT_CELL_SIZE 121
#define LEFT_MAP_X (DEFAULT_CELL_SIZE * (MAP_WIDTH - MAP_WIDTH_CAM) / 2)
#define RIGHT_MAP_X (MAP_WIDTH * DEFAULT_CELL_SIZE - DEFAULT_CELL_SIZE * (MAP_WIDTH - MAP_WIDTH_CAM) / 2)

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
#define CAR_TYPE 100
#define CAR_MAX_SPEED 8 //Vitesse maximum de la voiture en valeur absolue
#define CAR_MIN_SPEED 4 //Vitesse minimum de la voiture en valeur absolue
#define CAR_MIN_NUMBER 3 //Nombre minimum de voitures sur une ligne

//Définition des attributs de arbre
#define TREE_LEN 1*DEFAULT_CELL_SIZE
#define TREE_TYPE 101

//Définition des attributs de rock
#define ROCK_LEN 1*DEFAULT_CELL_SIZE
#define ROCK_TYPE 102

#define TRAIN_POLE_TYPE 107
#define TRAIN_POLE_LEN 0

//Définition des attributs de water lily
#define WATER_LILY_LEN 1*DEFAULT_CELL_SIZE
#define WATER_LILY_TYPE 103

//Définition des attributs de truck
#define TRUCK_TYPE 104
#define TRUCK_MAX_SPEED 5 //Vitesse maximum du camion en valeur absolue
#define TRUCK_MIN_SPEED 3 //Vitesse minimum du camion en valeur absolue
#define TRUCK_MIN_NUMBER 1 //Nombre minimum du camion sur une ligne

// Définition des attributs de log
#define LOG_TYPE 105
#define LOG_MAX_SPEED 2
#define LOG_MIN_SPEED 1.5
#define LOG_MIN_NUMBER 2

// Définition des attributs de train
#define TRAIN_TYPE 106
#define TRAIN_MIN_SPEED 50
#define TRAIN_MAX_SPEED 50
#define TRAIN_MIN_TIME 700
#define TRAIN_MAX_TIME 700
#define TRAIN_ALERT_TIME 250 // temps pour lequel on avertit de l'arrivée d'un train

// Définition des types de collisions
#define COLLIDE_NONE 110
#define COLLIDE_DEADLY 111
#define COLLIDE_HARMLESS 112
#define COLLIDE_ERROR 113


//Définition des intervalles d'obstacles
#define INTER_CAR_MIN 5*DEFAULT_CELL_SIZE
#define INTER_TRUCK_MIN 6*DEFAULT_CELL_SIZE
#define INTER_LOG_MIN 8*DEFAULT_CELL_SIZE

#define START_SIZE 4 // Taille de la zone de départ (PETITE = GRAND DEPART)

#define V_POS 10 // Position verticale du joueur sur le board

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
#define LOG_LONG_ID 6
#define LOG_MEDIUM_ID 7
#define LOG_SMALL_ID 8
#define ORANGE_CAR_ID 9
#define PURPLE_CAR_ID 10
#define RAIL_ID 11
#define RED_TRUCK_ID 12
#define ROCK_ID 13
#define TRAIN_ID 14
#define TRAIN_POLE_ID 15
#define TREE_ID 16

#define BLUE_CAR_NB 2
#define BLUE_TRUCK_NB 2
#define CHICKEN_NB 4
#define COIN_NB 1
#define GAZ_TRUCK_NB 2
#define LILYPAD_NB 3
#define LOG_LONG_NB 2
#define LOG_MEDIUM_NB 1
#define LOG_SMALL_NB 1
#define ORANGE_CAR_NB 2
#define PURPLE_CAR_NB 2
#define RAIL_NB 1
#define RED_TRUCK_NB 2
#define ROCK_NB 2
#define TRAIN_NB 1
#define TRAIN_POLE_NB 2
#define TREE_NB 5


#endif
