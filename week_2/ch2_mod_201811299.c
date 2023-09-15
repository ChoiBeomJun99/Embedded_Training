#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");

#define DEV_NAME "ch2_dev"

#define IOCTL_START_NUM 0x80
#define IOCTL_NUM1 IOCTL_START_NUM+1
#define IOCTL_NUM2 IOCTL_START_NUM+2
#define IOCTL_NUM3 IOCTL_START_NUM+3
#define IOCTL_NUM4 IOCTL_START_NUM+4

#define MOD_IOCTL_NUM 'z'
#define GET_IOCTL  _IOWR(MOD_IOCTL_NUM, IOCTL_NUM1, unsigned long)
#define SET_IOCTL  _IOWR(MOD_IOCTL_NUM, IOCTL_NUM2, unsigned long)
#define ADD_IOCTL  _IOWR(MOD_IOCTL_NUM, IOCTL_NUM3, unsigned long)
#define MUL_IOCTL  _IOWR(MOD_IOCTL_NUM, IOCTL_NUM4, unsigned long)


static long result = 0;
module_param(result, long, 0);

static int simple_mod_open(struct inode *inode, struct file *file){
	printk("simple_mod: open\n");
	return 0;
}

static int simple_mod_release(struct inode *inode, struct file *file){
        printk("simple_mod: release\n");
        return 0;
} 

static long simple_mod_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
	switch(cmd){
		case GET_IOCTL:
			printk("simple_mod: return result %ld \n", result);
			return result;
		case SET_IOCTL:
			printk("simple_mod: set result %ld to %ld \n", result, (long)arg);
			result = (long) arg;
			return result;
		case ADD_IOCTL:
			printk("simple_mod: add result %ld to %ld \n", result, result + (long)arg);
			result = result+(long) arg;
			return result;
		case MUL_IOCTL:
			printk("simple_mod: mul result %ld to %ld \n", result, result * (long)arg);
			result = result*(long) arg;
			return result;
		default:
			printk("any exist not cmd");
			return -1;
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
	printk("simple_mod: init module\n");

	alloc_chrdev_region(&dev_num, 0, 1, DEV_NAME);
	cd_cdev = cdev_alloc();
	cdev_init(cd_cdev, &simple_mod_fops);
	cdev_add(cd_cdev, dev_num, 1);

	return 0;
}

static void __exit simple_mod_exit(void){
	printk("simple_mod: exit module\n");

	cdev_del(cd_cdev);
	unregister_chrdev_region(dev_num, 1);
}

module_init(simple_mod_init);
module_exit(simple_mod_exit);



