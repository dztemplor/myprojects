/*                                                     
 * $Id: hello.c,v 1.5 2004/10/26 03:32:21 corbet Exp $ 
 */                                                    
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/rcupdate.h>
#include <linux/jiffies.h>
#include <linux/percpu-defs.h>

/*
this demos how completion used, the kthread waits for 5s, if rmmod before kthread exits,
the rmmod command waits for kthread finishes.
*/

MODULE_LICENSE("Dual BSD/GPL");
static struct task_struct *poll_thread1;
struct mutex lock;
int start;

DECLARE_COMPLETION(mr_comp);
static int poll_thrd(void *data)
{
	unsigned long j1;

	while (!kthread_should_stop()) {
		if (!start) {
			schedule();
			continue;
		}
		j1 = jiffies+HZ*5;
		while (time_before(jiffies, j1)) {
			schedule();
		}

		break;
	}
	complete(&mr_comp);
	printk("kthread done\n");
	return 0;
}

static int hello_init(void)
{
	poll_thread1 = kthread_create(poll_thrd, NULL, "thread1");
	kthread_bind(poll_thread1, 0);
	wake_up_process(poll_thread1);
	start = 1;
	
	return 0;
}

static void hello_exit(void)
{
	wait_for_completion(&mr_comp);
	printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
