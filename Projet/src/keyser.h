#ifndef __KEYSER_H
#define __KEYSER_H

#define EASTER "The greatest trick the devil ever pulled was convincing the world he didn't exist"
#define MAGIC 'N'

typedef struct {
	struct work_struct ws_keyser;
	int pid;
	int sig;
}wqs_data_t;

#define KEYSERKILL _IOR(MAGIC, 0, wqs_data_t *)

#endif
