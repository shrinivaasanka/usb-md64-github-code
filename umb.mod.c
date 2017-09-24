#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x939d23c1, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x8dcfe1d8, __VMLINUX_SYMBOL_STR(__asan_register_globals) },
	{ 0xeb2006be, __VMLINUX_SYMBOL_STR(__asan_unregister_globals) },
	{ 0xb35e0fcf, __VMLINUX_SYMBOL_STR(usb_deregister) },
	{ 0x1c4ab989, __VMLINUX_SYMBOL_STR(usb_register_driver) },
	{ 0xd2b09ce5, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0xfb42789d, __VMLINUX_SYMBOL_STR(__asan_store1_noabort) },
	{ 0x86d38e51, __VMLINUX_SYMBOL_STR(__asan_loadN_noabort) },
	{ 0xc76fe5ac, __VMLINUX_SYMBOL_STR(usb_register_dev) },
	{ 0x8d69e06b, __VMLINUX_SYMBOL_STR(usb_get_dev) },
	{ 0xfb578fc5, __VMLINUX_SYMBOL_STR(memset) },
	{ 0x7ece895f, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x52da9824, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0xb44ad4b3, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0x754d539c, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0xc079ace7, __VMLINUX_SYMBOL_STR(usb_bulk_msg) },
	{ 0xc499ae1e, __VMLINUX_SYMBOL_STR(kstrdup) },
	{ 0x69e59c53, __VMLINUX_SYMBOL_STR(virgo_kernel_analytics_conf) },
	{ 0x4c82c7e6, __VMLINUX_SYMBOL_STR(usb_free_urb) },
	{ 0xc146af6, __VMLINUX_SYMBOL_STR(usb_submit_urb) },
	{ 0xfb137e9a, __VMLINUX_SYMBOL_STR(__asan_store4_noabort) },
	{ 0xcfb82bbc, __VMLINUX_SYMBOL_STR(__asan_load1_noabort) },
	{ 0x362ef408, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0xe1618148, __VMLINUX_SYMBOL_STR(usb_alloc_coherent) },
	{ 0xc355fdbc, __VMLINUX_SYMBOL_STR(usb_alloc_urb) },
	{ 0xfbd0896e, __VMLINUX_SYMBOL_STR(__asan_store8_noabort) },
	{ 0xd39e03eb, __VMLINUX_SYMBOL_STR(usb_find_interface) },
	{ 0x76cc0dba, __VMLINUX_SYMBOL_STR(usb_deregister_dev) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x9cb729d, __VMLINUX_SYMBOL_STR(usb_put_dev) },
	{ 0xfd5d0082, __VMLINUX_SYMBOL_STR(__asan_load8_noabort) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x961155d1, __VMLINUX_SYMBOL_STR(__asan_load4_noabort) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=virgo_kernel_analytics";


MODULE_INFO(srcversion, "A4BEB04C11326F1054A8ADF");
