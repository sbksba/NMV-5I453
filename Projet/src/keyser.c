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

static int keyserKill(keyser_data_t *kdt);

/* IOCTL Declaration */
static int Major;
static const char *name = "KeyserSoze";

/* Initialize work queue */
static DECLARE_WORK(kwork, keyserKill);

/* Initialize wait queue */
static wait_queue_head_t kwq;
static bool kcond;

/**
 * Kill a proc
 * @param pid Target Pid
 * @param sig Signal wanted
 * @return 0 if it's work 1 if it doesn't
 */
static int keyserKill(keyser_data_t *kdt)
{
       	struct pid *p = find_get_pid(kdt->pid);

	if (!p)
		pr_info("[KeyserKill] pid not found\n");
	else
		kill_pid(p, kdt->sig, 1);

	wake_up(&kwq);
	return 0;
}

long device_cmd(struct file *filp, unsigned int cmd, keyser_data_t *arg)
{
	char buf[1024];

	switch (cmd) {
	case KEYSERKILL:
		pr_info("[KEYSERKILL]\n");
		
		schedule_work(&kwork);
		wait_event(kwq, kcond);
		pr_info("[NOTIFICATION] kill finish\n");
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

	DECLARE_WAIT_QUEUE_HEAD(kwq);
	
	return 0;
}

static void __exit keyser_exit(void)
{
	unregister_chrdev(Major, name);
	pr_info("[KEYSER] EXIT\n");
}

module_init(keyser_init);
module_exit(keyser_exit);
