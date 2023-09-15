#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");

#define SENSOR1 17
#define LED 5

static int irq_num;

struct my_timer_info {
	struct timer_list timer;
	long delay_jiffies;
};

static struct my_timer_info my_timer;

static irqreturn_t ch6_sensor_isr(int irq, void* dev_id) {
	printk("detect from sensor ");
	unsigned long flags;
//	local_irq_save(flags);
	gpio_set_value(LED, 1);
	mod_timer(&my_timer.timer, jiffies + msecs_to_jiffies(2000));
//	local_irq_restore(flags);
	return IRQ_HANDLED;
}

static void my_timer_func(struct timer_list *t) {
	// 2초 후 종료
	gpio_set_value(LED, 0);
}

//static dev_t dev_num;
//static struct cdev *cd_cdev;

static int __init ch6_sensor_init(void) {
	int ret;
	printk("init module \n");

	my_timer.delay_jiffies = msecs_to_jiffies(2000);
	timer_setup(&my_timer.timer, my_timer_func, 0);
	my_timer.timer.expires = jiffies + my_timer.delay_jiffies;

	gpio_request_one(SENSOR1, GPIOF_IN, "sensor1");
	gpio_request_one(LED, GPIOF_OUT_INIT_LOW, "LED");

	irq_num = gpio_to_irq(SENSOR1);
	ret = request_irq(irq_num, ch6_sensor_isr, IRQF_TRIGGER_RISING, "sensor_irq", NULL);
	if(ret) {
		printk("Unable to reset request IRQ : %d\n", irq_num);
		free_irq(irq_num, NULL);
	} else {
		printk("Enable to set request IRQ: %d\n", irq_num);
	}

	return 0;
}


static void __exit ch6_sensor_exit(void){
	printk("exit module \n");

	del_timer(&my_timer.timer);
	disable_irq(irq_num);
	free_irq(irq_num, NULL);
	gpio_free(SENSOR1);
	gpio_free(LED);
}

module_init(ch6_sensor_init);
module_exit(ch6_sensor_exit);
