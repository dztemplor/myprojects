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
#include <linux/percpu-defs.h>
/*
this example demos percpu usage.
1. if assign poll_thread1 and poll_thread2 to different cpus, g will change to 1000000, 1000000 in the end.
2. if assign poll_thread1 and poll_thread2 to same cpu, g will change to 2000000 on cpu0, and stay as 0 on cpu1.
3. percpu has many variants, you can access percpu as value or pointer, check more kernel code.
*/

MODULE_LICENSE("Dual BSD/GPL");
static struct task_struct *poll_thread1, *poll_thread2;
struct mutex lock;
int start;
int *g;
//static DEFINE_PER_CPU(int, g);

static int poll_thrd(void *data)
{
	int i, *tmp = NULL;

	while (!kthread_should_stop()) {
		if (!start) {
			schedule();
			continue;
		}
		for (i=0; i<1000000; i++) {
			tmp = this_cpu_ptr(g);
			++(*tmp);
			put_cpu_var(g);
			schedule_timeout(1);
		}
		break;
	}
	printk("done %d\n", *tmp);
	return 0;
}

static int hello_init(void)
{
	g = alloc_percpu(int);
	poll_thread1 = kthread_create(poll_thrd, NULL, "thread1");
	kthread_bind(poll_thread1, 0);
	wake_up_process(poll_thread1);

	poll_thread2 = kthread_create(poll_thrd, NULL, "thread2");
	kthread_bind(poll_thread2, 0);
	wake_up_process(poll_thread2);
	
	start = 1;
	
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Goodbye, cruel world\n");
	printk("dz %d %d\n", *per_cpu_ptr(g, 0), *per_cpu_ptr(g,1));
	free_percpu(g);
}

module_init(hello_init);
module_exit(hello_exit);
