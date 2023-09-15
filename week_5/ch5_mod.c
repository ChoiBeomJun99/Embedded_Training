#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/timer.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");

#define LED 5
#define SWITCH 12 


struct my_timer_info {
	struct timer_list timer;
	long delay_jiffies;
};

static struct my_timer_info my_timer;

static void my_timer_func(struct timer_list *t){
	struct my_timer_info *info = from_timer(info, t, timer);
	gpio_request_one(SWITCH, GPIOF_IN, "SWITCH");
	gpio_request_one(LED, GPIOF_OUT_INIT_LOW, "LED");

	printk("simple_timer: jiffies %ld\n", jiffies); 

	int ret = 0;
	ret = gpio_get_value(SWITCH);
	gpio_set_value(LED, ret);
	
	/*
	if(ret) {
		gpio_set_value(LED, 1);
	} else {
		gpio_set_value(LED, 0);
	}
	*/

	mod_timer(&my_timer.timer, jiffies + info->delay_jiffies);
}

static int __init simple_timer_init(void) {
	printk("simple_timer: init modules \n");

	my_timer.delay_jiffies = msecs_to_jiffies(500); 
	timer_setup(&my_timer.timer, my_timer_func, 0);
	my_timer.timer.expires = jiffies + my_timer.delay_jiffies;
	add_timer(&my_timer.timer);
	
	return 0;
}

static void __exit simple_timer_exit(void){
	printk("simple_switch : exit module\n");
	gpio_set_value(LED, 0);
	gpio_free(SWITCH);
	gpio_free(LED);
	del_timer(&my_timer.timer);
}

module_init(simple_timer_init);
module_exit(simple_timer_exit);

