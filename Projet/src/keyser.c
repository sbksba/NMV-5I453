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

#include "keyser.h"

MODULE_DESCRIPTION("Module ");
MODULE_AUTHOR("Houssem Kanzari & benjamin bielle");
MODULE_LICENSE("GPL");

static int keyserKill(struct work_struct *work);

/* Initialize work queue */
keyser_data_t kdt;
/* static struct work_struct kwork; */
/* static DECLARE_WORK(kwork, keyserKill); */

/* Initialize wait queue */
/* static wait_queue_head_t kwq; */
/* static bool kcond; */

/**
 * Kill a proc
 * @param pid Target Pid
 * @param sig Signal wanted
 * @return 0 if it's work 1 if it doesn't
 */
static int keyserKill(struct work_struct *work)
{
       	struct pid *p = find_get_pid(kdt.pid);

	if (!p)
		goto err;
	
	kill_pid(p, kdt.sig, 1);
	/* wake_up(&kwq); */
	
	return 0;
err:
		pr_info("[KeyserKill] pid not found\n");
		return 1;
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
				
		pr_info("[KDT] sig %d pid %d\n", kdt.sig, kdt.pid);
		struct pid *p = find_get_pid(kdt.pid);
		kill_pid(p, kdt.sig, 1);
		/* schedule_work(&kwork); */
		/* wait_event(kwq, kcond); */
		pr_info("[NOTIFICATION] kill %d %d finish\n", kdt.sig, kdt.pid);
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

	/* DECLARE_WAIT_QUEUE_HEAD(kwq); */
	
	return 0;
}

static void __exit keyser_exit(void)
{
	unregister_chrdev(Major, name);
	pr_info("[KEYSER] EXIT\n");
}

module_init(keyser_init);
module_exit(keyser_exit);
