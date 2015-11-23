#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/uaccess.h>

MODULE_DESCRIPTION("Module creant un nouveau driver");
MODULE_AUTHOR("Houssem Kanzari & benjamin bielle");
MODULE_LICENSE("GPL");

#define MAGIC 'N'
#define HELLO _IOR(MAGIC, 0, char *)
#define WHO _IOR(MAGIC, 1, char *)

struct file_operations fops;
static int Major;
static const char *name = "hello";
static char msg[1024] = "hello ioctl !";

static long device_cmd(struct file *filp, unsigned int cmd, unsigned long arg)
{
	char buff[512];
	
	switch(cmd) {
	case HELLO:
		copy_to_user((char *)arg, msg, strlen(msg));
		break;

	case WHO:
		copy_from_user(buff, (char *)arg, strlen(buff));
		strcpy(msg, "hello ");
		strcpy(msg, buff);
		strcpy(msg, " !");
		break;

	default:
		return -ENOTTY;
	}

	return 0;
}

static int __init helloioctl_init(void)
{
	fops.unlocked_ioctl = device_cmd;
	Major = register_chrdev(0, name, &fops);

	if (Major < 0) {
		pr_info("[HELLOIOCTL] %s failed with %d\n",
			"Sorry, registering the character device ", Major);
		return Major;
	}

	pr_info("%s The major device number is %d.\n",
		"Registeration is a success", Major);
	pr_info("mknod /dev/%s c %d 0\n","hello",Major);
	
	return 0;
}

static void __exit helloioctl_exit(void)
{
	unregister_chrdev(Major, name);
	pr_info("%s The major device number is %d.\n",
		"Unregisteration is a success", Major);
}

module_init(helloioctl_init);
module_exit(helloioctl_exit);
