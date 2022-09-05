# Bras à un degré de liberté (1DOF Drone Bench)
Ce dispositif a pour vocation d'Ãªtre utiliser Ã  des fins pedagogique. L'idée et de faire découvrir les principes et dispositifs que l'on peut retrouver sur un drone mais de façon largement simplifié et en toute sécurité.
Le prinicpe est d'asservir le bras en position horizontal à un angle voulue en pilotant le moteur et l'hélice fixé à l'extrimité du bras. La mesure de l'angle se fait par un gyroscope fixé sur le bras mobile.

# Utilisation
Il faut se connecter à la carte microcontroller via un cable USB situé sur l'un des montants. la carte (STM32 L476RG) est alimenté par USB.
Ouvrir ensuite un terminal série est établir la communication à 115200 bs/s.
La microcontroller se comporte comme un automate. En envoyant des commandes via la liaison série, il va entrer dans différent états, que l'on va appaler mode.
Par défaut, la carte sera en "Idle", taper 2 pour entrer dans le mode Info.


# Conception
## Mécanique
Une bonne partie des pièces ont été conçue avec Solidworks et fabriquées en impression 3D par dépot de filament.
Les modèles 3D sont accessibles [ici](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/tree/main/03_mdl)

Les montants sont en profilé d'aluminium de 20 mm dont voici la [référence](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/tree/main/01_doc/rs)


## Electronique
La 