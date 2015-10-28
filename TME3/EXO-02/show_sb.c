#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/fs.h>

MODULE_DESCRIPTION("Module de parcours des superblocks chargés mémoire pour noyau linux");
MODULE_AUTHOR("AssHole & Houssem, YOUR_ASS & TUNISIE");
MODULE_LICENSE("GPL");

static void printBlock(struct super_block *sb, void *unused)
{
	pr_info("uuid=%pUb type=%s\n", sb->s_uuid, sb->s_type->name);
}

static int __init showSb(void)
{
	pr_info("[SHOW_SB] insmod\n");

	iterate_supers(printBlock, NULL);
	
	return 0;
}

static void __exit unloadShow(void)
{
	pr_info("[SHOW_SB] rmmod\n");
}

module_init(showSb);
module_exit(unloadShow);
