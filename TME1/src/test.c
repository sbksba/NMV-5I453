#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "version.h"


struct commit_bis
{
  unsigned long id;
  struct version version;
  char *comment;
};


struct commit_bis *commitOf (struct version *version)
{
  struct commit_bis tmp ;
  unsigned long offset ;

  offset = (unsigned long) &tmp.version - (unsigned long) &tmp ;

  return (struct commit_bis*) ((unsigned long) version - offset) ;
}


int main (int argc, char** argv) {
  struct commit_bis c, *c_bis ;
  c.id = 1 ;
  c.version.major = 22 ;
  c.version.minor = 07 ;
  c.version.flags = 0 ;
  strcpy (c.comment, "commentaire") ;

  printf (" id : %p \n version : %p \n comment : %p \n",
	  &c.id, &c.version, &c.comment) ;

  c_bis = commitOf (&c.version) ;

  printf ("%p == %p ?\n", &c, c_bis) ;



  return 0 ;
}
