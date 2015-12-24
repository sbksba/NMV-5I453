#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/pid.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#include "keyser.h"

MODULE_DESCRIPTION("Module ");
MODULE_AUTHOR("Houssem Kanzari & benjamin bielle");
MODULE_LICENSE("GPL");

static struct workqueue_struct *wqs_keyser;
wqs_data_t *wqs_data_keyser;

static int Major;
static const char *name = "keyserSoze";

/**
 * Kill a proc
 * @param pid Target Pid
 * @param sig Signal wanted
 * @return 0 if it's work 1 if it doesn't
 */
static int keyserKill(wqs_data_t *wqsd)
{
       	struct pid *p = find_get_pid(wqsd->pid);

	if (!p)
		pr_info("[KeyserKill] pid not found\n");
	else
		kill_pid(p, wqsd->sig, 1);

	kfree((void *)wqsd);
	return 0;
}

long device_cmd(struct file *filp, unsigned int cmd, wqs_data_t *arg)
{
	/* char buf[1024]; */
	wqs_data_t *tmp;

	copy_from_user(tmp, arg, sizeof(wqs_data_t));

	switch (cmd) {
	case KEYSERKILL:
		pr_info("[KILL]\n");
		wqs_data_keyser = (wqs_data_t *)kmalloc(sizeof(wqs_data_t), GFP_KERNEL);
		INIT_WORK((struct work_struct *)wqs_data_keyser, keyserKill);
		pr_info("PID : %d -- SIG : %d\n", (int)tmp->pid, (int)tmp->sig);
		wqs_data_keyser->pid = (int)tmp->pid;
		wqs_data_keyser->sig = (int)tmp->sig;
		if (queue_work(wqs_keyser, (struct work_struct *)wqs_data_keyser) == false) {
			pr_info("[KILL] queue_work not working\n");
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

static int __init keyserQueue_init(void)
{
	pr_info("[KEYSERQUEUE] INIT");
	Major = register_chrdev(0, name, &fops);

	pr_info("%s The major device number is %d.\n",
		"Registeration is a success", Major);
	pr_info("mknod /dev/%s c %d 0\n", name, Major);

	wqs_keyser = create_workqueue("keyser_queue");

	if (!wqs_keyser) {
		pr_info("[KEYSERQUEUE] create_worqueue not working\n");
		destroy_workqueue(wqs_keyser);
		return 1;
	}

	return 0;
}

static void __exit keyserQueue_exit(void)
{
	unregister_chrdev(Major, name);
	flush_workqueue(wqs_keyser);
	destroy_workqueue(wqs_keyser);

	pr_info("[KEYSERQUEUE] EXIT");
}

module_init(keyserQueue_init);
module_exit(keyserQueue_exit);
