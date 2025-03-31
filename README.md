Simulation d'une File d'Attente avec Gestion d'Opérateurs :

Ce programme simule une file d'attente où des clients sont servis par des opérateurs. Il modélise l'arrivée des clients, leur gestion dans une file (via une liste doublement chaînée) et la prise en charge par des opérateurs en fonction de la durée des appels. Une horloge virtuelle permet de simuler le temps. Un menu interactif permet de configurer plusieurs paramètres : le nombre d'opérateurs, la loi exponentielle (paramètre lambda), les heures de début et de fin de travail, ainsi que la durée des appels, la durée de la journée et le nombre de jours simulés.

Après avoir lancée la simulation les données de cette dernière sont stockées dans test.txt qui contient un résumé quotidien des statistiques de la file d'attente et des clients.

Le programme utilise plusieurs type / structures de données : Temps, client, queue, attente, opérateurs

Le temps est le type qui permet de définir le pas de la simulation elle contient 3 entiers nommés heure, minute et seconde.

Le client est un type qui contient deux pointeurs formant la liste doublement chaînée en point vers un client nommé suivant et vers un autre nommé précédent. Ce type contient aussi le nom du client et des données permettant le calcul des statiques reliées au temps d'attente.

La queue correspond à notre liste doublement chaînée puisqu'elle contient le pointeur qui pointe sur la head de la file de client et celui qui pointe sur la tail de la file.
- TOUT EST DEFINI DU POINT DE VUE DE L OPERATEUR
- la head de la file correspont au prochain client qui passe à l'accueil
- la tail de la file correspont au dernier client arrivé

Le type attente contient lui toutes les données relatives aux calculs des statistiques.

Le type opérateur est construit avec trois informations relatives à son statut c'est-à-dire : la durée de l'appel actuel qui nous permet de savoir si l'opérateur est libre (duree_appel = 0) ou sinon combien de temps il lui reste avant qu'il soit à nouveau libre. On stocke aussi sur chaque opérateur le Temps du début de l'appel et celui de la fin.


IDEES :
faire en sorte que les clients peuvent sortir de la file si ils sont trop impatients
faire en sorte que certains opérateurs soient plus efficace que d'autre
