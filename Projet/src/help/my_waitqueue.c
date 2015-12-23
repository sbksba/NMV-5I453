#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/delay.h>
#include <linux/workqueue.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ITtraining.com.tw");
MODULE_DESCRIPTION("A Simple Blocking IO device RaspPi");

/* declare a wait queue*/
static wait_queue_head_t my_wait_queue;
static bool cond;

/* declare a work queue*/
struct work_struct workq;
void my_workqueue_handler(struct work_struct *work)
{
	printk("WORK QUEUE: I'm just a timer to wake up the sleeping moudlue. \n");
	msleep(10000);  /* sleep */
	printk("WORK QUEUE: time up MODULE !! wake up !!!! \n");
	/* wake_up_interruptible(&my_wait_queue); */
	wake_up(&my_wait_queue);
}

/*
 * INIT_MODULE -- MODULE START --
 * */
static int __init my_wait_queue_init(void)
{
	printk("Wait queue example ....\n");

	// -- initialize the work queue
	INIT_WORK(&workq, my_workqueue_handler);
	schedule_work(&workq);

	// -- initialize the WAIT QUEUE head
	/* init_waitqueue_head(&my_wait_queue); */
	DECLARE_WAIT_QUEUE_HEAD(&my_wait_queue);

	printk("MODULE: This moudle is goint to sleep....\n");
	/* interruptible_sleep_on(&my_wait_queue); */
	wait_event(my_wait_queue, cond);

	printk("MODULE: Wakeup Wakeup I am Waked up........\n");
	return 0;
}

/*
 * CLEANUP_MODULE -- MODULE END --
 * */
static void __exit my_wait_queue_exit(void)
{
	printk("<1> Start to cleanup \n");
}

module_init(my_wait_queue_init);
module_exit(my_wait_queue_exit);
