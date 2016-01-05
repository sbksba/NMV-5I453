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

/* Initialize wait queue */
DECLARE_WAIT_QUEUE_HEAD(wait_queue);
static int kcond, lsmodcond, memcond;

/* Struct for the features */
struct killWork {
	keyser_data_t *kdt;
	struct work_struct work;
};

struct lsmodWork {
	struct work_struct work;
};

struct meminfoWork {
	struct sysinfo *meminfo;
	struct work_struct work;
};

struct killWork *killWorker;
struct lsmodWork *lsmodWorker;
struct meminfoWork *meminfoWorker;

/**
 * Kill a proc
 * @work : just a work_queue
 */
static void keyserKill(struct work_struct *work)
{
	struct killWork *kw;
       	struct pid *p;

	kw = container_of(work, struct killWork, work);
	p = find_get_pid(kw->kdt->pid);

	if (!p) {
		pr_info("[keyserKill] pid not found\n");
		goto err;
	}

	kill_pid(p, kw->kdt->sig, 1);
	put_pid(p);

err:
	if (!kcond) {
		kcond = 1;
		wake_up(&wait_queue);
	}
}

/**
 * Print the module list (lsmod)
 * @work : just a work_queue
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
	/* wake_up(&wait_queue); */
}

/**
 * Print the information about the memory state
 * @work : just a work_queue
 */
static void keyserMeminfo(struct work_struct *work)
{
	struct meminfoWork *mw;

	mw = container_of(work, struct meminfoWork, work);
	si_meminfo(mw->meminfo);

	if (!memcond) {
		memcond = 1;
		wake_up(&wait_queue);
	}
}

long device_cmd(struct file *filp, unsigned int cmd, unsigned long arg)
{
	switch (cmd) {
	case KEYSERKILL:
		pr_info("[KEYSERKILL]\n");
		kcond = 0;
		killWorker->kdt = kmalloc(sizeof(keyser_data_t), GFP_KERNEL);

		if (copy_from_user(killWorker->kdt, (keyser_data_t *)arg, sizeof(keyser_data_t))) {
			pr_info("[KEYSERKILL] Error copy_from_user\n");
			return -EACCES;
		}

		schedule_work(&killWorker->work);
		wait_event(wait_queue, kcond);
		kfree(killWorker->kdt);

		break;

	case KEYSERLSMOD:
		pr_info("[KEYSERLSMOD]\n");
		schedule_work(&lsmodWorker->work);
		/* wait_event(wait_queue, lsmodcond); */

		break;

	case KEYSERMEMINFO:
		pr_info("[KEYSERMEMINFO]\n");
		memcond = 0;
		meminfoWorker->meminfo = kmalloc(sizeof(struct sysinfo), GFP_KERNEL);
		schedule_work(&meminfoWorker->work);
		wait_event(wait_queue, memcond);

		if (copy_to_user((char *)arg, meminfoWorker->meminfo, sizeof(struct sysinfo)) > 0) {
			pr_info("[KEYSERMEMINFO] Error copy_to_user\n");
			return -EFAULT;
		}

		kfree(meminfoWorker->meminfo);

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

	killWorker    = kmalloc(sizeof(struct killWork), GFP_KERNEL);
	lsmodWorker   = kmalloc(sizeof(struct lsmodWork), GFP_KERNEL);
	meminfoWorker = kmalloc(sizeof(struct meminfoWork), GFP_KERNEL);

	/* Initialize the workqueue */
	INIT_WORK(&killWorker->work, keyserKill);
	INIT_WORK(&lsmodWorker->work, keyserLsmod);
	INIT_WORK(&meminfoWorker->work, keyserMeminfo);

	return 0;
}

static void __exit keyser_exit(void)
{
	/* Free the workqueue */
	kfree(killWorker);
	kfree(lsmodWorker);
	kfree(meminfoWorker);

	unregister_chrdev(Major, name);
	pr_info("[KEYSER] EXIT\n");
}

module_init(keyser_init);
module_exit(keyser_exit);
