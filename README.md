# rez

Rez est un projet en cours de développement dont l’objectif principal est :

- D’apprendre le langage C++ et de consolider mes connaissances en réseau.
- De simuler un réseau décentralisé où chaque nœud s’exécute dans un thread dédié.

Fonctionnement

- Chaque nœud du réseau communique avec les autres via des pipes.
- Le processus principal (main) a pour rôle :
  - De lancer les nœuds et de les connecter via les pipes.
  - Une fois le réseau initialisé, celui-ci fonctionne de manière autonome jusqu’à sa fermeture.
  - Le main intervient uniquement pour arrêter proprement le réseau si nécessaire.
