#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <argp.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>

#include "keyser.h"

/* Number of proc to wait */
int NBProc;

/**
 * Print the information
 * @mem : just the information from the module keyser
 */
void printMeminfo(keyser_mem_t mem)
{
#define K(x) ((x) << (mem.page - 10))
	printf("MemTotal  :%8lu kB\n", K(mem.meminfo.totalram));
	printf("MemFree   :%8lu kB\n", K(mem.meminfo.freeram));
	printf("Buffers   :%8lu kB\n", K(mem.meminfo.bufferram));
	printf("HighTotal :%8lu kB\n", K(mem.meminfo.totalhigh));
	printf("HighFree  :%8lu kB\n", K(mem.meminfo.freehigh));
	printf("LowTotal  :%8lu kB\n", K(mem.meminfo.totalram - mem.meminfo.totalhigh));
	printf("LowFree   :%8lu kB\n", K(mem.meminfo.freeram - mem.meminfo.freehigh));
	printf("Shmem     :%8lu kB\n", K(mem.meminfo.sharedram));
	printf("SwapTotal :%8lu kB\n", K(mem.meminfo.totalswap));
	printf("SwapFree  :%8lu kB\n", K(mem.meminfo.freeswap));
	/*printf("MemUnit   :%8i kB\n",  mem.meminfo.mem_unit);*/
}

struct arguments {
	char *args[2];
	char *sig, *pid;
};

static struct argp_option options[] = {
		{"kill", 'k', 0, 0, "Kill a proc"},
		{"lsmod", 'l', 0, 0, "Print the list of modules"},
		{"meminfo", 'm', 0, 0,
		 "Print the information about the memory"},
		{"wait", 'w', 0, 0,
		 "Expect the end of a process, and only one"},
		{"waitall", 'a', 0, 0,
		 "Expect the end of all process"},
		{"soze", 's', 0, 0, "Find yourself"},
		{0}
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	struct arguments *arguments = state->input;

	int file = open("/dev/KeyserSoze", O_RDONLY);
	char soze[81];
	char lsmod[STRING_SIZE];
	keyser_data_t kd;
	keyser_mem_t km;
	keyser_wait_t kw;
	int i;

	/* Don't look or your eyes were bleeding */
	char m[7] = "Module";
	char s[5] = "Size";
	char u[8] = "Used by";
	/* ===================================== */

	switch (key) {
	case 'k':
		kd.sig = atoi(state->argv[state->next]);
		kd.pid = atoi(state->argv[(state->next)+1]);
		ioctl(file, KEYSERKILL, &kd);
		break;
	case 'l':
		/* printf("[LSMOD] not working correctly\n"); */
		ioctl(file, KEYSERLSMOD, &lsmod);
		printf("%-10s%20s %4s\n", m, s, u);
		printf("%s", lsmod);
		break;
	case 'm':
		ioctl(file, KEYSERMEMINFO, &km);
		printMeminfo(km);
		break;
	case 's':
		ioctl(file, SOZE, &soze);
		printf("%s\n", soze);
		break;
	case 'w':
		kw.cpt = NBProc;
                for (i=0; i<NBProc; i++) {
                        kw.list_pid[i] = atoi(state->argv[(state->next)+i]);
                }
                ioctl(file, KEYSERWAIT, &kw);
		break;
	case 'a':
		kw.cpt = NBProc;
                for (i=0; i<NBProc; i++) {
                        kw.list_pid[i] = atoi(state->argv[(state->next)+i]);
                }
                ioctl(file, KEYSERWAIT, &kw);
		ioctl(file, KEYSERWAITALL, NULL);
		break;
	case ARGP_KEY_ARG:
		if (state->arg_num > 2)
			argp_usage(state);
		arguments->args[state->arg_num] = arg;
		break;
	case ARGP_KEY_END:
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

static const char args_doc[] = "(SIGNAL PID) OR (LIST of PID)";
const char *argp_program_version = "test 1.0";

/* Program Documentation */
static char doc[] =
	"Test -- a program for use the Keyser Module\
\vSponsored by Roger Verbal Kint\n";

static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char **argv)
{
	char *name = argv[0];

	if (argc < 2) {
		printf("Usage: %s [OPTION...] (SIGNAL PID) OR (LIST of PID)\n \
Try '%s --help' or '%s --usage' for more information.\n", name, name, name);
		return EXIT_FAILURE;
	}

	struct arguments arguments;

	/* Set argument defaults */
	arguments.sig = "";
	arguments.pid = "";

	/* Where the magic happens */
	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	return EXIT_SUCCESS;
}
