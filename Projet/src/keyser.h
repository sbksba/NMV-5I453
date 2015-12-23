#ifndef __KEYSER_H
#define __KEYSER_H

#define EASTER "The greatest trick the devil ever pulled was convincing the world he didn't exist"
#define MAGIC 'N'

typedef struct {
	struct work_struct my_work;
	int pid;
	int sig;
}keyser_data_t;

#define KEYSERKILL _IOR(MAGIC, 0, keyser_data_t *)

#endif
