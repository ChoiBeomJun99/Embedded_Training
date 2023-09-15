#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/spinlock.h>
#include <linux/uaccess.h>
#include <asm/delay.h>

#include "ch3.h"
MODULE_LICENSE("GPL");

#define DEV_NAME "ch3_dev"

struct msg_st *kern_buf;
spinlock_t my_lock;

struct msg_list {
	struct list_head list;
	struct msg_st msg;
};

static struct msg_list msg_list_head;

void delay(int sec) {
	int i, j;
	for (j=0; j<sec; j++) {
		for (i=0; i<1000; i++) {
			udelay(1000);
		}
	}
}

static int ioctl_read(struct msg_st *buf) {
	int ret;
	
	delay(3);
	spin_lock(&my_lock);
	ret = copy_to_user(buf, kern_buf, sizeof(struct msg_st));
	memset(kern_buf, '\0', sizeof(struct msg_st));
        list_del_init(&msg_list_head.list);
	// vfree(&msg_list_head); 
        spin_unlock(&my_lock);
	
	return ret;
}

static int ioctl_write(struct msg_st *buf) {
	int ret;
	struct msg_list *tmp = 0;

	spin_lock(&my_lock);
	ret = copy_from_user(kern_buf, buf ,sizeof(struct msg_st));
	tmp = (struct msg_list*)vmalloc(sizeof(struct msg_list));
	tmp->msg = *buf;
	// printk("kern_buf str -> %s", kern_buf->str);
	list_add_tail(&tmp->list, &msg_list_head.list);
	spin_unlock(&my_lock);

	return ret;
}

static int simple_mod_open(struct inode *inode, struct file *file){
        printk("simple_mod: open\n");
        return 0;
}

static int simple_mod_release(struct inode *inode, struct file *file){
        printk("simple_mod: release\n");
        return 0;
}


static long simple_mod_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
	struct msg_st *user_buf;
	int ret;

	user_buf = (struct msg_st*)arg;
	//printk("CMD : %d\n", cmd);

	switch(cmd) {
		case CH3_IOCTL_READ:
			printk("empty state : %d", list_empty(&msg_list_head.list));
			if(list_empty(&msg_list_head.list)) {
				struct msg_st *temp = {
					0, "\0"
				};
				copy_to_user(user_buf, temp, sizeof(struct msg_st));
                                break;
                        } else {
                        ret = ioctl_read(user_buf);
                        printk("CH3_IOCTL_READ return: %d\n", ret);
			}
                        break;
		case CH3_IOCTL_WRITE:
			ret = ioctl_write(user_buf);
			printk("CH3_IOCTL_WRITE return: %d\n", ret);
			break;
		default:
			printk("Nothing");
			break;
	}

	return 0;
}

struct file_operations simple_mod_fops = {
        .open = simple_mod_open,
        .release = simple_mod_release,
        .unlocked_ioctl = simple_mod_ioctl,
};


static dev_t dev_num;
static struct cdev *cd_cdev;

static int __init simple_mod_init(void) {
	int ret;

	printk("challenge3: init module\n");
	alloc_chrdev_region(&dev_num, 0, 1, DEV_NAME);
	cd_cdev = cdev_alloc();
	cdev_init(cd_cdev, &simple_mod_fops);
	ret = cdev_add(cd_cdev, dev_num, 1);
	if (ret < 0) {
		printk("fail to add character device \n");
		return -1;
	} else {
		// printk("succed to add character device ret: %d\n", ret);
	}

	kern_buf = (struct msg_st*)vmalloc(sizeof(struct msg_st));
	memset(kern_buf, '\0', sizeof(struct msg_st));

	INIT_LIST_HEAD(&msg_list_head.list);

	return 0;
}

static void __exit simple_mod_exit(void) {
	printk("challenge3: exit module\n");
	cdev_del(cd_cdev);
	unregister_chrdev_region(dev_num, 1);
	vfree(kern_buf);
}

module_init(simple_mod_init);
module_exit(simple_mod_exit);


