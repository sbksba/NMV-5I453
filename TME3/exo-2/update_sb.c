#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/fs.h>
#include <linux/timekeeping.h>

MODULE_DESCRIPTION("Module de parcours des superblocks chargés mémoire pour noyau linux");
MODULE_AUTHOR("AssHole & Houssem, YOUR_ASS & TUNISIE");
MODULE_LICENSE("GPL");

static char *type;
module_param(type, charp, S_IRUGO);
MODULE_PARM_DESC(type, "Just the type of file we search !!");

static void printBlock(struct super_block *sb, void *unused)
{
	struct timespec now;
	
	getnstimeofday(&now);
	pr_info("uuid=%pUb type=%s time=%06lu\n", sb->s_uuid, sb->s_type->name,
		(unsigned long) now.tv_nsec / NSEC_PER_USEC);
}

static int __init updateSb(void)
{
	pr_info("[UPDATE_SB] insmod\n");
	
	struct file_system_type *myType;
		
	myType = get_fs_type(type);
	iterate_supers_type(myType, printBlock, NULL);
	/* put_filesystem(myType); */
	
	return 0;
}

static void __exit unloadShow(void)
{
	pr_info("[UPDATE_SB] rmmod\n");
	
}

module_init(updateSb);
module_exit(unloadShow);
