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
1. this file demos how to sync between tasklet and kthread
2. if define RUN_ON_SAME_CPU, tasklet and kthread both run on cpu 0; if not, kernel
will likely separte them on two cpus.
3. if run on same cpu, kthread must use spin_lock_bh(), otherwise deadlock appears when tasklet preempt kthread while kthread holds the lock.
4. if not run on same cpu, kthread may use spin_lock(), deadlock won't happen.
5. conclusion: it's always safe to use spin_lock_bh() when kthread shares data with tasklet.
*/

MODULE_LICENSE("Dual BSD/GPL");
static struct task_struct *poll_thread1;
struct mutex lock;
int start;
int g, g_tasklet;

void my_tasklet_handler(unsigned long data);
DECLARE_TASKLET(my_tasklet, my_tasklet_handler, 0);
DEFINE_SPINLOCK(my_lock);

struct fox {
	unsigned long tail_length; /* length in centimeters of tail */
	struct list_head list; /* list of all fox structures */
};


static LIST_HEAD(fox_list);

#define RUN_ON_SAME_CPU
void my_tasklet_handler(unsigned long data)
{
	struct fox *pos, *next;

	spin_lock(&my_lock);
	list_for_each_entry_safe(pos, next, &fox_list, list) {
		list_del(&pos->list);
		kfree(pos);
	}
	udelay(1);
	spin_unlock(&my_lock);

	printk_once("tasklet %x\n", smp_processor_id());
	if (++g_tasklet != 1000000)
		tasklet_schedule(&my_tasklet);
	else
		printk("dz tasklet done, g %d\n", g);
}

static int poll_thrd(void *data)
{
	int i;
#ifdef RUN_ON_SAME_CPU
	static int first=1;
	if (first) {
		first = 0;
		tasklet_schedule(&my_tasklet);
	}
#endif
	preempt_disable();
	printk_once("thrd %x\n", smp_processor_id());
	preempt_enable();
	while (!kthread_should_stop()) {
		if (!start) {
			schedule();
			continue;
		}
		for (i=0; i<1000000; i++) {
			struct fox * n;
			spin_lock_bh(&my_lock);
			n = kmalloc(sizeof(*n), GFP_KERNEL);
			if (n)
				list_add(&n->list, &fox_list);
			spin_unlock_bh(&my_lock);
			schedule_timeout(1);
		}
		break;
	}
	printk("done %d\n", g);
	return 0;
}


static int hello_init(void)
{
	mutex_init(&lock);
	poll_thread1 = kthread_create(poll_thrd, NULL, "thread1");
	if (likely(!IS_ERR(poll_thread1))) {
#ifdef RUN_ON_SAME_CPU		
		kthread_bind(poll_thread1, 0);
#endif		
		wake_up_process(poll_thread1);
	}
#ifndef RUN_ON_SAME_CPU	
	tasklet_schedule(&my_tasklet);
#endif	
	start = 1;
	
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Goodbye, cruel world\n");
	//kthread_stop(poll_thread1);
	//kthread_stop(poll_thread2);
	tasklet_kill(&my_tasklet);
	printk("dz %d\n", g);
}



module_init(hello_init);
module_exit(hello_exit);
