#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/pid.h>
#include <linux/kthread.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/workqueue.h>

MODULE_DESCRIPTION("Module Kill Process");
MODULE_AUTHOR("Houssem Kanzari & benjamin bielle");
MODULE_LICENSE("GPL");

#define MAGIC 'N'
#define GET_SAMPLE _IOR(MAGIC, 0, char *)
#define TASKMON_STOP _IOR(MAGIC, 1, void *)
#define TASKMON_START _IOR(MAGIC, 2, void *)
#define TASKMON_SET_PID _IOW(MAGIC, 3, int)

static struct workqueue_struct *wKill;

my_work_t *wpKill;

static int Major;
static const char *name = "pkill";

/**
 * 
 * @param 
 */
static int myKill(pid_t pid, int sig)
{
	pr_info("[MYKILL] start\n");

	struct pid *p = find_get_pid(pid);

	if (p) {
		kill_pid(p, sig, NULL);
		return 0;
	}

	pr_info("[MYKILL] proc not found\n");
	return 1;
}

long device_cmd(struct file *filp, unsigned int cmd, unsigned long arg)
{
	char buf[1024];

	switch (cmd) {
	case KILL:
		pr_info("[KILL]\n");
		
		break;

	default:
		return -ENOTTY;
	}

	return 0;
}

const struct file_operations fops = {
	.unlocked_ioctl = device_cmd
};

static int __init pkill_init(void)
{
	pr_info("[INIT] taskmonitor\n");
	Major = register_chrdev(0, name, &fops);

	pr_info("%s The major device number is %d.\n",
		"Registeration is a success", Major);
	pr_info("mknod /dev/%s c %d 0\n", name, Major);

	wKill = create_workqueue("my_queue");
	if (!wkill) {
		
	}
		
	return 0;
}

static void __exit pkill_exit(void)
{
	unregister_chrdev(Major, name);
	pr_info("[EXIT] pkill\n");
}

module_init(pkill_init);
module_exit(pkill_exit);
