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

/*
this example demos how to use rcu list, check kernel/Documentation/RCU/listRCU.txt for details.
the program first initialize a list, then start 3 thread, one thread reads, the other 2 write.
1. if define NO_RCU, you can see crash.
2. if don't mutex_lock(), you can see write thread crashes.
   so rcu only works for multi-reader, one-writer. if there are multi-writer, you still need a lock (spin_lock or mutex)
*/

MODULE_LICENSE("Dual BSD/GPL");
static struct task_struct *poll_thread1, *poll_thread2, *poll_thread3;
struct mutex lock;
int start, end;

struct fox {
	unsigned long tail_length; /* length in centimeters of tail */
	struct list_head list; /* list of all fox structures */
	struct rcu_head rcu;
};

static LIST_HEAD(fox_list);

//#define NO_RCU
static int poll_thrd(void *data)
{
	int t;
	struct fox *f;
	int i=0;
	while (!kthread_should_stop()) {
		if (!start) {
			schedule();
			continue;
		}
		
		while (!end) {
#ifdef NO_RCU
			list_for_each_entry(f, &fox_list, list) {
				t = f->tail_length;
			}
						
#else
			rcu_read_lock();
			list_for_each_entry_rcu(f, &fox_list, list) {
				t = f->tail_length;
				i++;
			}
			rcu_read_unlock();
#endif			
			schedule_timeout(1);
		}
		break;
	}
	printk("done 1 %d\n", i);
	return 0;
}

static int poll_thrd2(void *data)
{
	int i;
	struct fox *pos, *next;
	while (!kthread_should_stop()) {
		if (!start) {
			schedule();
			continue;
		}
		for (i=0; i<1; i++) {
#ifdef NO_RCU
			list_for_each_entry_safe(pos, next, &fox_list, list) {
				list_del(&pos->list);
				kfree(pos);
				schedule_timeout(1);
			}
#else
			mutex_lock(&lock);
			list_for_each_entry_safe(pos, next, &fox_list, list) {
				list_del_rcu(&pos->list);
				kfree_rcu(pos, rcu);
				schedule_timeout(1);
			}
			mutex_unlock(&lock);
#endif

		}
		break;
	}
	printk("done 2\n");
	end = 1;
	return 0;
}

static int hello_init(void)
{
	int i;
	for (i=0; i<1000000; i++) {
		struct fox * n;
		n = kmalloc(sizeof(*n), GFP_KERNEL);
		if (n)
			list_add(&n->list, &fox_list);
	}
	mutex_init(&lock);
	
	poll_thread1 = kthread_run(poll_thrd, NULL, "thread1");
	poll_thread2 = kthread_run(poll_thrd2, NULL, "thread2");
	poll_thread3 = kthread_run(poll_thrd2, NULL, "thread3");
	start = 1;
	
	return 0;
}

static void hello_exit(void)
{

	printk(KERN_ALERT "Goodbye, cruel world\n");

}

module_init(hello_init);
module_exit(hello_exit);
