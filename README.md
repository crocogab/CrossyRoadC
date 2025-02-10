# Développement d'une application de jeu **Crossy Road** avec IA


## Objectif du projet

Ce projet doit vous permettre de mettre en œuvre des compétences en développement C, en implémentation de structures de données, en utilisation d'une librairie graphique externe, en intelligence artificielle et en gestion de projet, tout en étant un défi ludique et pratique avec la conception d'un jeu.

L'objectif de ce projet est de concevoir et de développer une application permettant de jouer au jeu électronique **Crossy Road**. 
L'application devra permettre à un joueur humain (dans un premier temps puis à une IA) de jouer une partie. 
Les élèves devront réaliser une application complète en langage C suivant deux réalisations :
- Une version intégralement textuelle avec une interaction avec le joueur par ligne de commande et
- Une version graphique utilisant la librairie libSDL (https://www.libsdl.org/)

![Crossy Road](crossy-road-revenue.webp)


## Objectifs Pédagogiques

- Élaborer un cahier des charges
- Modéliser un jeu
- Implémentation de structures de données et d'algorithmes avancés en langage C.
- Développement et application de tests unitaires pour assurer la qualité et la fiabilité du code.
- Expérience concrète de la gestion de projet en équipe (planification, suivi d'avancement, compte rendu de réunion, etc.).
- Mise en pratique de la gestion de versions (utilisation de git/GitLab)


## Cahier des charges

#### Fonctionnalités requises
   - **Réalisation d'une partie en mode textuel** : Permettre le lancement d'une partie en mode textuel dans un terminal et permettre à l'utilisateur d'y jouer.
   - **Réalisation d'une partie en mode graphique** : Permettre le lancement d'une partie en mode graphique et permettre à l'utilisateur d'y jouer.
   - **Réalisation d'un joueur virtuel** : Permettre le lancement d'une partie avec un joueur virtuel qui cherche à résoudre au mieux le jeu. L'affichage sera réalisé en mode textuel et graphique (choix laissé à l'utilisateur).


#### Technologies
   - **C** : Utilisation exclusive du langage C pour la programmation du jeu.
   - **libSDL** : Utilisation de la librairie C libSDL pour la réalisation de la version graphique de l'interface du jeu.


## Étapes du projet

#### Étude préalable
   - Réalisez une étude sur le fonctionnement du jeu Crossy Road, tout particulièrement sur la gestion des véhicules et sur les déplacements possibles.
   - Réalisez un **état de l’art** des algorithmes d'intelligence artificielle utilisés dans les jeux de stratégie (algorithmes Minimax, Alpha-Beta Pruning, Algorithmes d'apprentissage automatique, etc.) afin de déterminer le plus adapté pour implémenter un joueur virtuel compétent.


#### Conception de l'application
   - Définissez l'ensemble des structures de données nécessaires à la réalisation du jeu dans sa forme la plus élémentaire 
   - Étendez votre modèle afin qu'il puisse interagir avec la librairie libSDL.
   - Pensez dès la conception de votre application à une modalité d'interaction avec une intelligence artificielle remplaçant le joueur humain.

#### Développement de l’application
   - **Core** : Implémentation de la logique du jeu, des règles de Crossy Road, et des interactions avec le joueur.
   - **IA** : Implémenter un ou plusieurs algorithmes pour le joueur virtuel prenant en compte le niveau de difficulté et/ou les temps de réflexion/calcul.
   - **UI** : Conception de l’interface utilisateur pour visualiser le plateau de jeu, l'évolution des véhicules et les déplacements du joueur. Celle-ci comprendra deux modes : un mode textuel et un mode graphique. Les deux interfaces utilisateurs feront usage du même code du moteur de jeu (**Core**).
   
#### Documentation et tests
   - Documentation complète du code et des choix d'implémentation (structures de données, algorithmes mis en œuvre, logique de jeu).
   - Tests unitaires et fonctionnels pour s'assurer que l'application respecte les règles du jeu et gère correctement les scénarios (mise en pause, contact joueur véhicule, ...).


##  Exigences du projet et points évalués

Pour l'évaluation, les points suivants seront pris en considération :

- Qualité de la modélisation et des **structures de données** utilisées pour représenter les éléments du jeu.
- Implémentation correcte de la **logique du jeu** et respect des règles de Crossy Road.
- Fonctionnalité de l'**intelligence artificielle** (qualité de l'algorithme choisi et performances).
- **Qualité de l'interface utilisateur** et fluidité des interactions avec le jeu (à la fois sur la version textuelle que sur la version graphique)
- Respect des **bonnes pratiques de programmation** (structure du code, compilation séparée, commentaires, organisation, makefile, ...).
- Tests et gestion des erreurs (robustesse de l’application).


