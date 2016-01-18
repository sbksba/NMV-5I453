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

/* For the log ^^ */
struct timeval now;
struct tm tm_val;

/* Initialize wait queue */
DECLARE_WAIT_QUEUE_HEAD(wait_queue);
static int kcond, lsmodcond, memcond, waitcond, waitAllcond;

/* Struct for the features */
struct killWork {
	keyser_data_t *kdt;
	struct work_struct work;
};

struct lsmodWork {
	char *buffer;
	struct work_struct work;
};

struct meminfoWork {
	keyser_mem_t *kmt;
	struct work_struct work;
};

struct waitWork {
        keyser_wait_t *kwt;
        struct delayed_work work;
};

struct waitAllWork {
        keyser_wait_t *kwt;
        struct delayed_work work;
};

struct killWork *killWorker;
struct lsmodWork *lsmodWorker;
struct meminfoWork *meminfoWorker;
struct waitWork *waitWorker;
struct waitAllWork *waitAllWorker;

/**
 * Schedule Fonction
 */
void schedule()
{
        if (!schedule_delayed_work(&waitWorker->work, 100))
		pr_err("[SCHEDULE] delayed work failed\n");
}

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
	struct lsmodWork *lw;
	struct module *mod;
	struct module_use *umod;
	char *tmp;
	char sourceName[BUFFER_SIZE];
	int memFree, cAppend;

	memFree = BUFFER_SIZE;
	cAppend = 0;
	lw = container_of(work, struct lsmodWork, work);
	tmp = kmalloc(512, GFP_KERNEL);

	list_for_each_entry(mod, &modules, list) {
		memFree -= scnprintf(tmp, STRING_SIZE,
				     "%-10s%20u%4u",
				     mod->name,
				     mod->core_size,
				     mod->init_size);

		if (mod->init_size > 0)
			list_for_each_entry(umod, &mod->source_list,
					    source_list) {
				memset(&sourceName[0], 0, BUFFER_SIZE);
				cAppend = scnprintf(sourceName, BUFFER_SIZE,
						    " %s", umod->target->name);
				strncat(tmp, sourceName, memFree - cAppend);
			}
		strncat(tmp, "\n", 1);
		strcat(lw->buffer, tmp);
	}
	kfree(tmp);
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
	mw->kmt->page = PAGE_SHIFT;
	si_meminfo(&mw->kmt->meminfo);

	if (!memcond) {
		memcond = 1;
		wake_up(&wait_queue);
	}
}

/**
 * Expect the end of a process, and only one process
 * @work : just a work_queue
 */
static void keyserWait(struct work_struct *work)
{
	int i;
        struct waitWork *ww;

        ww = container_of(work, struct waitWork, work);

        for (i=0; i<ww->kwt->cpt; i++) {
                if (pid_alive(ww->kwt->list_pid[i])) {
                        pr_info("[WAIT] %d is alive\n", ww->kwt->list_pid[i]);
		} else {
                        waitcond = 1;
                        wake_up(&wait_queue);
			break;
                }
        }

        if (!waitcond)
                schedule();

}

/**
 * Expect the end of all process
 * @work : just a work_queue
 */
static void keyserWaitAll(struct work_struct *work)
{
}

