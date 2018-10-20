#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

MODULE_DESCRIPTION("Operating Systems - Chapter 11 Kernels");
MODULE_AUTHOR("Tom Hogenkamp");
MODULE_LICENSE("GPL");

static struct proc_dir_entry* my_proc_file;

/* Initialize counter */
static atomic_t my_counter = ATOMIC_INIT(0);

#define procfs_name "helloworld"

static int my_show(struct seq_file *m, void *v)
{
    /* Print my_counter atomically */
    seq_printf(m, "%s %d\n", "Hello World!", atomic_read(&my_counter));
    
    /* Increase my_counter by one */
    atomic_inc(&my_counter);
    return 0;
}

static int my_open(struct inode *inode, struct file *file)
{
    return single_open(file, my_show, NULL);
}

static const struct file_operations my_fops = {
    .owner	= THIS_MODULE,
    .open	= my_open,
    .read	= seq_read,
    .llseek	= seq_lseek,
    .release	= single_release,
};

static int __init my_init(void)
{
  my_proc_file = proc_create(procfs_name, 0, NULL, &my_fops);

  if (!my_proc_file) {
      return -ENOMEM;
  }

  return 0;
}

static void __exit my_exit(void)
{
        remove_proc_entry(procfs_name, NULL);
        printk(KERN_INFO "/proc/%s removed\n", procfs_name);
}

module_init(my_init);
module_exit(my_exit);
