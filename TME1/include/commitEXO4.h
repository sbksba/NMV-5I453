#ifndef COMMITEXO4_H
#define COMMITEXO4_H

#include "version.h"
#include "list.h"

struct commit;

struct commit{
  unsigned long id;
  struct version version;
  char *comment;
  struct list_head *lhead;
};

struct commit *new_commit(unsigned short major, unsigned long minor, char *comment);

struct commit *add_minor_commit(struct commit *from, char *comment);

struct commit *add_major_commit(struct commit *from, char *comment);

struct commit *del_commit(struct commit *victim);

void display_commit(struct commit *from);

void display_history(struct commit *from);

void infos(struct commit *from, int major, unsigned long minor);

#endif
