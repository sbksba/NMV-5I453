#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/workqueue.h>

MODULE_LICENSE("GPL");

static struct workqueue_struct *my_wq;

typedef struct {
	struct work_struct my_work;
	int    x;
} my_work_t;

my_work_t *work, *work2;

static void my_wq_function( struct work_struct *work)
{
	my_work_t *my_work = (my_work_t *)work;

	printk( "my_work.x %d\n", my_work->x );
	kfree( (void *)work );
	return;
}

static void my_test(my_work_t *mwt, int arg)
{
	int ret;

	mwt = (my_work_t *)kmalloc(sizeof(my_work_t), GFP_KERNEL);
	if (mwt) {
		INIT_WORK( (struct work_struct *)mwt, my_wq_function );
		mwt->x = arg;
		ret = queue_work( my_wq, (struct work_struct *)mwt );
	}
}

static int __init testQueue_init( void )
{
	int ret;

	pr_info("[INIT] create workqueue\n");
	my_wq = create_workqueue("my_queue");

	if (!my_wq) {
		destroy_workqueue(my_wq);
		return 1;
	}
	
	/* Queue some work (item 1) */
	pr_info("[INIT] my_test(work)\n");
	my_test(work, 1);
	/* Queue some additional work (item 2) */
	pr_info("[INIT] my_test(work2)\n");
	my_test(work2, 2);
	
	return 0;
}

static void __exit testQueue_exit( void )
{
	pr_info("[EXIT]\n");
	flush_workqueue( my_wq );
	destroy_workqueue( my_wq );
	return;
}

module_init(testQueue_init);
module_exit(testQueue_exit);
