# Bras à un degré de liberté (1DOF Drone Bench)
Dispositif pédagogique permettant de faire découvrir quelques principes et composants que l'on peut retrouver sur un drone mais de façon largement simplifiée et en toute sécurité.  
Le principe est d'asservir un bras mobile (qui par défaut est à la verticale) à un angle voulu en pilotant le moteur et l'hélice fixés à l'extremité de ce bras. La mesure de l'angle se fait par une centrale inertielle. Le tout est controlé par un microcontrôleur relié à un PC où une interface en Python trace différentes courbes (angle en fonction des gazs, angles en fonction du temps).


![This is an image](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/blob/main/01_doc/Photos/plan.png)

[Vidéo](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/blob/main/01_doc/Photos/video_20220902_181232.mp4)


# Utilisation
## Alimentation
La carte microcontrôleur est alimentée par USB. Il faut donc la connecter à un PC, ce qui permet egalement d'échanger des données.  
La partie moteur est alimentée par les câbles bananes. Régler l'alimentation sur 24V avec une limitation de courant à 5A.  
## Commandes
Ouvrir ensuite un [terminal série](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/tree/main/04_UART_Term/UART_Terminal_1.2.2) puis établir la communication à 115200 bits/s.  
Le microcontrôleur se comporte comme un automate. En envoyant des commandes via la liaison série, il va entrer dans différent états, dont certains seront propres aux différents modes de fonctionnement du dispositif.  
Le mode dans lequel se trouve l'automate s'affiche à l'écran.  
Par défaut, la carte est dans l'état "Entrance" : tapez sur la touche Entrée et suivez les indications à l'écran.  
Une fois l'état "Init UC" passé, la série de "bip" émis par le moteur doit s'arrêter. Le moteur est prêt.  
L'état suivant "Motor Ready" fait office de menu de sélection de mode.  
## Mode automatique
Le mode "Auto Mode" vous permettra de renseigner une consigne de position angulaire, ainsi que des coefficients PID. Il s'agit du seul mode asservis.  
## Modes manuels
Le mode "Manual Mode Pot" vous permettra de modifier manuellement les gazs du moteur via le potentiomètre.  
Le mode "Manual Mode Term" vous permettra de jouer sur une consigne en % de gazs du moteur.  
# Conception
## Mécanique
Une bonne partie des pièces ont été conçues avec Solidworks et fabriquées en impression 3D par dépot de filament.  
Les modèles 3D sont accessibles [ici](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/tree/main/03_mdl)  
Les montants sont en profilé d'aluminium de 20 mm dont voici la [référence](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/tree/main/01_doc/rs)  
## Électronique
- [Carte microcontrôleur STM32 L476RG](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/tree/main/01_doc/stm)
- [Moteur et ESC](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/tree/main/01_doc/moteur_esc)
- [Centrale inertielle](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/tree/main/01_doc/gyro%20doc)
# IHM
## Installation
- Bibliothèques à installer : tkinter / serial / pandas / matplotlib     
- Seul le fichier ["Master"](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/blob/a495668b303d1658d739c43df9a72356ac64ced2/06_IHM/Fichiers%20Python/GUI/Master.py) doit être exécuté, mais il est nécessaire d'avoir tous les fichiers du dossier GUI.  
## Utilisation
L'IHM est réalisée en Python via la bibliothèque Tkinter. Une fois le fichier "Master" exécuté, il vous sera alors demandé de sélectionner le port COM et le Baud rate (similaire à l'interaction avec le terminal UART).  
Ensuite, vous aurez alors le choix entre 4 modes : les modes "Manual Mode Term" et "Auto Mode" (accessibles également avec le terminal), ainsi que les modes "Calibration" et "Trip Mode" (séquence de vol).  
Le mode "Calibration" nécessite de sélectionner un [fichier .txt](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/blob/9a31ac3b8853a55df0e124fcb4ea317559d65e36/06_IHM/test%20calibration.txt) comportant les lignes d'instructions suivantes : valeur de % de gazs moteur (tabulation) temps d'attente passé sur cette mesure avant l'accès à la ligne suivante. Il est tout à fait possible de modifier le fichier .txt (seulement, veuillez noter qu'une valeur de gazs au-delà de 17% entraînera le bras dans une position supérieure à 90°, ce qui n'est pas recommandé).  
Le mode "Trip Mode" (ou séquence de vol) nécessite également la sélection d'un [fichier .txt](https://github.com/InnovationLab-EFREIParis/Stage_1DOF_DroneBench/blob/9a31ac3b8853a55df0e124fcb4ea317559d65e36/06_IHM/test%20trip.txt) comportant les lignes d'instructions suivantes : valeur consigne de position angulaire (tabulation) temps d'attente avant la prochaine consigne. Il est également possible de modifier les consignes, cependant veuillez noter qu'il n'est pas possible de renseigner des valeurs d'angles supérieures à 90°.  
