# Bras � un degr� de libert� (1DOF Drone Bench)
Ce dispositif a pour vocation � �tre utilis� � des fins pedagogique. L'id�e et de faire d�couvrir les principes et dispositifs que l'on peut retrouver sur un drone mais de fa�on largement simplifi� et en toute s�curit�.
Le prinicpe est d'asservir un bras mobile (qui par d�faut est � la vertical) � un angle voulue en pilotant le moteur et l'h�lice fix� � l'extrimit� de ce bras. La mesure de l'angle se fait par un gyroscope.

![This is an image](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/blob/main/01_doc/img/plan.png)

[Vid�o](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/blob/main/01_doc/img/video_20220902_181232.mp4)


# Utilisation
Il faut se connecter � la carte microcontroller via un cable USB situ� sur l'un des montants. la carte (STM32 L476RG) est aliment� par USB.
La partie moteur est alimenter par les cables bannanes. R�gler l'alimentation dur 24V avec limitation de courant � 5A.
Ouvrir ensuite un terminal s�rie puis �tablir la communication � 115200 bd/s.
La microcontroller se comporte comme un automate. En envoyant des commandes via la liaison s�rie, il va entrer dans diff�rent �tats, que l'on va appaler mode.
Par d�faut, la carte sera en "Idle", taper 2 pour entrer dans le mode Info.
Taper ensuite 1 pour initialiser le moteur. A cette �tape, le s�rie de "bip" �mis par le moteur doit s'arreter. C'est signe qu'il est pr�t.
## Mode automatique
Taper 4 pour entrer dans le mode automatique. Les gaz sont calcul�s pour que le bras se stabilise � un angle donn�.
Taper 6 pour quitter et revenir au mode "Moteur pr�t"
## Mode manuel
Taper 3 pour commander les gaz avec le potentionm�tres
Taper 6 pour quitter et revenir au mode "Moteur pr�t"

# Conception
## M�canique
Une bonne partie des pi�ces ont �t� con�ue avec Solidworks et fabriqu�es en impression 3D par d�pot de filament.
Les mod�les 3D sont accessibles [ici](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/tree/main/03_mdl)

Les montants sont en profil� d'aluminium de 20 mm dont voici la [r�f�rence](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/tree/main/01_doc/rs)


## Electronique
La 

