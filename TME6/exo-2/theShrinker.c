#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/stat.h>
#include <linux/pid.h>
#include <linux/slab.h>
#include <linux/kthread.h>
#include <linux/string.h>
#include <linux/mutex.h>
#include <linux/list.h>
#include <linux/delay.h>
#include <linux/shrinker.h>

MODULE_DESCRIPTION("Module affichant périodiquement les stats CPU d'un proc dans syslog");
MODULE_AUTHOR("Houssem Kanzari & benjamin bielle");
MODULE_LICENSE("GPL");

#define BUFSIZE 512

int target;
module_param(target, int, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(target, "PID du processus à monitorer");

struct task_sample {
	struct list_head listsample;
	cputime_t utime;
	cputime_t stime;
};

struct task_monitor {
	struct pid *id;
	struct list_head listmonitor;
	int nbsample;
	struct mutex lock;
};

static struct task_monitor *tm;

static ssize_t taskmonitor_show(struct kobject *kobj,
				struct kobj_attribute *attr, char *buf);

static ssize_t taskmonitor_store(struct kobject *kobj,
				 struct kobj_attribute *attr,
				 const char *buf, size_t count);
static void skrinker_func(struct skrinker *shr, struct skrink_control *sc);

static struct kobj_attribute taskmonitor = __ATTR_RW(taskmonitor);
static struct task_struct *kmonitorfn_thread;
static struct shrinker sh = {
	.count_objects = tm->nbsample,
	.scan_objects = tm->skrinker_func,
	.seeks = DEFAULT_SEEKS,
	.batch = 0,
	.flags = 0
};

static void skrinker_func(struct skrinker *shr, struct skrink_control *sc)
{
	struct task_sample *ms, *tmp_s;
	int nbDel;

	mutex_lock(&tm->lock);
	list_for_each_entry_safe(ms, tmp_s, &tm->listmonitor, listsample) {
		kfree(ms);
		
	}
}

bool get_sample(struct task_monitor *tm, struct task_sample *sample)
{
	struct task_struct *ts;

	ts = get_pid_task(tm->id, PIDTYPE_PID);
	if (pid_alive(ts)) {
		sample->utime = ts->utime;
		sample->stime = ts->stime;
		put_task_struct(ts);
		return true;
	}
	put_task_struct(ts);
	return false;
}

void save_sample(void)
{
	struct task_sample *s = kmalloc(sizeof(struct task_sample), GFP_KERNEL);
	get_sample(tm, s);
	mutex_lock(&tm->lock);
	list_add(&s->listsample, &tm->listmonitor);
	tm->nbsample++;
	mutex_unlock(&tm->lock);
}

int monitor_fn(void *unusued)
{
	struct task_struct *ts;

	ts = get_pid_task(tm->id, PIDTYPE_PID);
	if (ts) {
		while (!kthread_should_stop()) {
			if (pid_alive(ts)) {
				save_sample();
				pr_warn("pid %d usr %d sys %d\n",
					pid_nr(tm->id), (int)ts->utime,
					(int)ts->stime);
			} else {
				pr_warn("Monitored pid %d is dead\n",
					pid_nr(tm->id));
				break;
			}
			ssleep(1);
		}
		put_task_struct(ts);
	}
	return 1;
}

struct task_monitor *monitor_pid(pid_t pid)
{
	struct task_monitor *monitor =
		(struct task_monitor *)kmalloc(sizeof(struct task_monitor),
					      GFP_KERNEL);
	monitor->id = find_get_pid(pid);
	INIT_LIST_HEAD(&monitor->listmonitor);
	mutex_init(&monitor->lock);
	if (!monitor->id) {
		pr_warn("Unknow pid\n");
		return NULL;
	}
	pr_warn("Pid found\n");
	return monitor;
}

ssize_t taskmonitor_show(struct kobject *kobj, struct kobj_attribute *attr,
			 char *buf)
{
	struct task_sample *sample;
	ssize_t count = 0;
	ssize_t count_tmp;
	char buf_tmp[BUFSIZE];
	
	list_for_each_entry_reverse(sample, &tm->listmonitor, listsample)
	{
		count_tmp = 0;
		count_tmp = scnprintf(buf_tmp, BUFSIZE, "pid %d usr %d sys %d\n",
				      pid_nr(tm->id), (int)sample->utime,
				     (int)sample->stime);
		if(count + count_tmp < PAGE_SIZE) {
			strcat(buf, buf_tmp);
			count += count_tmp;
		} else {
			break;
		}
	}
	return count;
}

ssize_t taskmonitor_store(struct kobject *kobj, struct kobj_attribute *attr,
			  const char *buf, size_t count)
{
	if (count < 4 && count > 5)
		return -1;

	if (strcmp(buf, "stop") == 0) {
		if (kmonitorfn_thread) {
			kthread_stop(kmonitorfn_thread);
			kmonitorfn_thread = NULL;
			pr_warn("thread stopped\n");
		}
		return 4;
	} else if (strcmp(buf, "start") == 0) {
		if (!kmonitorfn_thread) {
			kmonitorfn_thread = kthread_run(monitor_fn,
							NULL, "monitor_fn");
			pr_warn("thread started\n");
		}
		return 5;
	}
	return 1;
}

static int taskmonitor_init(void)
{
	sysfs_create_file(kernel_kobj, &(taskmonitor.attr));
	tm = monitor_pid(target);
	if (tm == NULL) {
		pr_warn("Unknow pid\n");
		sysfs_remove_file(kernel_kobj, &(taskmonitor.attr));
		return -1;
	}
	kmonitorfn_thread = kthread_run(monitor_fn, NULL, "monitor_fn");
	pr_warn("taskmonitor module loaded\n");
	return 0;
}

static void taskmonitor_exit(void)
{
	if (kmonitorfn_thread)
		kthread_stop(kmonitorfn_thread);
	put_pid(tm->id);
	kfree(tm);
	sysfs_remove_file(kernel_kobj, &(taskmonitor.attr));
	pr_warn("taskmonitor module unloaded\n");
}

module_init(taskmonitor_init);
module_exit(taskmonitor_exit);
