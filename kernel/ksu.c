#include <linux/export.h>
#include <linux/fs.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/init.h>

#include "allowlist.h"
#include "arch.h"
#include "core_hook.h"
#include "klog.h" // IWYU pragma: keep
#include "ksu.h"
#include "throne_tracker.h"

unsigned int enable_kernelsu = 1;

static int __init read_kernelsu_state(char *s)
{
	if (s)
		enable_kernelsu = simple_strtoul(s, NULL, 0);

	return 1;
}
__setup("kernelsu.enabled=", read_kernelsu_state);

unsigned int get_ksu_state(void)
{
	return enable_kernelsu;
}

#ifdef CONFIG_KSU_SAFE_MODE
unsigned int ksu_safe_mode = 1;
#else
unsigned int ksu_safe_mode = 0;
#endif

static int __init is_ksu_safe_mode(char *s)
{
	if (s)
		ksu_safe_mode = simple_strtoul(s, NULL, 0);
	
	if (ksu_safe_mode > 1)
		ksu_safe_mode = 1;

	return 1;
}
__setup("kernelsu.safemode=", is_ksu_safe_mode);

unsigned int get_ksu_safe_mode_state(void)
{
	return ksu_safe_mode;
}

static struct workqueue_struct *ksu_workqueue;

bool ksu_queue_work(struct work_struct *work)
{
	return queue_work(ksu_workqueue, work);
}

extern int ksu_handle_execveat_sucompat(int *fd, struct filename **filename_ptr,
					void *argv, void *envp, int *flags);

extern int ksu_handle_execveat_ksud(int *fd, struct filename **filename_ptr,
				    void *argv, void *envp, int *flags);

int ksu_handle_execveat(int *fd, struct filename **filename_ptr, void *argv,
			void *envp, int *flags)
{
	ksu_handle_execveat_ksud(fd, filename_ptr, argv, envp, flags);
	return ksu_handle_execveat_sucompat(fd, filename_ptr, argv, envp,
					    flags);
}

extern void ksu_sucompat_init();
extern void ksu_sucompat_exit();
extern void ksu_ksud_init();
extern void ksu_ksud_exit();

int __init kernelsu_init(void)
{

	if (enable_kernelsu < 1) {
		pr_info_once(" is disabled");
		return 0;
	}

#ifdef CONFIG_KSU_DEBUG
	pr_alert("*************************************************************");
	pr_alert("**     NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE    **");
	pr_alert("**                                                         **");
	pr_alert("**         You are running KernelSU in DEBUG mode          **");
	pr_alert("**                                                         **");
	pr_alert("**     NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE    **");
	pr_alert("*************************************************************");
#endif

	ksu_core_init();

	ksu_workqueue = alloc_ordered_workqueue("kernelsu_work_queue", 0);

	ksu_allowlist_init();

	ksu_throne_tracker_init();

#ifdef CONFIG_KPROBES
	ksu_sucompat_init();
	ksu_ksud_init();
#else
	pr_alert("KPROBES is disabled, KernelSU may not work, please check https://kernelsu.org/guide/how-to-integrate-for-non-gki.html");
#endif

#ifdef MODULE
#ifndef CONFIG_KSU_DEBUG
	kobject_del(&THIS_MODULE->mkobj.kobj);
#endif
#endif
	return 0;
}

void kernelsu_exit(void)
{
	if (enable_kernelsu < 1)
		return;
	
	ksu_allowlist_exit();

	ksu_throne_tracker_exit();

	destroy_workqueue(ksu_workqueue);

#ifdef CONFIG_KPROBES
	ksu_ksud_exit();
	ksu_sucompat_exit();
#endif

	ksu_core_exit();
}

module_init(kernelsu_init);
module_exit(kernelsu_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("weishu");
MODULE_DESCRIPTION("Android KernelSU");

// Fix MODULE_IMPORT_NS error
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)
MODULE_IMPORT_NS(VFS_internal_I_am_really_a_filesystem_and_am_NOT_a_driver);
#endif