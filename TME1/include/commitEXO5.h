#ifndef COMMITEXO5_H
#define COMMITEXO5_H

#include "version.h"
#include "list.h"

struct commit;

struct commit{
  unsigned long id;
  struct version version;
  char *comment;
  struct list_head lhead;
  struct list_head major_list;
  struct commit *major_parent;

  void (*display) (struct commit*);
  void (*extract) (struct commit*);
};

struct commit *new_commit(unsigned short major, unsigned long minor, char *comment);

struct commit *add_minor_commit(struct commit *from, char *comment);

struct commit *add_major_commit(struct commit *from, char *comment);

struct commit *del_commit(struct commit *victim);

void display_commit(struct commit *from);

void display_commit_minor (struct commit *c);

void display_commit_major (struct commit *c);

void display_history(struct commit *from);

void infos(struct commit *from, int major, unsigned long minor);

void freeHistory();

void extract_minor(struct commit *c);

void extract_major(struct commit *c);
  
#endif
