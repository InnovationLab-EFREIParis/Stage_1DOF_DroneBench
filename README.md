# Bras à un degré de liberté (1DOF Drone Bench)
Ce dispositif a pour vocation à être utilisé à des fins pedagogique. L'idée et de faire découvrir les principes et dispositifs que l'on peut retrouver sur un drone mais de façon largement simplifié et en toute sécurité.
Le prinicpe est d'asservir un bras mobile (qui par défaut est à la vertical) à un angle voulue en pilotant le moteur et l'hélice fixé à l'extrimité de ce bras. La mesure de l'angle se fait par un gyroscope.

![This is an image](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/blob/main/01_doc/img/plan.png)

[Vidéo](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/blob/main/01_doc/img/video_20220902_181232.mp4)


# Utilisation
Il faut se connecter à la carte microcontroller via un cable USB situé sur l'un des montants. la carte (STM32 L476RG) est alimenté par USB.
La partie moteur est alimenter par les cables bannanes. Régler l'alimentation dur 24V avec limitation de courant à 5A.
Ouvrir ensuite un terminal série puis établir la communication à 115200 bd/s.
La microcontroller se comporte comme un automate. En envoyant des commandes via la liaison série, il va entrer dans différent états, que l'on va appaler mode.
Par défaut, la carte sera en "Idle", taper 2 pour entrer dans le mode Info.
Taper ensuite 1 pour initialiser le moteur. A cette étape, le série de "bip" émis par le moteur doit s'arreter. C'est signe qu'il est prêt.
## Mode automatique
Taper 4 pour entrer dans le mode automatique. Les gaz sont calculés pour que le bras se stabilise à un angle donné.
Taper 6 pour quitter et revenir au mode "Moteur prêt"
## Mode manuel
Taper 3 pour commander les gaz avec le potentionmètres
Taper 6 pour quitter et revenir au mode "Moteur prêt"

# Conception
## Mécanique
Une bonne partie des pièces ont été conçue avec Solidworks et fabriquées en impression 3D par dépot de filament.
Les modèles 3D sont accessibles [ici](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/tree/main/03_mdl)

Les montants sont en profilé d'aluminium de 20 mm dont voici la [référence](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/tree/main/01_doc/rs)


## Electronique
La 

