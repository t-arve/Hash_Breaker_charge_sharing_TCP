# Dehasher par Thomas Aranda Verstraete
Projet final de développement

## Prérequis :

- Vérifiez la présence des paquets nécessaires ; sinon, les commandes d'installation sous Debian sont les suivantes :

sudo apt-get update
sudo apt install build-essential
sudo apt install libssl-dev

- Deux machines virtuelles en réseau interne (assurez-vous d'installer les fichiers nécessaires au préalable).
Adresses IP recommandées : (les deux machines doivent pouvoir se pinguer).

- Serveur : 192.168.1.10
- Client : 192.168.1.11

## Instructions :

1. Placez chaque fichier (client et server) sur leurs machines respectives.

2. Compilation :

 - Serveur : `gcc -o main main.c`
 - Client : `gcc -o main main.c -lssl -lcrypto`

3. Démarrage (commencez par le serveur) :

 - Serveur : `./main 192.168.1.11 8080`
 - Client : `./main 192.168.1.10 8080`

4. Choisissez l'un des fichiers contenant des hachages inclus dans le répertoire /server :

 - `hashesdemo.txt` : Fichier de démonstration avec 10 combinaisons faciles à trouver.
 - `hashesfull.txt` : Fichier avec 1000 combinaisons de lettres majuscules, minuscules et chiffres avec 5 caractères.

## Avertissement :

Ce projet est réservé uniquement à des fins éducatives. Le créateur n'est pas responsable de l'utilisation abusive de ce code.
