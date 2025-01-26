# Développement d'une application web pour le jeu **Blokus** avec IA


## Objectif du projet

Ce projet doit vous permettre de mettre en œuvre des compétences en développement web, en bases de données, en intelligence artificielle, et en gestion de projet, tout en étant un défi ludique et pratique avec la conception d'un jeu de société.

L'objectif de ce projet est de concevoir et de développer une application web permettant de jouer au jeu de plateau **Blokus** en tour par tour. L'application devra permettre à plusieurs joueurs humains de participer à une partie, soit co-localisés (dans le même navigateur internet), soit à distance (depuis différentes machines à travers plusieurs navigateurs internet), et inclure au moins un joueur virtuel contrôlé par un algorithme d'intelligence artificielle (IA). Les élèves devront réaliser une application complète en Python, avec une partie serveur (*back-end*) gérant la logique du jeu et une interface utilisateur interactive (*front-end*).


![Blokus](figures/blokus.jpg)

## Objectifs Pédagogiques

- Implémentation de structures de base et d'algorithmes avancés.
- Application pratique des connaissances en base de données et programmation Web.
- Développement et application de tests unitaires pour assurer la qualité et la fiabilité du code.
- Expérience concrète de la gestion de projet en équipe (planification, suivi d'avancement, compte rendu de réunion, etc.).
- Mise en pratique de la gestion de versions (utilisation de git/GitLab)


## Cahier des charges

#### Fonctionnalités requises
   - **Gestion des parties** : Permettre la création de nouvelles parties et l'ajout de joueurs (humains ou IA).
   - **Tour par tour** : Implémentation du déroulement des parties en mode tour par tour, avec les vérifications de règles associées (validité des coups, placements corrects des pièces).
   - **Reprendre une partie** : La possibilité de sauvegarder et de reprendre une partie en cours.
   - **Historique des parties** : Un accès à l’historique complet des coups pour chaque partie.
   - **IA pour un joueur virtuel** : Développer un algorithme d'intelligence artificielle qui pourra participer à la partie en tant que joueur virtuel, avec des choix de coups basés sur des algorithmes tels que Minimax ou Alpha-Beta Pruning.


#### Technologies
   - **Back-end** : Utilisation de **Flask** (framework Python) pour implémenter la logique du jeu Blokus, la gestion des parties et des joueurs, ainsi que les interactions avec la base de données.
   - **Base de données** : Concevoir un schéma relationnel en 3ème forme normale pour stocker les informations de jeu (joueurs, parties, historique des coups). Toutes les informations relatives aux parties (joueurs, positions des pièces, historique des coups) seront stockées dans une base de données relationnelle (SQLite, PostgreSQL, ou MySQL).
   - **Front-end** : Créer une interface web interactive avec HTML/CSS/(JavaScript en option) permettant de représenter le plateau de jeu, les pièces disponibles pour chaque joueur et l'historique des coups.


## Étapes du projet

#### Étude préalable
   - Réaliser un **état de l’art** des algorithmes d'intelligence artificielle utilisés dans les jeux de stratégie (algorithmes Minimax, Alpha-Beta Pruning, etc.) afin de déterminer le plus adapté pour implémenter un joueur virtuel compétent.
   - Décrire le schéma conceptuel de la base de données pour modéliser les parties, les joueurs et l'historique des coups.
   - Proposer un mécanisme pour permettre "le jeu en tour par tour".

#### Conception de la base de données
   - Élaboration d’un conceptuel de données **modèle entité-association** et de son **modèle relationnel** associé en 3ème forme normale.
   - Définir les relations entre les parties, les joueurs, les coups joués et leur stockage.

#### Développement de l’application
   - **Back-end** : Implémentation de la logique du jeu, des règles de Blokus, et des interactions avec la base de données à l’aide de Flask.
   - **IA** : Implémenter un ou plusieurs algorithmes pour le joueur virtuel prenant en compte le niveau de difficulté et/ou les temps de réflexion/calcul.
   - **Front-end** : Conception de l’interface utilisateur pour visualiser le plateau de jeu, placer les pièces et interagir avec les autres joueurs.
   
#### Documentation et tests
   - Documentation complète du code et des choix d'implémentation (structure de la base de données, algorithmes utilisés, logique de jeu).
   - Tests unitaires et fonctionnels pour s'assurer que l'application respecte les règles du jeu et gère correctement les scénarios (annulation de coups, parties sauvegardées, etc.).


##  Exigences du projet et points évalués

Pour l'évaluation, les points suivants seront pris en considération :

- Qualité du **modèle relationnel** et respect de la 3ème forme normale.
- Implémentation correcte de la **logique du jeu** et respect des règles de Blokus.
- Fonctionnalité de l'**intelligence artificielle** (qualité de l'algorithme choisi et performances).
- **Qualité de l'interface utilisateur** et fluidité des interactions avec le jeu.
- Respect des **bonnes pratiques de programmation** (structure du code, commentaires, organisation).
- Tests et gestion des erreurs (robustesse de l’application).


