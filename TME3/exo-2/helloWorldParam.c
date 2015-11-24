#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_DESCRIPTION("Module \"hello word param\" pour noyau linux");
MODULE_AUTHOR("HOUSSEM, TUNISIE");
MODULE_LICENSE("GPL");

static char *whom = "world";
static int howmany = 1;
module_param(whom, charp, S_IRUGO);
module_param(howmany, int, S_IRUGO);

MODULE_PARM_DESC(whom, "Just who you are !!");
MODULE_PARM_DESC(howmany, "How many you want to print it !!");

static int hello_init(void)
{
	int k;
	for (k=0; k<howmany; k++) {
		pr_info("(%d) Hello, %s\n", k, whom);
	}
	return 0;
}
module_init(hello_init);

static void hello_exit(void)
{
	pr_info("Goodbye, cruel world\n");
}
module_exit(hello_exit);
