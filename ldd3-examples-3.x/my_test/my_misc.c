
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
#include <linux/slab.h>
#include <linux/mm.h>       

#include <asm/uaccess.h>
/*this example demos misc device, and advanced driver operations like mmap() and poll()
1. mmap() with simple_remap_mmap() makes /dev/my_misc like /dev/mem, as it maps physical addr
into user space, try ldd3 mapper with "./mapper /dev/my_misc $pci_bar_addr 0", and check 
devmem2 output, see they are same.
2 page fault handler.
this is more useful when exporting driver memory to user space.
in this simple example, I just export a static page to userspace, each time mmap()/unmap() will
change the memory content. don't support offset.
*/


static char my_buf[128];
struct ctx {
	struct page * page;
};

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
	struct ctx * p;
	printk("dz in misc_open\n");

	p = kmalloc(sizeof(*p), GFP_KERNEL);
	p->page= alloc_page(GFP_KERNEL);
	file->private_data = p;

	
	return 0;
}

static int my_misc_release(struct inode *inode, struct file *file)
{
	/*
	 * Turn off all interrupts once the device is no longer
	 * in use and clear the data.
	 */
	struct ctx *p;
	p = file->private_data;
	printk("dz in misc_release\n");
	__free_pages(p->page, 0);
	kfree(p);
	return 0;
}
static int g;
void simple_vma_open(struct vm_area_struct *vma)
{
	
	struct ctx *c=vma->vm_private_data;
	struct page *page = NULL;
	void * p;
	
	page = c->page;
	p = page_address(page);
	
	memset(p, ++g, PAGE_SIZE);
	printk("in %s\n", __func__);
}

void simple_vma_close(struct vm_area_struct *vma)
{
	printk("in %s\n", __func__);
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

int scullp_vma_fault(struct vm_area_struct *vma,
                                struct vm_fault *vmf)
{
	struct ctx *c=vma->vm_private_data;
	struct page *page = NULL;


	page = c->page;

	get_page(page);
	if (!page)
		return VM_FAULT_SIGBUS;
	vmf->page = page;
	return 0;
}


struct vm_operations_struct scullp_vm_ops = {
	.open =     simple_vma_open,
	.close =    simple_vma_close,
	.fault =   scullp_vma_fault,
};


int scullp_mmap(struct file *filp, struct vm_area_struct *vma)
{
	//struct inode *inode = filp->f_dentry->d_inode;

	/* don't do anything here: "nopage" will set up page table entries */
	vma->vm_ops = &scullp_vm_ops;
	vma->vm_flags |= (VM_DONTEXPAND | VM_DONTDUMP);
	vma->vm_private_data = filp->private_data;
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
#if 0	
	.mmap    = simple_remap_mmap,
#else
	.mmap    = scullp_mmap,
#endif
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

