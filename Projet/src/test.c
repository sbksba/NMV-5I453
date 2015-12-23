#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "keyser.h"

int main(int argc, char **argv)
{
	int file = open("/dev/KeyserSoze", O_RDONLY);
	char soze[81];
	keyser_data_t kd;

	/* while (1) { */
	/* 	printf("ASK what you want (kill|quit) : "); */
	/* 	scanf("%s", query); */

	/* 	if (strncmp(query, "kill", 4) == 0) { */
	/* 		kd.pid = atoi(argv[2]); */
	/* 		kd.sig = atoi(argv[1]); */
	/* 		ioctl(file, KEYSERKILL, kd); */
	/* 	} else if (strncmp(query, "quit", 4) == 0) { */
	/* 		break; */
	/* 	} */
	/* } */
	
	kd.pid = atoi(argv[2]);
       	kd.sig = atoi(argv[1]);
	printf("[USER] sig %d pid %d\n", kd.sig, kd.pid);
	ioctl(file, KEYSERKILL, &kd);

	printf("[EASTER]\n");
	ioctl(file, SOZE, &soze);
	printf("%s\n", soze);
	
	return EXIT_SUCCESS;
}
