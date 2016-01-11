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

/**
 * Print the information
 * @mem : just the information from the module keyser
 */
void printMeminfo(struct sysinfo mem)
{
	printf("total ram           :%8lu\n", mem.totalram);
	printf("shared ram          :%8lu\n", mem.sharedram);
	printf("free ram            :%8lu\n", mem.freeram);
	printf("buffer ram          :%8lu\n", mem.bufferram);
	printf("total high          :%8lu\n", mem.totalhigh);
	printf("free (avail) high   :%8lu\n", mem.freehigh);
	printf("mem unit            :%8i\n",  mem.mem_unit);
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
	struct sysinfo mysysinfo;

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
		ioctl(file, KEYSERMEMINFO, &mysysinfo);
		printMeminfo(mysysinfo);
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
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

static const char args_doc[] = "SIGNAL PID";
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
		printf("Usage: %s [OPTION...] SIGNAL PID\n \
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
