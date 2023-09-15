#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0xa4f461dd, "module_layout" },
	{ 0x9ffea724, "single_release" },
	{ 0x78107c54, "seq_read" },
	{ 0x37c0393b, "seq_lseek" },
	{ 0xf4d468d8, "proc_create" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0xc5850110, "printk" },
	{ 0xdecd0b29, "__stack_chk_fail" },
	{ 0x9660b4a5, "cpu_hwcaps" },
	{ 0xa916b694, "strnlen" },
	{ 0xbcab6ee6, "sscanf" },
	{ 0xaf507de1, "__arch_copy_from_user" },
	{ 0xb384ca2e, "cpu_hwcap_keys" },
	{ 0x14b89635, "arm64_const_caps_ready" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x317ffbb, "seq_printf" },
	{ 0xff0a9394, "single_open" },
	{ 0x1fdc7df2, "_mcount" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "2F5B7AFF47BCE079FCF7629");