long device_cmd(struct file *filp, unsigned int cmd, unsigned long arg)
{
	char *date;

      	date = kmalloc(512, GFP_KERNEL);
	do_gettimeofday(&now);
	time_to_tm(now.tv_sec, 0, &tm_val);
	scnprintf(date, STRING_SIZE, "%d/%d/%ld %02d:%02d:%02d\n",
		  tm_val.tm_yday + 1, tm_val.tm_mon + 1,
		  1900 + tm_val.tm_year,
		  tm_val.tm_hour + 1, tm_val.tm_min, tm_val.tm_sec);

	switch (cmd) {
	case KEYSERKILL:
		pr_info("[KEYSERKILL] %s", date);

		kcond = 0;
		killWorker->kdt = kmalloc(sizeof(keyser_data_t), GFP_KERNEL);

		if (copy_from_user(killWorker->kdt, (keyser_data_t *)arg,
				   sizeof(keyser_data_t))) {
			pr_info("[KEYSERKILL] Error copy_from_user\n");
			return -EACCES;
		}

		schedule_work(&killWorker->work);
		wait_event(wait_queue, kcond);
		kfree(killWorker->kdt);

		break;

	case KEYSERLSMOD:
		pr_info("[KEYSERLSMOD] %s", date);

		lsmodcond = 0;
		lsmodWorker->buffer = kmalloc(STRING_SIZE, GFP_KERNEL);

		schedule_work(&lsmodWorker->work);
		flush_work(&lsmodWorker->work);
		/* wait_event(wait_queue, lsmodcond); */

		if (copy_to_user((char *)arg, lsmodWorker->buffer,
				 STRING_SIZE) > 0) {
			pr_info("[KEYSERLSMOD] Error copy_to_user\n");
			return -EACCES;
		}

		kfree(lsmodWorker->buffer);

		break;

	case KEYSERMEMINFO:
		pr_info("[KEYSERMEMINFO] %s", date);

		memcond = 0;
		meminfoWorker->kmt = kmalloc(sizeof(keyser_mem_t), GFP_KERNEL);
		schedule_work(&meminfoWorker->work);
		wait_event(wait_queue, memcond);

		if (copy_to_user((char *)arg, meminfoWorker->kmt,
				 sizeof(keyser_mem_t)) > 0) {
			pr_info("[KEYSERMEMINFO] Error copy_to_user\n");
			return -EFAULT;
		}

		kfree(meminfoWorker->kmt);

		break;

	case KEYSERWAIT:
		pr_info("[KEYSERWAIT] %s", date);

		waitcond = 0;
                waitWorker->kwt = kmalloc(sizeof(keyser_wait_t), GFP_KERNEL);

		if (copy_from_user(waitWorker->kwt, (keyser_wait_t *)arg,
                                   sizeof(keyser_data_t))) {
			pr_err("[KEYSERWAIT] Error copy_from_user\n");
                        return -EACCES;
		}

		schedule_work(&waitWorker->work);
                wait_event(wait_queue, waitcond);

		kfree(waitWorker->kwt);

		break;

	case KEYSERWAITALL:
		pr_info("[KEYSERWAITALL] %s", date);
		break;

	case SOZE:
		pr_info("[EASTER] %s", date);

		if (copy_to_user((char *)arg, EASTER, strlen(EASTER)+1) > 0) {
			pr_info("[SOZE] Error copy_to_user\n");
			return -EFAULT;
		}

		break;
	default:
		return -ENOTTY;
	}

	kfree(date);
	return 0;
}

const struct file_operations fops = {
	.unlocked_ioctl = device_cmd
};

static int __init keyser_init(void)
{
	pr_info("[KEYSER] Load Module\n");
	Major = register_chrdev(0, name, &fops);

	pr_info("%s The major device number is %d.\n",
		"Registeration is a success", Major);
	pr_info("mknod /dev/%s c %d 0\n", name, Major);

	killWorker    = kmalloc(sizeof(struct killWork), GFP_KERNEL);
	lsmodWorker   = kmalloc(sizeof(struct lsmodWork), GFP_KERNEL);
	meminfoWorker = kmalloc(sizeof(struct meminfoWork), GFP_KERNEL);
	waitWorker    = kmalloc(sizeof(struct waitWork), GFP_KERNEL);
	waitAllWorker = kmalloc(sizeof(struct waitAllWork), GFP_KERNEL);

	/* Initialize the workqueue */
	INIT_WORK(&killWorker->work, keyserKill);
	INIT_WORK(&lsmodWorker->work, keyserLsmod);
	INIT_WORK(&meminfoWorker->work, keyserMeminfo);
	INIT_DELAYED_WORK(&waitWorker->work, keyserWait);
	INIT_DELAYED_WORK(&waitAllWorker->work, keyserWaitAll);

	return 0;
}

static void __exit keyser_exit(void)
{
	/* Free the workqueue */
	kfree(killWorker);
	kfree(lsmodWorker);
	kfree(meminfoWorker);
	kfree(waitWorker);
        kfree(waitAllWorker);

	unregister_chrdev(Major, name);
	pr_info("[KEYSER] Unload Module\n");
}

module_init(keyser_init);
module_exit(keyser_exit);
