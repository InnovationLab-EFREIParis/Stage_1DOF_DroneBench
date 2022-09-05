# Bras � un degr� de libert� (1DOF Drone Bench)
Ce dispositif a pour vocation d'être utiliser à des fins pedagogique. L'id�e et de faire d�couvrir les principes et dispositifs que l'on peut retrouver sur un drone mais de fa�on largement simplifi� et en toute s�curit�.
Le prinicpe est d'asservir le bras en position horizontal � un angle voulue en pilotant le moteur et l'h�lice fix� � l'extrimit� du bras. La mesure de l'angle se fait par un gyroscope fix� sur le bras mobile.

# Utilisation
Il faut se connecter � la carte microcontroller via un cable USB situ� sur l'un des montants. la carte (STM32 L476RG) est aliment� par USB.
Ouvrir ensuite un terminal s�rie est �tablir la communication � 115200 bs/s.
La microcontroller se comporte comme un automate. En envoyant des commandes via la liaison s�rie, il va entrer dans diff�rent �tats, que l'on va appaler mode.
Par d�faut, la carte sera en "Idle", taper 2 pour entrer dans le mode Info.


# Conception
## M�canique
Une bonne partie des pi�ces ont �t� con�ue avec Solidworks et fabriqu�es en impression 3D par d�pot de filament.
Les mod�les 3D sont accessibles [ici](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/tree/main/03_mdl)

Les montants sont en profil� d'aluminium de 20 mm dont voici la [r�f�rence](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/tree/main/01_doc/rs)


## Electronique
La 