# Bras � un degr� de libert� (1DOF Drone Bench)
Dispositif p�dagogique permettant de faire d�couvrir quelques principes et composants que l'on peut retrouver sur un drone mais de fa�on largement simplifi� et en toute s�curit�. Le prinicpe est d'asservir un bras mobile (qui par d�faut est � la verticale) � un angle voulue en pilotant le moteur et l'h�lice fix� � l'extremit� de ce bras. La mesure de l'angle se fait par un gyroscope. Le tout est control� par un microncontroller reli� � un PC.

![This is an image](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/blob/main/01_doc/img/plan.png)

[Vid�o](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/blob/main/01_doc/img/video_20220902_181232.mp4)


# Utilisation
## Alimentation
La carte microcontroller est aliment�e par USB. Il faut donc la connecter � un PC, ce qui permet egalement d'�changer des donn�es.
La partie moteur est aliment�e par les cables bannanes. R�gler l'alimentation sur 24V avec une limitation de courant � 5A.
## Commandes
Ouvrir ensuite un terminal s�rie puis �tablir la communication � 115200 bd/s.
Le microcontroller se comporte comme un automate. En envoyant des commandes via la liaison s�rie, il va entrer dans diff�rent �tats, que l'on va appeler mode.
Le mode dans lequel se trouve l'automate s'affiche � l'�cran.
Par d�faut, la carte sera en "Init", taper 2 pour entrer dans le mode "Info".
Taper ensuite 1 pour initialiser le moteur. A cette �tape, la s�rie de "bip" �mis par le moteur doit s'arreter. C'est signe qu'il est pr�t.
## Mode automatique
Taper 4 pour entrer dans le mode automatique. Les gaz sont calcul�s pour que le bras se stabilise � un angle donn�.
Taper 6 pour quitter et revenir au mode "Moteur pr�t"
## Mode manuel
Taper 3 pour commander les gaz avec le potentionm�tre.
Taper 6 pour quitter et revenir au mode "Moteur pr�t"

# Conception
## M�canique
Une bonne partie des pi�ces ont �t� con�ues avec Solidworks et fabriqu�es en impression 3D par d�pot de filament.
Les mod�les 3D sont accessibles [ici](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/tree/main/03_mdl)

Les montants sont en profil� d'aluminium de 20 mm dont voici la [r�f�rence](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/tree/main/01_doc/rs)


## Electronique
- [Carte microcontroller STM32 L476RG](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/tree/main/01_doc/stm)
- [Moteur et ESC](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/tree/main/01_doc/moteur_esc)
- [Gyroscope](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/tree/main/01_doc/gyro%20doc)
