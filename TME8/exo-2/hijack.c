#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/unistd.h>
#include <asm/cacheflush.h>
#include <asm/page.h>
#include <asm/current.h>
#include <linux/sched.h>
#include <linux/kallsyms.h>
#include <linux/syscalls.h>
#include <linux/delay.h>

MODULE_DESCRIPTION("Module Hijack SIG");
MODULE_AUTHOR("Houssem Kanzari & benjamin bielle");
MODULE_LICENSE("GPL");

void **syscall_table;

/* WORK BUT NO CLEAN
unsigned long **find_sys_call_table(void)
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
*/

unsigned long **find_sys_call_table(void)
{
	unsigned long ptr;
	unsigned long *p;

	for (ptr = (unsigned long)sys_close;
	     ptr < (unsigned long)&loops_per_jiffy;
	     ptr += sizeof(void *)) {

		p = (unsigned long *)ptr;

		if (p[__NR_close] == (unsigned long)sys_close) {
			pr_info("Found the sys_call_table!!!\n");
			return (unsigned long **)p;
		}
	}

	return NULL;
}


static int __init hijack_init(void)
{
	pr_info("[HIJACK_INIT]\n");
	syscall_table = (void **)find_sys_call_table();

	if (!syscall_table)
		pr_info("Cannot find the system call address !!!\n");
	else
		pr_info("SYS_CALL_TABLE address : 0x%lu\n", (unsigned long)syscall_table);
	
	return 0;
}

static void __exit hijack_exit(void)
{
	pr_info("[HIJACK_EXIT]\n");
}

module_init(hijack_init);
module_exit(hijack_exit);
