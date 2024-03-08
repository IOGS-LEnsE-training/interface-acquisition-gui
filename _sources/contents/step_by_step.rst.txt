Main steps of a graphical interface design
==========================================

.. caution::
   This application and its documentation website are still works in progress

On se propose ici d'étudier la conception d'une application graphique permettant de récupérer une série de données acquises par l'intermédiaire d'une carte d'acquisition.

La carte d'acquisition (AB) sera pilotée à l'aide d'un protocole série standard.

Cahier des charges
------------------

Le système à réaliser devra permettre :
* d'**initialiser** la carte d'acquisition
* de lancer une **acquisition manuelle** point par point (à la demande de l'utilisateur) et d'afficher la valeur obtenue
* de lancer une **acquisition automatique** sur un ensemble de N points à une fréquence d'échantillonnage Fe (dans la gamme de fréquence compatible avec la carte d'acquisition), et pour cela :

	* d'envoyer un ordre de début d'acquisition (avec spécification des paramètres N et Fe)
	* d'afficher l'ensemble des points sur un graphique
	* de sauvegarder les paramètres et les valeurs dans un fichier CSV
	
La fréquence d'échantillonnage pourra être paramétrée entre 100~Hz et 10~kHz (valeurs limite atteignable sur une Nucléo L476RG).

Le nombre d'échantillons pourra être paramétré entre 100 et 10000.

Les échantillons seront codés sur 12 bits.	



Main steps of the design
------------------------

Les grandes étapes sont les suivantes :

* Comprendre les bases du protocole de bas niveau RS232
* Réaliser une première application embarquée, testée à l'aide d'un terminal série, permettant :
	* de transmettre un caractère simple
	* de transmettre une chaîne de caractères dont le nombre d'éléments est connu
	* de recevoir une commande simple (caractère)
* Réaliser une première application console permettant :
	* de recevoir un caractère simple
	* de recevoir une chaîne de caractères dont le nombre d'éléments est connu
	* de transmettre une commande simple (caractère)
* Faire une liste des commandes et des données à transmettre dans les deux directions


Commandes et données
--------------------

La partie la plus critique de cette application est la transmission fiable des N échantillons depuis le PC jusqu'à la carte d'acquisition. La transmission des informations étant parfaitement asynchrone entre les deux noeuds du réseau, il est intéressant d'empaqueter la donnée utile et d'ajouter du contenu permettant d'identifier l'échantillon.

Il peut par exemple être intéressant d'ajouter le numéro de l'échantillon transmis. En cas d'erreur de transmission d'un échantillon, il est alors possible de le supprimer de l'affichage final ou de redemander spécifiquement à la carte de renvoyer cette échantillon.

Il est également possible de demander à l'application sur PC d'acquitter chacune des données reçues. Le délai de transmission sera allongé mais les données acquises seront fiables.

Afin également d'harmoniser les trames de message et de savoir quand débute et se termine un échange de données, il est possible d'ajouter des éléments spécifiques permettant de détecter le début et la fin d'un message. Dans certains protocoles, les commandes et les données ont été conçues pour qu'aucune confusion ne soit possible (dans le cas du MIDI, les types de message sont des octets supérieurs à 128 alors que les données sont des octets inférieurs à 128). 

Enfin, il est possible d'ajouter des éléments de vérification de données de type checksum. Cette méthode sera proposée dans le dernier exemple.

Data quantification
~~~~~~~~~~~~~~~~~~~

The number of samples can be set up from 100 to 10000, meaning that 2 bytes are necessary to code this information.

Each sample is a 12 bits value requiring 2 bytes to be transmitted.

The number of samples to transmit from the PC to the AB is a value of between 100 and 10000, needing 2 bytes.

The sampling frequency to transmit from the PC to the AB is a value of between 100Hz and 10kHz, needing 2 bytes.


Frames
~~~~~~

Une première idée pour la conception d'une trame entre l'AB et le PC est la suivante :

i_msb i_lsb d_msb d_lsb

i_ms


Commands
~~~~~~~~

i = initialization, first connection
j = initialization but not first try
c = not connected

m = manual mode
a = automatic mode
d = data in automatic mode


* Initialiazing and testing communication
	* communication at 115200 bauds
	* sending **'i'** from PC to AB
	* answering **'i'** from AB to PC if connection is ok
	* answering **'i'** from AB to PC if connection is ok
* 


.. caution:: 
	Ne pas envoyer 2 octets successifs à la même valeur du PC vers AB


Improvements
------------

La version 4 est opérationnelle en mode console. Mais lorsqu'on augmente le nombre d'échantillons à acquérir et à transmettre (au-delà de 256 échantillons, lorsque les poids forts peuvent être égaux au poids faible de l'indice lors de la transmission d'une commande), l'application se bloque.

Il est alors nécessaire de retravailler un peu sur le protocole applicatif afin d'éviter que des données successives soient égales, sinon la carte d'acquisition suppose qu'il s'agit d'un début de trame.

