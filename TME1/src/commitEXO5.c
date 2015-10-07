#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include"commitEXO5.h"
#include"version.h" 
#include"list.h"
#include "color.h"

static int nextId = 0;
LIST_HEAD(list_complete); /*RED LIST*/
LIST_HEAD(list_major);    /*GREEN LIST*/

/**
  * new_commit - alloue et initialise une structure commit correspondant au parametre
  * @major:      numero de version majeure
  * @minor:      numero de version mineure
  * @comment:    pointeur vers une chaine de caracteres contenant un commentaire
  */
struct commit *new_commit(unsigned short major, unsigned long minor, char *comment)
{
  struct commit *c = (struct commit*) malloc (sizeof (struct commit));
  c->id = nextId++;
  c->version.major = major;
  c->version.minor = minor;
  c->version.flags = 0;
  c->comment = strdup(comment); /*PETITE FUITE*/
  
  INIT_LIST_HEAD (&c->lhead);
  INIT_LIST_HEAD (&c->major_list);
  c->major_parent = c;

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
  list_add (&new->lhead, &from->lhead);

  /* Si les majeurs correspondent, ils ont le meme parent, sinon
   * le nouveau est son propre pere.
   */
  /*if (new->version.minor == 0){
    list_add(&new->major_list, &(from->major_parent->major_list));
    new->major_parent = new;
  }
  else{
    new->major_parent = from->major_parent;
    }*/

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
  struct commit* c = new_commit (from->version.major, from->version.minor + 1, comment);
  c->major_parent = from->major_parent;
  list_add(&c->lhead, &from->lhead);
  
  return c;/*insert_commit(from, c);*/
}

/**
  * add_major_commit - genere et insert un commit correspondant a une version majeure
  * @from:           commit qui deviendra le predecesseur du commit insere
  * @comment:        commentaire du commit
  */
struct commit *add_major_commit(struct commit *from, char *comment)
{
	/* TODO : Exercice 3 - Question 3 */
  struct commit* c = new_commit (from->version.major + 1, 0, comment);
  list_add(&c->lhead, &from->lhead);
  list_add (&c->major_list, &from->major_parent->major_list);
    
  return c;
}

/**
  * del_commit - extrait le commit de l'historique
  * @victim:         commit qui sera sorti de la liste doublement chainee
  */
struct commit *del_commit(struct commit *victim)
{
	/* TODO : Exercice 3 - Question 5 */
  list_del(&victim->lhead);
  free(victim);
  return NULL;
}

/**
  * display_commit - affiche un commit : "2:  0-2 (stable) 'Work 2'"
  * @c:             commit qui sera affiche
  */
void display_commit(struct commit *c)
{
  printf("%2lu: %2u-%lu %s \t'%s'\n",c->id,c->version.major,c->version.minor,
	 isUnstableBis(&c->version)? "(unstable)" : "(stable)", c->comment);
}

/**
  * display_history - affiche tout l'historique, i.e. l'ensemble des commits de la liste
  * @from:           premier commit de l'affichage
  */
void display_history(struct commit *from)
{
  struct list_head* tmp;
  list_for_each (tmp, &list_complete)
    display_commit(container_of (tmp, struct commit, lhead));
  printf("\n");
}

/**
  * infos - affiche le commit qui a pour numero de version <major>-<minor>
  * @major: major du commit affiche
  * @minor: minor du commit affiche
  */
void infos(struct commit *from, int major, unsigned long minor)
{
  struct commit *commitMajor, *commitMinor;
  struct list_head *pos;

  if (from->version.major == major && from->version.minor == minor){
    display_commit (from);
    return;
  }
  
  list_for_each(pos, &list_major){
    commitMajor = container_of (pos, struct commit, major_list);

    /* Cas ou le commit recherche est celui la */
    if (commitMajor->version.major == major && commitMajor->version.minor == minor){
      display_commit (from);
      return;
    }

    /* Cas ou le numero majeur est bon, on recherche le mineur */
    if (commitMajor->version.major == major){
      list_for_each (pos, &commitMajor->lhead){
	/* Si tete de liste */
	if (pos == &list_complete)
	  break;

	commitMinor = container_of (pos, struct commit, lhead);

	if (commitMinor->version.major != major)
	  break;
	
	if (commitMinor->version.minor == minor){
	  display_commit (commitMinor);
	  return;
	}
      }
      break;
    }
  }
  
  printf("%d-%lu : Not here !!!\n",major, minor);
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

/**
 * freeHistory - libere la memoire occupe par l'ensemble d'un liste de commit
 * @from: commit qui sera supprime
 *
 */
void freeHistory()
{
  struct list_head *pos, *tmp;
  struct commit *c;
  
  list_for_each_safe(pos, tmp, &list_complete){
    c = container_of(pos, struct commit, lhead);
    free(c->comment);
    free(c);
  }
}
