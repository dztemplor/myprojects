
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/fcntl.h>

#include <linux/rtc.h>
#include <linux/init.h>
#include <linux/poll.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/mutex.h>
#include <linux/workqueue.h>
#include <linux/fs.h>
#include <linux/mm.h>       

#include <asm/uaccess.h>
/*this example demos misc device, and advanced driver operations like mmap() and poll()
1. mmap() with simple_remap_mmap() makes /dev/my_misc like /dev/mem, as it maps physical addr
into user space, try ldd3 mapper with "./mapper /dev/my_misc $pci_bar_addr 0", and check 
devmem2 output, see they are same.*/


static char my_buf[128];
/*
 *	Now all the various file operations that we export.
 */
static ssize_t my_misc_read(struct file *file, char __user *buf,
			size_t count, loff_t *ppos)
{
	unsigned long n;
	n = (count < 127)? count:127;
	if (copy_to_user(buf, my_buf, n))
		return -EFAULT;
	return n;
}

static ssize_t my_misc_write(struct file *file, const char __user *buf,
			size_t count, loff_t *ppos)
{
	unsigned long n;
	n = (count < 127)? count:127;
	if (copy_from_user(my_buf, buf, n))
		return -EFAULT;
	return n;
}

static unsigned int my_misc_poll(struct file *file,
				 struct poll_table_struct *wait)
{
	return 0;
}

#if 0
static int my_misc_ioctl(struct file *file,
			 unsigned int cmd, unsigned long arg)
{
	return -EINVAL;
}
#endif

/*
 *	We enforce only one user at a time here with the open/close.
 *	Also clear the previous interrupt data on an open, and clean
 *	up things on a close.
 */

static int my_misc_open(struct inode *inode, struct file *file)
{
	printk("dz in misc_open\n");
	return 0;
}

static int my_misc_release(struct inode *inode, struct file *file)
{
	/*
	 * Turn off all interrupts once the device is no longer
	 * in use and clear the data.
	 */
	printk("dz in misc_release\n");
	return 0;
}
void simple_vma_open(struct vm_area_struct *vma)
{
	printk(KERN_NOTICE "Simple VMA open, virt %lx, phys %lx\n",
			vma->vm_start, vma->vm_pgoff << PAGE_SHIFT);
}

void simple_vma_close(struct vm_area_struct *vma)
{
	printk(KERN_NOTICE "Simple VMA close.\n");
}


/*
 * The remap_pfn_range version of mmap.  This one is heavily borrowed
 * from drivers/char/mem.c.
 */

static struct vm_operations_struct simple_remap_vm_ops = {
	.open =  simple_vma_open,
	.close = simple_vma_close,
};

static int simple_remap_mmap(struct file *filp, struct vm_area_struct *vma)
{
	if (remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff,
			    vma->vm_end - vma->vm_start,
			    vma->vm_page_prot))
		return -EAGAIN;

	vma->vm_ops = &simple_remap_vm_ops;
	simple_vma_open(vma);
	return 0;
}



/*
 *	The various file operations we support.
 */

static const struct file_operations my_misc_fops = {
	.owner		= THIS_MODULE,
	.read		= my_misc_read,
	.write		= my_misc_write,	
	.poll		= my_misc_poll,
	.open		= my_misc_open,
	.release	= my_misc_release,
	.llseek		= noop_llseek,
	.mmap    = simple_remap_mmap,
};

static struct miscdevice my_misc_dev =
{
	.minor		= MISC_DYNAMIC_MINOR,
	.name		= "my_misc",
	.fops		= &my_misc_fops,
};

static int __init my_misc_init(void)
{
	int retval;

	retval = misc_register(&my_misc_dev);
	if (retval < 0)
		return retval;

	return 0;
}

static void __exit my_misc_exit(void)
{
	misc_deregister(&my_misc_dev);
}


module_init(my_misc_init);
module_exit(my_misc_exit);

