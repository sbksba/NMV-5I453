#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include"commit.h"
#include"version.h" 

static int nextId = 0;

/**
  * new_commit - alloue et initialise une structure commit correspondant au parametre
  * @major:      numero de version majeure
  * @minor:      numero de version mineure
  * @comment:    pointeur vers une chaine de caracteres contenant un commentaire
  */
struct commit *new_commit(unsigned short major, unsigned long minor, char *comment)
{
	/* TODO : Exercice 3 - Question 2 */
  struct commit *c = (struct commit*) malloc(sizeof(struct commit));
  c->version.major=major;
  c->version.minor=minor;
  c->comment=strdup(comment);
  c->next = NULL;
  c->prev = NULL;
  
  return c;
}

/**
  * insert_commit - insert sans le modifier un commit dans la liste doublement chainee
  * @from:       commit qui deviendra le predecesseur du commit insere
  * @new:        commit a inserer - seul ses champs next et prev seront modifie
  */
static struct commit *insert_commit(struct commit *from, struct commit *new)
{
  
	/* TODO : Exercice 3 - Question 3 */
  /*from->prev->prev=new;
    new->next=from->next;*/
  
  from->next=new;
  new->prev=from;
  
  return new;
}

/**
  * add_minor_commit - genere et insert un commit correspondant a une version mineure
  * @from:           commit qui deviendra le predecesseur du commit insere
  * @comment:        commentaire du commit
  */
struct commit *add_minor_commit(struct commit *from, char *comment)
{
	/* TODO : Exercice 3 - Question 3 */
  struct commit *c=new_commit(from->version.major, from->version.minor+1, comment);
  insert_commit(from,c);
  
  return c;
}

/**
  * add_major_commit - genere et insert un commit correspondant a une version majeure
  * @from:           commit qui deviendra le predecesseur du commit insere
  * @comment:        commentaire du commit
  */
struct commit *add_major_commit(struct commit *from, char *comment)
{
	/* TODO : Exercice 3 - Question 3 */
  struct commit *c=new_commit(from->version.major+1, 0, comment);
  insert_commit(from,c);
  
  return c;
}

/**
  * del_commit - extrait le commit de l'historique
  * @victim:         commit qui sera sorti de la liste doublement chainee
  */
struct commit *del_commit(struct commit *victim)
{
	/* TODO : Exercice 3 - Question 5 */
  return NULL;
}

/**
  * display_commit - affiche un commit : "2:  0-2 (stable) 'Work 2'"
  * @c:             commit qui sera affiche
  */
void display_commit(struct commit *c)
{
	/* TODO : Exercice 3 - Question 4 */
  display_version(&(c->version), isUnstableBis);
  printf ("%12s\n",c->comment) ;
}

/**
  * display_history - affiche tout l'historique, i.e. l'ensemble des commits de la liste
  * @from:           premier commit de l'affichage
  */
void display_history(struct commit *from)
{
	/* TODO : Exercice 3 - Question 4 */
  int i;
  printf("\n");
  for (from; from != NULL; from = from->next) {
    printf("%2d:",i);
    display_commit(from);
    i++;
  }
}

/**
  * infos - affiche le commit qui a pour numero de version <major>-<minor>
  * @major: major du commit affiche
  * @minor: minor du commit affiche
  */
void infos(struct commit *from, int major, unsigned long minor)
{
	/* TODO : Exercice 3 - Question 6 */
}

/**
  * commitOf - retourne le commit qui contient la version passe en parametre
  * @version:  pointeur vers la structure version dont cherche le commit
  * Note:      cette fonction continue de fonctionner meme si l'on modifie
  *            l'ordre et le nombre des champs de la structure commit.
  */
struct commit *commitOf(struct version *version)
{
	/* TODO : Exercice 2 - Question 2 */
  struct commit tmp;
  unsigned long offset;

  offset = (unsigned long) &tmp.version - (unsigned long) &tmp ;

  return (struct commit*) ((unsigned long) version - offset) ;
}




