#include <linux/init.h>
#include <linux/module.h>

int my_id = 0;
int set_my_id(int id)
{
	my_id = id;
	if(my_id == id){
		return 1;
		
	}else {
		return 0;
	}
}

int get_my_id(void) 
{
	return my_id;
}

EXPORT_SYMBOL(set_my_id);
EXPORT_SYMBOL(get_my_id);

static int __init mod1_init(void)
{
	return 0;
}

static void __exit mod1_exit(void)
{
}

module_init(mod1_init);
module_exit(mod1_exit);