## Rendu Final

Le rendu final sera constitué a minima des éléments suivants :
- **Code source** : Livraison du code source complet et des instructions pour exécuter celui-ci. 
- **État de l'art** : Rapport de l’état de l’art sur les algorithmes d’intelligence artificielle applicables et appliqués.
- **Documentation** : Comprend une description du jeu, des détails techniques et tous les éléments de gestion de projet que vous aurez produits (fiche de projet, comptes-rendus de réunion, planification et répartition des tâches, analyse post-mortem des efforts individuels et de l'atteinte des objectifs, etc.).
- **Tests unitaires** : Un ensemble de tests unitaires accompagnant le code source. 

Tous ces éléments seront fournis sur le dépôt GitLab du projet.


## Date de rendu et Soutenance

Le projet est à rendre pour le **mardi 7 janvier 2025** à 22 heures au plus tard.

Des soutenances de groupes de projet seront organisées la deuxième ou troisième semaine de janvier.

Votre projet fera l'objet d'une démonstration devant un jury composé d'au moins 2 membres de l’équipe pédagogique. Durant cette soutenance, vous serez jugés sur votre démonstration de l'application et votre capacité à expliquer votre projet et son fonctionnement. Chaque membre du groupe devra être présent lors de soutenance et **participer activement**.

*Toute personne ne se présentant pas à la soutenance sera considérée comme démissionnaire de l'UE et en conséquence, ne pourra pas la valider pour l’année universitaire 2024-2025.*

Il est attendu que chaque membre du groupe ait contribué à plusieurs parties fonctionnelles du code (il ne s'agit pas d'avoir uniquement corrigé quelques lignes par ci et par là).


## Développement incrémental

Il est vivement recommandé à ce que le groupe adopte une stratégie de développement incrémentale.

L'idée est donc de planifier et de définir des "incréments" ou de petites unités fonctionnelles du jeu (ou de ces composants). Cela permet de se concentrer sur une petite section du jeu à la fois et d'être toujours capable d'avoir une version fonctionnelle du jeu complet. Cela permet également d'éviter l'effet tunnel : de commencer le développement de beaucoup de fonctionnalités et de n'avoir finalement rien ou pas grand chose de fonctionnel à montrer à la fin du projet.


## Fraude, tricherie et plagiat

Ne trichez pas ! Ne copiez pas ! Ne plagiez pas ! Si vous le faites, vous serez lourdement sanctionnés. Nous ne ferons pas de distinction entre copieur et copié. Vous n'avez pas de (bonnes) raisons de copier. De même, vous ne devez pas utiliser de solution clé en main trouvée sur internet.

Par tricher, nous entendons notamment :
- Rendre le travail d’un collègue en y apposant votre nom ;
- Obtenir un code, une solution par un moteur de recherche (ou une IA) et la rendre sous votre nom ;
- Récupérer du code et ne changer que les noms de variables et fonctions ou leur ordre avant de les présenter sous votre nom 
- Autoriser consciemment ou inconsciemment un collègue à s'approprier votre travail personnel. Assurez-vous notamment que votre projet et ses différentes copies locales ne soient lisibles que par vous et les membres de votre groupe.

Nous encourageons les séances de *brainstorming* et de discussion entre les élèves sur le projet. C’est une démarche naturelle et saine comme vous la rencontrerez dans votre vie professionnelle. Si les réflexions communes sont fortement recommandées, vous ne pouvez rendre que du code et des documents écrits par vous-même. Vous indiquerez notamment dans votre rapport toutes vos sources (comme les sites internet que vous auriez consultés), en indiquant brièvement ce que vous en avez retenu.
Il est quasi certain que nous détections les tricheries. En effet, les rapports et les codes sont systématiquement soumis à des outils de détection de plagiat et de copie. Il existe notamment des outils de détection de manipulation de code extraordinaire mis à disposition par l’Université de Stanford, tels que `MOSS` (https://theory.stanford.edu/~aiken/moss/) ou `compare50` (https://cs50.readthedocs.io/projects/compare50/). De plus, chacun a son propre style de programmation et personne ne développe la même chose de la même manière.

Puisqu'il s'agit d'un projet réalisé dans le cadre de cours avancés de programmation, nous nous attendons à ce que vous soyez capable d'apprendre à déboguer des programmes par vous-même. Par exemple, demander à un autre élève de regarder directement votre code et de donner des suggestions d'amélioration commence à devenir un peu délicat au niveau éthique.

Si vous rencontrez des difficultés pour terminer une tâche, veuillez contacter l'un de vos enseignants afin que nous puissions vous aider. Nous préférons de loin passer du temps à vous aider plutôt que de traiter des cas de fraudes.

