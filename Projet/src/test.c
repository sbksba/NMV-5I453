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

struct arguments
{
	char *args[2];      /* ARG1 and ARG2    */
        char *sig, *pid;    /* Arguments for -k */
};

static struct argp_option options[] = {
		{"kill", 'k', 0, 0, "Kill a proc"},
		{"lsmod", 'l', 0, 0, "Print the list of modules"},
		{"meminfo", 'm', 0, 0, "Print the information about the memory"},
		{"soze", 's', 0, 0, "Find yourself"},
		{0}
};

static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
	struct arguments *arguments = state->input;

	int file = open("/dev/KeyserSoze", O_RDONLY);
	char soze[81];
	keyser_data_t kd;
	struct sysinfo mysysinfo;

	switch (key)
	{
	case 'k':
		kd.sig = atoi(state->argv[state->next]);
		kd.pid = atoi(state->argv[(state->next)+1]);
		ioctl(file, KEYSERKILL, &kd);
		break;
	case 'l':
		/* printf("[LSMOD] not working correctly\n"); */
		ioctl(file, KEYSERLSMOD, NULL);
		break;
	case 'm':
		ioctl(file, KEYSERMEMINFO, &mysysinfo);
		printf("MemTotal : %8lu\n", mysysinfo.totalram);
		break;
	case 's':
		ioctl(file, SOZE, &soze);
		printf("%s\n", soze);
		break;
	case ARGP_KEY_ARG:
		if (state->arg_num > 2)
			argp_usage(state);
		arguments->args[state->arg_num] = arg;
		break;
	case ARGP_KEY_END:
		/* if (state->arg_num < 2) */
		/* 	argp_usage(state); */
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

static char args_doc[] = "SIGNAL PID";
const char *argp_program_version = "test 1.0";

/* Program Documentation */
static char doc[] =
	"Test -- a program for use the Keyser Module\
\vSponsored by Roger Verbal Kint\n";

static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char **argv)
{
	/* int file = open("/dev/KeyserSoze", O_RDONLY); */
	/* char soze[81]; */
	/* keyser_data_t kd; */
	/* struct sysinfo mysysinfo; */


       	/* kd.pid = atoi(argv[2]); */
       	/* kd.sig = atoi(argv[1]); */
	/* printf("[USER] sig %d pid %d\n", kd.sig, kd.pid); */
	/* ioctl(file, KEYSERKILL, &kd); */

	/* ERROR TO CORRECT */
	/* printf("[USER] lsmod\n"); */
	/* ioctl(file, KEYSERLSMOD, NULL); */

	/* printf("[USER] meminfo\n"); */
	/* ioctl(file, KEYSERMEMINFO, &mysysinfo); */
	/* printf("MemTotal : %8lu\n", mysysinfo.totalram); */

	/* printf("[EASTER]\n"); */
	/* ioctl(file, SOZE, &soze); */
	/* printf("%s\n", soze); */

	if (argc < 2) {
		printf("Usage: %s [OPTION...] SIGNAL PID\nTry `%s --help' or `%s --usage' for more information.\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}
	       	
	struct arguments arguments;

	/* Set argument defaults */
	arguments.sig = "";
	arguments.pid = "";
	
	/* Where the magic happens */
	argp_parse (&argp, argc, argv, 0, 0, &arguments);

	return EXIT_SUCCESS;
}
