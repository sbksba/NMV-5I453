#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <fcntl.h>
#include <string.h>

#include "keyser.h"

int main(int argc, char **argv)
{
	int file = open("/dev/KeyserSoze", O_RDONLY);
	char soze[81];
	keyser_data_t kd;
	struct sysinfo mysysinfo;

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

	return EXIT_SUCCESS;
}
