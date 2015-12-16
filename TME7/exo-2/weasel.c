#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/dcache.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

MODULE_DESCRIPTION("Module affichant l'adresse et la taille de la dentry");
MODULE_AUTHOR("Houssem Kanzari & benjamin bielle");
MODULE_LICENSE("GPL");

const char *msg = "I'm a weasel\n";

/* QUESTION 3
static int weasel_read(struct file *file, char __user *user_buf,
		       size_t length, loff_t *offset)
{
	pr_info("[WEASEL_READ] (/proc/%s) called\n", "weasel");

       	return simple_read_from_buffer(user_buf, length, offset,msg,
				       sizeof(msg));
}

static const struct file_operations proc_file_ops = {
	.owner = THIS_MODULE,
	.open = simple_open,
	.llseek = noop_llseek,
	.read = weasel_read,
};
*/

static int weasel_show(struct seq_file *sf, void *v)
{
	struct hlist_node *pos;
	struct dentry *tpos;

	pr_info("[WEASEL_SHOW]\n");
	
	hlist_bl_for_each_entry(tpos, pos,
				(dentry_hashtable->first), d_hash) {
		seq_printf(sf, "%s\n", tpos->d_name.name);
	}
}

static int weasel_open(struct inode *inode, struct file *file)
{
	pr_info("[WEASEL_OPEN]\n");
	return single_open(file, weasel_show, NULL);
}

static const struct file_operations proc_file_ops = {
	.owner = THIS_MODULE,
	.open = weasel_open,
	.llseek = seq_lseek,
	.read = seq_read,
	.release = single_release,
};

static int __init weasel_init(void)
{
	int size;
	struct proc_dir_entry *proc_file_entry;

	pr_info("[WEASEL INIT]\n");

	size = 1U << d_hash_shift;
	pr_info("Adresse dentry_hashtable : %pk\n", dentry_hashtable);
	pr_info("Taille dentry_hashtable  : %d\n", size);
	proc_file_entry = proc_create("weasel", 0666, NULL, &proc_file_ops);

	if (proc_file_entry == NULL)
		return -ENOMEM;

	return 0;
}

static void __exit weasel_exit(void)
{
	pr_info("[WEASEL EXIT]\n");
	remove_proc_entry("weasel", NULL);
}

module_init(weasel_init);
module_exit(weasel_exit);