## Rendu Final

Le rendu final sera constitué a minima des éléments suivants :

- **Code source** : Livraison du code source complet et des instructions pour exécuter celui-ci. 
- **État de l'art** : Rapport de l’état de l’art sur les algorithmes d’intelligence artificielle applicables et appliqués.
- **Documentation** : Comprend une description du jeu, des détails techniques et tous les éléments de gestion de projet que vous aurez produits (fiche de projet, comptes-rendus de réunion, planification et répartition des tâches, analyse post-mortem des efforts individuels et de l'atteinte des objectifs, etc.).
- **Tests unitaires** : Un ensemble de tests unitaires accompagnant le code source. 

Tous ces éléments seront fournis sur le dépôt GitLab du projet.


## Date de rendu et Soutenance

Le projet est à rendre pour le **26 mai 2025** à 22 heures au plus tard.

Des soutenances de groupes de projet seront organisées dans la foulée (début juin).

Votre projet fera l'objet d'une démonstration devant un jury composé d'au moins 2 membres de l’équipe pédagogique. Durant cette soutenance, vous serez jugés sur votre démonstration de l'application et votre capacité à expliquer votre projet et son fonctionnement. Chaque membre du groupe devra être présent lors de soutenance et **participer activement**.

*Toute personne ne se présentant pas à la soutenance sera considérée comme démissionnaire de l'UE et en conséquence, ne pourra pas la valider pour l’année universitaire 2024-2025.*

Il est attendu que chaque membre du groupe ait contribué à plusieurs parties fonctionnelles du code (il ne s'agit pas d'avoir uniquement corrigé quelques lignes par ci et par là).


## Développement incrémental

Il est vivement recommandé à ce que le groupe adopte une stratégie de développement incrémentale.

L'idée est donc de planifier et de définir des "incréments" ou de petites unités fonctionnelles du jeu (ou de ces composants). Cela permet de se concentrer sur une petite section du jeu à la fois et d'être toujours capable d'avoir une version fonctionnelle du jeu complet. Cela permet également d'éviter l'effet tunnel : de commencer le développement de beaucoup de fonctionnalités et de n'avoir finalement rien ou pas grand-chose de fonctionnel à montrer à la fin du projet.


## Fraude, tricherie et plagiat

Ne trichez pas ! Ne copiez pas ! Ne plagiez pas ! Si vous le faites, vous serez lourdement sanctionnés. Nous ne ferons pas de distinction entre copieur et copié. Vous n'avez pas de (bonnes) raisons de copier. De même, vous ne devez pas utiliser de solution clé en main trouvée sur internet.

Par tricher, nous entendons notamment :
- Rendre le travail d’un collègue en y apposant votre nom ;
- Obtenir un code, une solution par un moteur de recherche (ou une IA) et la rendre sous votre nom ;
- Récupérer du code et ne changer que les noms de variables et fonctions ou leur ordre avant de les présenter sous votre nom 
- Autoriser consciemment ou inconsciemment un collègue à s'approprier votre travail personnel. Assurez-vous notamment que votre projet et ses différentes copies locales ne soient lisibles que par vous et les membres de votre groupe.

Nous encourageons les séances de *brainstorming* et de discussion entre les élèves sur le projet. C’est une démarche naturelle et saine comme vous la rencontrerez dans votre vie professionnelle. Si les réflexions communes sont fortement recommandées, vous ne pouvez rendre que du code et des documents écrits par vous-même. Vous indiquerez notamment dans votre rapport toutes vos sources (comme les sites internet que vous auriez consultés), en indiquant brièvement ce que vous en avez retenu.
Il est quasi certain que nous détections les tricheries. En effet, les rapports et les codes sont systématiquement soumis à des outils de détection de plagiat et de copie. Il existe notamment des outils de détection de manipulation de code extraordinaire mis à disposition par l’Université de Stanford, tels que `MOSS` (https://theory.stanford.edu/~aiken/moss/) ou `compare50` (https://cs50.readthedocs.io/projects/compare50/). De plus, chacun a son propre style de programmation et personne ne développe la même chose de la même manière.

Puisqu'il s'agit d'un projet réalisé dans le cadre de cours avancés de programmation, nous nous attendons à ce que vous soyez capable d'apprendre à débogguer des programmes par vous-même. Par exemple, demander à un autre élève de regarder directement votre code et de donner des suggestions d'amélioration commence à devenir un peu délicat au niveau éthique.

Si vous rencontrez des difficultés pour terminer une tâche, veuillez contacter l'un de vos enseignants afin que nous puissions vous aider. Nous préférons de loin passer du temps à vous aider plutôt que de traiter des cas de fraudes.

