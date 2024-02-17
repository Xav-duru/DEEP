**DEEP - Monopoly**

  Le projet DEEP (Digital Embedded Electronics Projects) est un projet obligatoire dans le cursus académique que j'ai réalisé en binôme.
C'est un projet en 1er année de cycle ingénieur sur les système embarqué.
Avec mon collègue, nous avons eu l'idée de refaire le jeu Monopoly à notre façon avec des éléments comme des LEDs ou un écran LCD.
<br>

**Langage utilisé :**
- C
<br>

**Outils utilsés :**
- STM32CubeIDE comme logiciel de développement pour les microcontrôleurs STM32
- Altium designer pour créer la carte électronique 
<br>

**Composants utilsés :**
- Carte STM32
- Ruban LED WS2812 
- Ecran LCD ILI9341 
- 3 boutons poussoires
- Plateau de jeu en carton avec design des cases
<br>

**But du jeu :**

  Le but du jeu est de posséder le plus de case possible afin de pouvoir augmenter le tarif lorsque les joueurs tomberont sur ces cases.  
<br>

**Tour de jeu :**

A chaque tour de jeu, le joueur doit lancer les dés. Pour ce faire, il appuiera sur un bouton, une simulation d’un chiffre aléatoire entre 1 et 6 s’affichera sur l’écran. Il n’y a donc aucun dé physique. Ensuite le joueur se déplacera alors sur le plateau grâce au bandeau de leds (1 led par case). Plusieurs cas se présentent à lui : 
- Le joueur tombe sur une case « SALLE », équivalent d'une case "Ville" du vrai Monopoly, il peut soit acheter la case, si celle-ci n’est pas déjà occupé, soit ne rien faire. Dans le cas où la case est déjà occupée, le joueur devra un certain montant au propriétaire de la case. Dans les deux cas, toutes les interactions se font grâce à l'écran LCD et aux 3 boutons. Le bouton de gauche pour se déplacer à gauche sur l'écran, celui de droite et le bouton "Valider" au milieu.
- Le joueur tombe sur une case « CHANCE », une carte lui sera affiché sur l’écran. Celle-ci peut être un déplacement (avancer / reculer / rendez-vous sur la case) ou bien, elle peut être liée à de l’argent (gagner / perdre). 
- Le joueur tombe sur une la case « CAFET’ », équivalent à la case « PRISON » du vrai monopoly, le joueur devra attendre 3 tours avant de pouvoir rejouer. Il peut également payer pour pouvoir rejouer plus rapidement. 
- A chaque fois que le joueur tombe sur la case « DEPART », comme dans les règles classiques, il reçoit un certain montant et s’il tombe pile sur la case, ce montant est doublé. 
<br>

**Fin du jeu :**

  Le jeu s’arrête lorsque qu’il ne reste plus qu’un seul joueur dans la partie, tous les autres ayant été éliminé car ils n’avaient plus d’argent. 
<br>

**Travail effectué :**

  Le travail était en binôme. Mon collègue s'est occupé de la partie hardware tandis que je me suis concentré sur le software.
<br>

**Résultat :**

Le jeu était fonctionnel, nous avons pu présenter le jeu du début à la fin. Il s'agisait évidemment d'une version simplifiée. 
