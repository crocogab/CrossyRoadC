# Crossy Road - Version TUI

Ce projet est une réimplémentation du célèbre jeu **Crossy Road**, dans une première version jouable en terminal (TUI) à l'aide de la bibliothèque `ncurses`. Une version avec interface graphique (GUI) est également prévue prochainement.

## Objectifs

- Reproduire les mécaniques de **Crossy Road** dans un environnement en ligne de commande.
- Utiliser `ncurses` pour gérer l'affichage et les interactions dans le terminal.
- Développer une base solide pour migrer ensuite vers une interface graphique.

## Fonctionnalités

- Affichage en temps réel dans le terminal
- Déplacement du joueur
- Gestion des routes et des obstacles
- Rafraîchissement dynamique de l’écran
- Contrôles clavier (flèches directionnelles)

## Installation

### Prérequis

Assurez-vous d’avoir les paquets suivants installés :

```bash
sudo apt update
sudo apt install libncurses-dev build-essential
```

### Compilation

Clonez le projet et compilez avec make :
```bash
git clone https://gibson.telecomnancy.univ-lorraine.fr/projets/2425/ppii-s6/ppii-s6-grp-26.git
cd ppii-s6-grp-26
make
```

### Tests

Exécuter les tests : 
```bash
make run_tests
```

### Nettoyer le projet

Nettoyer les fichiers de compilation : 
```bash
make clean
```

## Exécution

### Lancer la version TUI

```bash
make run_tui
```

## Structure du projet

```bash
.
├── include
    ├── board.h
    ├── game.h 
    ├── ground.h  
    ├── macro.h  
    ├── obstacle.h
    ├── player.h  
    ├── random_custom.h 
├── src
    ├── board.c
    ├── game.c 
    ├── ground.c    
    ├── obstacle.c
    ├── player.c  
    ├── random_custom.c
├── Tests
    ├── test_board.c
    ├── test_ground.c 
    ├── test_obstacle.c  
    ├── test_player.c 
    ├── test_random_custom.c
├── Makefile         
├── README.md        
├── .gitignore
├── .gitlab-ci.yml
├── main_tui.c  
```