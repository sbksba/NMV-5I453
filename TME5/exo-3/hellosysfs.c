#include <linux/init.h>
#include <linux/module.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>

MODULE_DESCRIPTION("Module hellosysfs");
MODULE_AUTHOR("Houssem Kanzari & benjamin bielle");
MODULE_LICENSE("GPL");

/* cf Linux/Documentation/filesystems/sysfs.txt */
static ssize_t hello_show(struct kobject *kobj, struct kobj_attribute *attr,
			  char *buf);
static ssize_t hello_store(struct kobject *kobj, struct kobj_attribute *attr,
			   const char *buf, size_t count);

static int file;
struct kobj_attribute attHello = __ATTR_RW(hello);
static char nom[PAGE_SIZE] = "sysfs";

static int __init hellosysfs_init(void)
{
	file = sysfs_create_file(kernel_kobj, &attHello.attr);

	if (file < 0)
		return -1;
	return 0;
}

static void __exit hellosysfs_exit(void)
{
	sysfs_remove_file(kernel_kobj, &attHello.attr);
}

static ssize_t hello_show(struct kobject *kobj, struct kobj_attribute *attr,
			  char *buf)
{
	int i, j;
	char *f = "HELLO ";

	for (i = 0; i < 7; i++)
		buf[i] = f[i];
	for (j = 0; nom[j] != '\0'; j++)
		buf[i++] = nom[j];
	buf[i++] = '!';
	buf[i++] = '\n';

	return i;
}

static ssize_t hello_store(struct kobject *kobj, struct kobj_attribute *attr,
			   const char *buf, size_t count)
{
	int i;

	if (count > PAGE_SIZE)
		return count;

	for (i = 0; i < count; i++)
		nom[i] = buf[i];

	nom[i] = '\0';

	return count;
}

module_init(hellosysfs_init);
module_exit(hellosysfs_exit);
