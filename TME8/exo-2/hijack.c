#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/unistd.h>
#include <linux/syscalls.h>
#include <linux/delay.h>
#include <linux/tty.h>

MODULE_DESCRIPTION("Module Hijack SIG");
MODULE_AUTHOR("Houssem Kanzari & benjamin bielle");
MODULE_LICENSE("GPL");

/* void **syscall_table; */
void **syscall_table_V2;

unsigned long **find_sys_call_table_V2(void)
{
	unsigned long int offset = PAGE_OFFSET;
	unsigned long **sct;
	
	while (offset < ULLONG_MAX) {
		sct = (unsigned long **)offset;
		if (sct[__NR_close] == (unsigned long *)sys_close)
			return sct;
		offset += sizeof(void*);
	}

	return NULL;
}

/* unsigned long **find_sys_call_table(void) */
/* { */
/* 	unsigned long **sct; */
/* 	unsigned long ptr; */
/* 	unsigned long *p; */

/* 	for (ptr = (unsigned long)sys_close; */
/* 	     ptr < (unsigned long)&loops_per_jiffy; */
/* 	     ptr += sizeof(void *)) { */

/* 		p = (unsigned long *)ptr; */

/* 		if (p[__NR_close] == (unsigned long)sys_close) { */
/* 			pr_info("Found the sys_call_table!!!\n"); */
/* 			sct = (unsigned long **)p; */
/* 			return &sct[0]; */
/* 		} */
/* 	} */

/* 	return NULL; */
/* } */


static int __init hijack_init(void)
{
	pr_info("[HIJACK_INIT]\n");
	/* syscall_table = (void **)find_sys_call_table(); */
	syscall_table_V2 = (void **)find_sys_call_table_V2();

	if (!syscall_table_V2)
		pr_info("Cannot find the system call address !!!\n");
	else
		pr_info("SYS_CALL_TABLE address : v2(%p)\n", &syscall_table_V2);
	
	return 0;
}

static void __exit hijack_exit(void)
{
	pr_info("[HIJACK_EXIT]\n");
}

module_init(hijack_init);
module_exit(hijack_exit);
