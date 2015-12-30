#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/pid.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/workqueue.h>
#include <linux/wait.h>
#include <linux/time.h>
#include <linux/delay.h>
#include <linux/kobject.h>
#include <linux/mm.h>

#include "keyser.h"

MODULE_DESCRIPTION("Module ");
MODULE_AUTHOR("Houssem Kanzari & benjamin bielle");
MODULE_LICENSE("GPL");

/* Meminfo variable */
struct sysinfo *val;

/* Initialize work queue */
keyser_data_t kdt;
static struct work_struct kwork;

/* Initialize wait queue */
/* static wait_queue_head_t kwq; */
/* static bool kcond; */

/**
 * Kill a proc
 * @return 0 if it's work 1 if it doesn't
 */
static void keyserKill(struct work_struct *work)
{
       	struct pid *p;

	p = find_get_pid(kdt.pid);

	if (!p)
		goto err;
	
	kill_pid(p, kdt.sig, 1);
	/* wake_up(&kwq); */
	
	return;
err:
		pr_info("[KeyserKill] pid not found\n");
		return;
}

/**
 * Print the module list (lsmod)
 */
static void keyserLsmod(struct work_struct *work)
{
	struct module *mod;
	struct module_use *umod;

	pr_info("Module                  Size  modstruct     Used by\n");
	
	list_for_each_entry(mod, *(&THIS_MODULE->list.prev), list) {
		if (mod->state == MODULE_STATE_UNFORMED)
			continue;
		pr_info("%-20s%8u%8u ", mod->name, mod->core_size, mod->init_size);

		list_for_each_entry(umod, &mod->source_list, source_list) {
			pr_info("%s", umod->target->name);
		}
	}
	pr_info("\n");
}

/**
 * Print the information about the memory state
 */
static void keyserMeminfo(struct work_struct *work)
{
	struct timespec uptime;

	ktime_get_ts(&uptime);
	memset(val, 0, sizeof(*val));
	
	val->uptime   = uptime.tv_sec;
	val->loads[0] = avenrun[0];
	val->loads[1] = avenrun[1];
	val->loads[2] = avenrun[2];
	val->procs    = nr_threads-1;
	si_meminfo(val);
}

long device_cmd(struct file *filp, unsigned int cmd, unsigned long arg)
{
	switch (cmd) {
	case KEYSERKILL:
		pr_info("[KEYSERKILL]\n");
		if (copy_from_user(&kdt, (keyser_data_t *)arg, sizeof(keyser_data_t))) {
			pr_info("[KEYSERKILL] Error copy_from_user\n");
			return -EACCES;
		}
				
		INIT_WORK(&kwork, keyserKill);
		schedule_work(&kwork);
		
		/* init_waitqueue_head(&kwq); */
		/* wait_event(kwq, kcond); */
		pr_info("[NOTIFICATION] kill %d %d finish\n", kdt.sig, kdt.pid);
		break;
		
	case KEYSERLSMOD:
		pr_info("[KEYSERLSMOD]\n");
		
		INIT_WORK(&kwork, keyserLsmod);
		schedule_work(&kwork);

		break;

	case KEYSERMEMINFO:
		pr_info("ERMEMINFO]\n");
		
		INIT_WORK(&kwork, keyserMeminfo);
		schedule_work(&kwork);
		
		break;

	case SOZE:
		pr_info("[EASTER]\n");
		if (copy_to_user((char *)arg, EASTER, strlen(EASTER)+1) > 0) {
			pr_info("[SOZE] Error copy_to_user\n");
			return -EFAULT;
		}
		break;
	default:
		return -ENOTTY;
	}

	return 0;
}

const struct file_operations fops = {
	.unlocked_ioctl = device_cmd
};

static int __init keyser_init(void)
{
	pr_info("[KEYSER] INIT\n");
	Major = register_chrdev(0, name, &fops);

	pr_info("%s The major device number is %d.\n",
		"Registeration is a success", Major);
	pr_info("mknod /dev/%s c %d 0\n", name, Major);
	
	return 0;
}

static void __exit keyser_exit(void)
{
	unregister_chrdev(Major, name);
	pr_info("[KEYSER] EXIT\n");
}

module_init(keyser_init);
module_exit(keyser_exit);
