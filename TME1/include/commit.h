#ifndef COMMIT_H
#define COMMIT_H

#include "version.h"
#include "list.h"

extern struct list_head list_complete;
extern struct list_head list_major;

struct commit;

struct commit_ops {
	void (*display)(struct commit *);
	void (*extract)(struct commit *);
};

struct commit {
	unsigned long id;
	struct version version;
	char *comment;
	struct list_head lhead;
	struct list_head major_list;
	struct commit *major_parent;
	struct commit_ops *ops;
};

struct commit *new_commit(unsigned short major,
			  unsigned long minor,
			  char *comment);

struct commit *add_minor_commit(struct commit *from, char *comment);

struct commit *add_major_commit(struct commit *from, char *comment);

struct commit *del_commit(struct commit *victim);

void display_commit_minor(struct commit *c);

void display_commit_major(struct commit *c);

void display_history(struct commit *from);

void infos(struct commit *from, int major, unsigned long minor);

void freeHistory(void);

void extract_minor(struct commit *c);

void extract_major(struct commit *c);

#endif
