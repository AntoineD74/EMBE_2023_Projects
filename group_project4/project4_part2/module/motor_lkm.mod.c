#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
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
__used __section("__versions") = {
	{ 0x92997ed8, "_printk" },
	{ 0xc1514a3b, "free_irq" },
	{ 0x882ce6fa, "gpio_to_desc" },
	{ 0x85eae08d, "gpiod_unexport" },
	{ 0xfe990052, "gpio_free" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0xb4f9efb7, "__register_chrdev" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xc62a4969, "gpiod_direction_input" },
	{ 0x601b1f5c, "gpiod_set_debounce" },
	{ 0x6fd9bfb8, "gpiod_export" },
	{ 0x907b1f36, "gpiod_get_raw_value" },
	{ 0x2bf839e4, "gpiod_to_irq" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0xd575b280, "param_ops_charp" },
	{ 0x8f80e6e5, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "DE82318FC0EC78039A38DF3");
