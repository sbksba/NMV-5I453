#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_DESCRIPTION("Module \"hello word\" pour noyau linux");
MODULE_AUTHOR("Julien Sopena, LIP6");
MODULE_LICENSE("GPL");

static int hello_init(void)
{
	pr_info("Hello, world\n");
	return 0;
}
module_init(hello_init);

static void hello_exit(void)
{
	pr_info("Goodbye, cruel world\n");
}
module_exit(hello_exit);

