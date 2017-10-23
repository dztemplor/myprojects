
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
/*
this examples copies from ldd3, it shows how to use waitqueue.
note scull_getwritespace() is a raw method of using waitqueue, should use wait_event_interruptible()
and wake_up_interruptible().

*/


struct scull_pipe {
        wait_queue_head_t inq, outq;       /* read and write queues */
        char *buffer, *end;                /* begin of buf, end of buf */
        int buffersize;                    /* used in pointer arithmetic */
        char *rp, *wp;                     /* where to read, where to write */
        int nreaders, nwriters;            /* number of openings for r/w */
        struct mutex mutex;              /* mutual exclusion semaphore */
};
static struct scull_pipe *dev;

static int spacefree(struct scull_pipe *dev);
/*
 *	Now all the various file operations that we export.
 */
#if 0
#define mutex_lock_interruptible(x) 0
#define mutex_lock(x)
#define mutex_unlock(x)
#endif
static ssize_t my_misc_read(struct file *file, char __user *buf,
			size_t count, loff_t *ppos)
{
	struct scull_pipe *dev= file->private_data;

	if (mutex_lock_interruptible(&dev->mutex))
		return -ERESTARTSYS;

	while(dev->rp == dev->wp) {
		mutex_unlock(&dev->mutex);

		if (wait_event_interruptible(dev->inq, (dev->rp!=dev->wp)))
			return -ERESTARTSYS;

		if (mutex_lock_interruptible(&dev->mutex))
			return -ERESTARTSYS;
	}

	if (dev->wp > dev->rp)
		count = min(count, (size_t)(dev->wp - dev->rp));
	else
		count = min(count, (size_t)(dev->end-dev->rp));
	if (copy_to_user(buf, dev->rp, count)) {
		mutex_unlock(&dev->mutex);
		return -EFAULT;
	}
	dev->rp += count;
	if (dev->rp==dev->end)
		dev->rp = dev->buffer;
	mutex_unlock(&dev->mutex);
	wake_up_interruptible(&dev->outq);
	return count;
}
#if 0
static int scull_getwritespace(struct scull_pipe *dev, struct file *file)
{
	while(spacefree(dev) ==0) {
		DEFINE_WAIT(wait);
		mutex_unlock(&dev->mutex);
		prepare_to_wait(&dev->outq, &wait, TASK_INTERRUPTIBLE);
		if (spacefree(dev) == 0)
			schedule();
		finish_wait(&dev->outq, &wait);
		if (mutex_lock_interruptible(&dev->mutex))
			return -ERESTARTSYS;
	}
	return 0;
}
#endif
static int spacefree(struct scull_pipe *dev)
{
	if (dev->rp == dev->wp)
		return dev->buffersize -1;
	return ((dev->rp + dev->buffersize - dev->wp)%dev->buffersize) -1;
}

static ssize_t my_misc_write(struct file *file, const char __user *buf,
			size_t count, loff_t *ppos)
{
	struct scull_pipe *dev= file->private_data;

	if (mutex_lock_interruptible(&dev->mutex))
		return -ERESTARTSYS;

	while(spacefree(dev) == 0) {
		mutex_unlock(&dev->mutex);

		if (wait_event_interruptible(dev->outq, (spacefree(dev))))
			return -ERESTARTSYS;

		if (mutex_lock_interruptible(&dev->mutex))
			return -ERESTARTSYS;
	}
	
	count =min(count, (size_t)spacefree(dev));
	
	if (dev->wp >= dev->rp)
		count =min(count, (size_t)(dev->end - dev->wp));
	else
		count = min(count, (size_t)(dev->rp - dev->wp-1));

	if (copy_from_user(dev->wp, buf, count)) {
		mutex_unlock(&dev->mutex);
		return -EFAULT;
	}
	dev->wp+= count;
	if (dev->wp == dev->end)
		dev->wp = dev->buffer;
	mutex_unlock(&dev->mutex);

	wake_up_interruptible(&dev->inq);
	printk("write %d\n", count);	
	return count;
}

static unsigned int my_misc_poll(struct file *file,
				 struct poll_table_struct *wait)
{
	struct scull_pipe *dev = file->private_data;
	unsigned int mask = 0;

	/*
	 * The buffer is circular; it is considered full
	 * if "wp" is right behind "rp" and empty if the
	 * two are equal.
	 */
	mutex_lock(&dev->mutex);
	poll_wait(file, &dev->inq,  wait);
	poll_wait(file, &dev->outq, wait);
	if (dev->rp != dev->wp)
		mask |= POLLIN | POLLRDNORM;	/* readable */
	if (spacefree(dev))
		mask |= POLLOUT | POLLWRNORM;	/* writable */
	mutex_unlock(&dev->mutex);
	return mask;
}


/*
 *	We enforce only one user at a time here with the open/close.
 *	Also clear the previous interrupt data on an open, and clean
 *	up things on a close.
 */
static int my_misc_open(struct inode *inode, struct file *file)
{
	printk("dz in misc_open\n");

	file->private_data = dev;	
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

	dev = kmalloc(sizeof(*dev), GFP_KERNEL);

	dev->buffer  = kmalloc(100, GFP_KERNEL);
	dev->buffersize = 100;
	dev->end = dev->buffer+dev->buffersize;
	dev->rp = dev->wp =dev->buffer;
	init_waitqueue_head(&(dev->inq));
	init_waitqueue_head(&(dev->outq));
	mutex_init(&dev->mutex);
	
	retval = misc_register(&my_misc_dev);
	if (retval < 0)
		return retval;

	return 0;
}

static void __exit my_misc_exit(void)
{
	kfree(dev);
	misc_deregister(&my_misc_dev);
}


module_init(my_misc_init);
module_exit(my_misc_exit);

