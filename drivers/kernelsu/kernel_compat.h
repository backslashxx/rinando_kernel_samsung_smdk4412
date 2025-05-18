#ifndef __KSU_H_KERNEL_COMPAT
#define __KSU_H_KERNEL_COMPAT

#include "linux/fs.h"
#include "linux/key.h"
#include "linux/version.h"
#include "linux/key.h"

extern long ksu_strncpy_from_user_nofault(char *dst,
					  const void __user *unsafe_addr,
					  long count);

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 10, 0) || defined(CONFIG_KSU_ALLOWLIST_WORKAROUND)
extern struct key *init_session_keyring;
#endif

extern void ksu_android_ns_fs_check();
extern struct file *ksu_filp_open_compat(const char *filename, int flags,
					 umode_t mode);
extern ssize_t ksu_kernel_read_compat(struct file *p, void *buf, size_t count,
				      loff_t *pos);
extern ssize_t ksu_kernel_write_compat(struct file *p, const void *buf,
				       size_t count, loff_t *pos);

extern int ksu_access_ok(const void *addr, unsigned long size);

// this is backported on msm-3.10 though SO YEAH WE STILL USE IT IF ITS THERE !! (ref: Makefile)
// but we have to try to follow what upstream linux is, and it is only added on 3.11
// need to inline struct dir_context since this doesnt exist pre-iterate_dir era!
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 11, 0) && !defined(KSU_HAS_ITERATE_DIR)
struct dir_context {
	const filldir_t actor;
	loff_t pos;
};

// ref: https://github.com/torvalds/linux/commit/5c0ba4e0762e6dabd14a5c276652e2defec38de7
// analysis: 
// int kernel_iterate_dir(struct file *file, struct dir_context *ctx) -> res = readdir(file, ctx, ctx->actor);  ++++ file, struct -> file, struct, struct->member
// int vfs_readdir(struct file *file, filldir_t filler, void *buf) -> res = readdir(file, buf, filler); file, ??, ?? +++ 1 3 2
static int iterate_dir(struct file *file, struct dir_context *ctx)
{
	return vfs_readdir(file, ctx->actor, ctx);
}
#endif // CONFIG_KSU_HAS_ITERATE_DIR

#endif
