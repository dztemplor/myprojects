/*                                                     
 * $Id: hello.c,v 1.5 2004/10/26 03:32:21 corbet Exp $ 
 */                                                    
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/mutex.h>

MODULE_LICENSE("Dual BSD/GPL");
static struct task_struct *poll_thread1, *poll_thread2;
struct mutex lock;
int start;
int g;

static int poll_thrd(void *data)
{
	int i;
	while (!kthread_should_stop()) {
		if (!start) {
			schedule();
			continue;
		}
		for (i=0; i<1000000; i++) {
			mutex_lock(&lock);
			g++;
			mutex_unlock(&lock);
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
	poll_thread1 = kthread_run(poll_thrd, NULL, "thread1");
	poll_thread2 = kthread_run(poll_thrd, NULL, "thread2");
	printk("HZ %d\n", HZ);
	start = 1;
	
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Goodbye, cruel world\n");
	//kthread_stop(poll_thread1);
	//kthread_stop(poll_thread2);
	printk("dz %d\n", g);
}



module_init(hello_init);
module_exit(hello_exit);
