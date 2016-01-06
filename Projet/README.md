Projet : Invite de commande pour le noyau Linux
===============================================

Objectifs
---------

L'objectif de ce projet est de réaliser un outil en ligne de commmande capable d'intéragir avec le noyau pour obtenir certaines informations ou d'exécuter certaines actions.

> Cet outil peut être sous la forme d'un terminal intéractif (comme gdb)
> ou uniquement en ligne de commandes avec des sous-commandes (comme git).     
> Ici l'outil est de la même forme que git.

Les fonctionnalités du programme sont les suivantes :

* [kill]    - Permet de tuer un processus
* [lsmod]   - Permet d'afficher la liste des modules chargés
* [meminfo] - Permet d'afficher les informations concernant la mémoire
* [wait]    - Permet d'attendre la terminaison d'un processus dans une liste de processus
* [waitall] - Permet d'attendre la terminaison de tous les processus passés en paramètre

Utilisation
-----------

Usage: TEST [OPTION...] SIGNAL PID
Test -- a program for use the Keyser Module

  -k, --kill                 Kill a proc     
  -l, --lsmod                Print the list of modules     
  -m, --meminfo              Print the information about the memory     
  -s, --soze                 Find yourself     
  -?, --help                 Give this help list     
  --usage                    Give a short usage message     
  -V, --version              Print program version     


La commande kill est suivie du numéro du signal ainsi que du pid du processus à tuer.
exemple

./TEST -k 11 666

## Licence

(Licence MIT)

Copyright © 2015 Bielle Benjamin & Kanzari Houssem

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
