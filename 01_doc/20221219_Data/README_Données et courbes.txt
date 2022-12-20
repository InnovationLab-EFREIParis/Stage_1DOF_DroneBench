# Explications relatives au fichier "Données et courbes - comportements statique et dynamique"

## Comportement statique -> feuille "Statique"

Cette feuille comporte des données issues d'un "test calibration" consistant à récupérer les valeurs de positions angulaires,
calculées à partir des données brutes de la centrale inertielle du bras, pendant un certain temps t définis par l'utilisateur (ici 20s).
Les valeurs de consignes sont exprimées en % de gaz du moteur.
La simulation poursuit donc la routine suivante : l'utilisateur choisit une valeur de gaz (%) de départ (ici 1%), 
une valeur finale (dans ce cas-ci 17%), une valeur d'incrémentation (dans cet exemple, tous les 1%) et enfin une valeur de temps d'attente
entre la prise en compte de la valeur consigne et le début de l'enregistrement des valeurs de positions angulaires.

Ce temps d'attente permet donc de prendre uniquement en compte, les valeurs de positions, une fois que le bras s'est stabilisé
(et non pas durant le changement de valeur de gaz).
On va donc avoir par exemple, un envoi de consigne de valeur de gaz de 2%, un délai de 10s durant laquelle on s'assure que le bras
a eu le temps de se stabiliser, et on récupère ensuite les données de positions pendant 20s, avant de prendre en compte la valeur
consigne de gaz suivante (soit 3%).

La valeur de départ est de 1% et non 0%, car la valeur de '0' serait comprise par notre machine d'état comme une demande d'arrêt
des moteurs (le mode atterissage). 
La valeur finale est de 17% car au delà, le bras tend à basculer au dessus de 90° et se bloque.

En 20s d'acquisition, nous arrivons à récupérer autour de 64-65 valeurs de positions pour une valeur consigne (%).
Petite précision concernant la valeur de départ de 1%, le moteur ne s'allume qu'à partir d'une valeur consigne de 2%.
A 1% le moteur est donc encore éteint, mais nous faisons tout de même l'acquisition de valeurs de positions 
(qui sont pour certaines, hors normes, car le bras est considéré en position de départ autour de 0°).

La courbe orange correspond au tracé des valeurs moyennes des valeurs de positions pour chaque valeur consigne.

Commentaires : On constate sur différents essais dont cet exemple, un pic de valeurs de positions élevées pour une valeur consigne de 5%,
ainsi que 2 valeurs consignes (8 et 13 %) dont les valeurs de position ne suivent pas la croissance de la courbe.

## Comportement dynamique -> feuille "Dynamique"

Cette feuille comporte des données issues également d'un "test calibration", mais cette fois-ci nous mesurons la réponse à un échelon,
en prenant en compte, seulement 2 valeurs consignes : 1% (pour les mêmes motivations qu'en statique) puis 10%.
On récupère cette fois-ci les valeurs de position angulaire, sans temps d'attente, durant 15s pour chacune des valeurs.
Ainsi à t=15, on note la transition de la valeur 1% à 10%.

Commentaires : On observe, en cohérence avec le comportement statique, que la courbe oscille jusqu'à se stabiliser autour de la valeur de 30° 
pour 10% de gaz.



 
  