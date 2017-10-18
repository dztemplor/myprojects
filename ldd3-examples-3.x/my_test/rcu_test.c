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

MODULE_LICENSE("Dual BSD/GPL");
static struct task_struct *poll_thread1, *poll_thread2;
struct mutex lock;
int start;
struct _s{
	struct rcu_head rcu;
	int i;
}*g;

static int poll_thrd(void *data)
{
	int i;
	struct _s tmp, *p;
	memset(&tmp, 0, sizeof(tmp));
	while (!kthread_should_stop()) {
		if (!start) {
			schedule();
			continue;
		}
		for (i=0; i<1000000; i++) {
			rcu_read_lock();
			p = rcu_dereference(g);
			printk_once("%p %p\n", p, g);
			tmp = *p;
			rcu_read_unlock();
			schedule_timeout(1);
		}
		break;
	}
	printk("done %d\n", tmp.i);
	return 0;
}

void free_func(struct rcu_head *head)
{
	struct _s *p = container_of(head, struct _s, rcu);
	printk_once("in free_func\n");
	kfree(p);
}

//#define USE_KFREE_RCU
static int poll_thrd2(void *data)
{
	int i;
	struct _s *p = NULL, *old;
	while (!kthread_should_stop()) {
		if (!start) {
			schedule();
			continue;
		}
		for (i=0; i<1000000; i++) {
			old = rcu_dereference(g);
			p = kmalloc(sizeof(struct _s), GFP_KERNEL);
			p->i = ++(g->i);
			RCU_INIT_POINTER(g, p);
#ifdef USE_KFREE_RCU
			kfree_rcu(old, rcu);
#else			
			call_rcu(&old->rcu, free_func);
#endif			
			schedule_timeout(1);
		}
		break;
	}
	printk("done %d\n", p->i);
	return 0;
}

static int hello_init(void)
{
	g = kmalloc(sizeof(struct _s), GFP_KERNEL);
	g->i = 0;
	poll_thread1 = kthread_run(poll_thrd, NULL, "thread1");
	poll_thread2 = kthread_run(poll_thrd2, NULL, "thread2");
	start = 1;
	
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Goodbye, cruel world\n");
	printk("dz %d\n", g->i);
}

module_init(hello_init);
module_exit(hello_exit);
