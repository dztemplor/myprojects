/*                                                     
 * $Id: hello.c,v 1.5 2004/10/26 03:32:21 corbet Exp $ 
 */                                                    
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/mutex.h>
#include <linux/workqueue.h>

/*
this example demos kthread and workqueue usage. my_work1 use default work queue; my_work2 uses
specific work queue.
*/

MODULE_LICENSE("Dual BSD/GPL");
static struct task_struct *poll_thread1;
struct mutex lock;
int start;
int g;

static void work_func(struct work_struct * data);
struct workqueue_struct *my_wq;

DECLARE_WORK(my_work1, work_func);
DECLARE_WORK(my_work2, work_func);


static int poll_thrd(void *data)
{
	int i;
	while (!kthread_should_stop()) {
		if (!start) {
			schedule();
			continue;
		}
		schedule_work(&my_work1);
		queue_work(my_wq, &my_work2);
		for (i=0; i<1000000; i++) {
			mutex_lock(&lock);
			g++;
			mutex_unlock(&lock);
			schedule_timeout(1);
		}
		break;
	}
	printk("done %d\n", g);
	preempt_disable();
	printk_once("poll_thrd cpu %x %x\n", smp_processor_id(), preempt_count());
	preempt_enable();
	return 0;
}
void work_func(struct work_struct * data)
{
	int i;
	if (data == &my_work1)
		printk_once("work 1: cpu %x %x\n", smp_processor_id(), preempt_count());
	else if (data == &my_work2)
		printk_once("work 2: cpu %x %x\n", smp_processor_id(), preempt_count());
	
	for (i=0; i<1000000; i++) {
		mutex_lock(&lock);
		g++;
		mutex_unlock(&lock);
		schedule_timeout(1);
	}
}

static int hello_init(void)
{
	mutex_init(&lock);
	poll_thread1 = kthread_run(poll_thrd, NULL, "thread1");
	my_wq = create_workqueue("events");
	start = 1;
	
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Goodbye, cruel world\n");
	//kthread_stop(poll_thread1);
	//kthread_stop(poll_thread2);
	flush_scheduled_work();
	flush_workqueue(my_wq);
	printk("dz %d\n", g);
}



module_init(hello_init);
module_exit(hello_exit);
