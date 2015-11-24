#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/string.h>

#include <linux/utsname.h>
#include <linux/uts.h>

MODULE_DESCRIPTION("Module de changement de nom du systeme pour noyau linux");
MODULE_AUTHOR("ASSHOLE, YOUR_ASS");
MODULE_LICENSE("GPL");

static int change_name(void)
{
	strcpy(init_uts_ns.name.sysname,"AssHole-System");
	pr_info("[CHANGE]  NAME : %s\n", init_uts_ns.name.sysname);

	return 0;
}

static void restore_name(void)
{
	strcpy(init_uts_ns.name.sysname, UTS_SYSNAME);
	pr_info("[RESTORE] NAME : %s\n",init_uts_ns.name.sysname);
}

module_init(change_name);
module_exit(restore_name);
