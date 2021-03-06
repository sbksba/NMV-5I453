#ifndef __KEYSER_H
#define __KEYSER_H

#define EASTER "The greatest trick the devil ever pulled was convincing the world he didn't exist"
#define MAGIC 'N'
#define BUFFER_SIZE 256
#define STRING_SIZE 4096

/* IOCTL Declaration */
static int Major;
static const char *name = "KeyserSoze";

typedef struct {
	int pid;
	int sig;
} keyser_data_t;

typedef struct {
	int page;
	struct sysinfo meminfo;
} keyser_mem_t;

typedef struct {
        int cpt;
        int list_pid[8];
} keyser_wait_t;

#define KEYSERKILL _IOR(MAGIC, 0, keyser_data_t *)
#define KEYSERLSMOD _IOR(MAGIC, 1, char *)
#define KEYSERMEMINFO _IOR(MAGIC, 2, keyser_mem_t *)
#define KEYSERWAIT _IOR(MAGIC, 3, void *)
#define KEYSERWAITALL _IOR(MAGIC, 4, void *)
#define SOZE _IOR(MAGIC, 5, void *)

#define IOC_MAXNR 6

#endif
