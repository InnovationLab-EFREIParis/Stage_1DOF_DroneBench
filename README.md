# Bras à un degré de liberté (1DOF Drone Bench)
Dispositif pédagogique permettant de faire découvrir quelques principes et composants que l'on peut retrouver sur un drone mais de façon largement simplifié et en toute sécurité. Le prinicpe est d'asservir un bras mobile (qui par défaut est à la verticale) à un angle voulue en pilotant le moteur et l'hélice fixé à l'extremité de ce bras. La mesure de l'angle se fait par un gyroscope. Le tout est controlé par un microncontroller relié à un PC.

![This is an image](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/blob/main/01_doc/img/plan.png)

[Vidéo](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/blob/main/01_doc/img/video_20220902_181232.mp4)


# Utilisation
## Alimentation
La carte microcontroller est alimentée par USB. Il faut donc la connecter à un PC, ce qui permet egalement d'échanger des données.
La partie moteur est alimentée par les cables bannanes. Régler l'alimentation sur 24V avec une limitation de courant à 5A.
## Commandes
Ouvrir ensuite un terminal série puis établir la communication à 115200 bd/s.
Le microcontroller se comporte comme un automate. En envoyant des commandes via la liaison série, il va entrer dans différent états, que l'on va appeler mode.
Le mode dans lequel se trouve l'automate s'affiche à l'écran.
Par défaut, la carte sera en "Init", taper 2 pour entrer dans le mode "Info".
Taper ensuite 1 pour initialiser le moteur. A cette étape, la série de "bip" émis par le moteur doit s'arreter. C'est signe qu'il est prêt.
## Mode automatique
Taper 4 pour entrer dans le mode automatique. Les gaz sont calculés pour que le bras se stabilise à un angle donné.
Taper 6 pour quitter et revenir au mode "Moteur prêt"
## Mode manuel
Taper 3 pour commander les gaz avec le potentionmètre.
Taper 6 pour quitter et revenir au mode "Moteur prêt"

# Conception
## Mécanique
Une bonne partie des pièces ont été conçues avec Solidworks et fabriquées en impression 3D par dépot de filament.
Les modèles 3D sont accessibles [ici](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/tree/main/03_mdl)

Les montants sont en profilé d'aluminium de 20 mm dont voici la [référence](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/tree/main/01_doc/rs)


## Electronique
- [Carte microcontroller STM32 L476RG](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/tree/main/01_doc/stm)
- [Moteur et ESC](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/tree/main/01_doc/moteur_esc)
- [Gyroscope](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/tree/main/01_doc/gyro%20doc)
