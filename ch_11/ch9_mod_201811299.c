#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");

#define BUF_SIZE 50

int count = 0;
char value[50];

static int proc_info_show(struct seq_file *seq, void *v) {
	seq_printf(seq, "str : %s\n\n", value);
	seq_printf(seq, "count : %d\n", count); 
	return 0;
}

static int proc_info_open(struct inode *inode, struct file *file) {
	return single_open(file, proc_info_show, NULL);
}

static ssize_t proc_info_write(struct file *file, const char __user *ubuf, size_t ubuf_len, loff_t *pos) {
	char buf[BUF_SIZE];

	if (ubuf_len > BUF_SIZE)
		return -1;
	if (copy_from_user(buf, ubuf, ubuf_len)) {
		return -1;
	}
	
	memset(value, 0, sizeof(value));
	sscanf(buf, "%50[^\n]s", value);

	count = 0;
	int i;
	for ( i=0; i<strlen(value); i++){
		if (value[i] == ' ') {
			count ++;
		}
	}

	printk("str: %s\n count : %d\n", value, count);

	return strlen(buf);
}

static const struct file_operations proc_info_fops = {
	.open = proc_info_open,
	.read = seq_read,
	.write = proc_info_write,
	.llseek = seq_lseek,
	.release = single_release,
};

static int __init simple_proc_init(void) {
	proc_create("ch9_proc", 0666, NULL, &proc_info_fops);
	return 0;
}

static void __exit simple_proc_exit(void) {
}

module_init(simple_proc_init);
module_exit(simple_proc_exit);


