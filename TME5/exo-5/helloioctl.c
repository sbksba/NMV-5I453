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

static int Major;
static const char *name = "hello";
static char msg[1024] = "hello ioctl !";

long device_cmd(struct file *filp, unsigned int cmd, unsigned long arg)
{
	char buf[1024];

	switch (cmd) {
	case HELLO:
		copy_to_user((char *)arg, msg, strlen(msg));
		break;

	case WHO:
		copy_from_user(buf, (char *)arg, strlen(buf));
		strcpy(msg, "hello ");
		strcat(msg, buf);
		strcat(msg, " !");
		break;

	default:
		return -ENOTTY;
	}

	return 0;
}

const struct file_operations fops = {
	.unlocked_ioctl = device_cmd
};

static int __init helloioctl_init(void)
{
	Major = register_chrdev(0, name, &fops);

	if (Major < 0) {
		pr_info("[HELLOIOCTL] %s failed with %d\n",
			"Sorry, registering the character device ", Major);
		return Major;
	}

	pr_info("%s The major device number is %d.\n",
		"Registeration is a success", Major);
	pr_info("mknod /dev/%s c %d 0\n", "hello", Major);

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
