/*                                                     
 * $Id: hello.c,v 1.5 2004/10/26 03:32:21 corbet Exp $ 
 */                                                    
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/mutex.h>
#include <linux/interrupt.h>
#include <linux/smp.h>
#include <linux/preempt.h>
#include <linux/slab.h>

/*
1. this file demos how to sync between two tasklets.
2. if define WANT_CRASH, you can see crash if my_tasklet and my_tasklet2 run on different cpus.
   in this case, spin_lock is enough, no need spin_lock_bh()
3. if my_tasklet and my_tasklet2 run on same cpu, don't call spin_lock() won't crash
*/

MODULE_LICENSE("Dual BSD/GPL");
int g, g_tasklet;

void my_tasklet_handler(unsigned long data);
void my_tasklet_handler2(unsigned long data);
DECLARE_TASKLET(my_tasklet, my_tasklet_handler, 0);
DECLARE_TASKLET(my_tasklet2, my_tasklet_handler2, 0);
DEFINE_SPINLOCK(my_lock);
static struct task_struct *poll_thread1;

struct fox {
	unsigned long tail_length; /* length in centimeters of tail */
	struct list_head list; /* list of all fox structures */
};


static LIST_HEAD(fox_list);

//#define WANT_CRASH
void my_tasklet_handler(unsigned long data)
{
	struct fox *pos, *next;

#ifndef WANT_CRASH	
	spin_lock(&my_lock);
#endif	
	list_for_each_entry_safe(pos, next, &fox_list, list) {
		list_del(&pos->list);
		kfree(pos);
	}
	udelay(1);
#ifndef WANT_CRASH	
	spin_unlock(&my_lock);
#endif	

	printk_once("tasklet %x\n", smp_processor_id());
	if (++g_tasklet != 1000000)
		tasklet_schedule(&my_tasklet);
	else
		printk("dz tasklet done, g %d\n", g);
}

void my_tasklet_handler2(unsigned long data)
{
	int i;
	printk_once("tasklet2 %x\n", smp_processor_id());
	for (i=0; i<1000000; i++) {
		struct fox * n;
#ifndef WANT_CRASH		
		spin_lock(&my_lock);
#endif		
		n = kmalloc(sizeof(*n), GFP_KERNEL);
		if (n)
			list_add(&n->list, &fox_list);
		udelay(1);
#ifndef WANT_CRASH		
		spin_unlock(&my_lock);
#endif		
	}
	printk("dz tasklet2 done, g %d\n", g);
}

static int poll_thrd(void *data)
{
	tasklet_schedule(&my_tasklet2);
	return 0;
}



static int hello_init(void)
{
	poll_thread1 = kthread_create(poll_thrd, NULL, "thread1");
	if (likely(!IS_ERR(poll_thread1))) {
		kthread_bind(poll_thread1, 1);
		wake_up_process(poll_thread1);
	}

	tasklet_schedule(&my_tasklet);
	
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Goodbye, cruel world\n");
	tasklet_kill(&my_tasklet);
	tasklet_kill(&my_tasklet2);
}



module_init(hello_init);
module_exit(hello_exit);
