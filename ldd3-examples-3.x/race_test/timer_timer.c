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
2. if define WANT_CRASH, you can see in the end g is not 2000.
   but if not define WANT_CRASH, in the end g is always 2000.
   don't know why the list doesn't crash anyway.
3. timer works like tasklet, it binds to cpu that calls add_timer()
*/

MODULE_LICENSE("Dual BSD/GPL");

struct fox {
	unsigned long tail_length; /* length in centimeters of tail */
	struct list_head list; /* list of all fox structures */
};

struct timer_list my_timer;
struct timer_list my_timer2;
static LIST_HEAD(fox_list);
DEFINE_SPINLOCK(my_lock);
static struct task_struct *poll_thread1;

//#define WANT_CRASH
int g1, g2, g3, g;

#define TIMES 1000
void my_timer_func(unsigned long data)
{
	struct fox *pos, *next;
#ifndef WANT_CRASH	
	spin_lock(&my_lock);
#endif	
	list_for_each_entry_safe(pos, next, &fox_list, list) {
		udelay(1);
		list_del(&pos->list);
		g3++;
		kfree(pos);
	}
	udelay(2);
	++g;
#ifndef WANT_CRASH	
	spin_unlock(&my_lock);
#endif	


	if (++g1 < TIMES)
		mod_timer(&my_timer, jiffies+1);
	else
		printk("dz timer end, %d\n", g);
	printk_once("timer %x\n", smp_processor_id());

}

void my_timer_func2(unsigned long data)
{
	struct fox * n;
#ifndef WANT_CRASH		
	spin_lock(&my_lock);
#endif		
	n = kmalloc(sizeof(*n), GFP_KERNEL);
	if (n)
		list_add(&n->list, &fox_list);
	udelay(1);
	g++;
#ifndef WANT_CRASH		
	spin_unlock(&my_lock);
#endif		


	if (++g2 < TIMES)
		mod_timer(&my_timer2, jiffies+1);
	else
		printk("dz timer2 end %d\n", g);
	printk_once("timer2 %x\n", smp_processor_id());
}
static int poll_thrd(void *data)
{
	if (data == (void *)1)
		add_timer(&my_timer);
	else
		add_timer(&my_timer2);
	return 0;
}


static int hello_init(void)
{
	init_timer(&my_timer);
	init_timer(&my_timer2);

	my_timer.expires = jiffies+HZ;
	my_timer.data = 0;
	my_timer.function = my_timer_func;
	
	my_timer2.expires = jiffies+HZ;
	my_timer2.data = 0;
	my_timer2.function = my_timer_func2;

	poll_thread1 = kthread_create(poll_thrd, (void *)1, "thread1");
	if (likely(!IS_ERR(poll_thread1))) {
		kthread_bind(poll_thread1, 0);
		wake_up_process(poll_thread1);
	}

	poll_thread1 = kthread_create(poll_thrd, (void *)2, "thread1");
	if (likely(!IS_ERR(poll_thread1))) {
		kthread_bind(poll_thread1, 1);
		wake_up_process(poll_thread1);
	}
	
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Goodbye, cruel world\n");
	del_timer(&my_timer);
	del_timer(&my_timer2);
}



module_init(hello_init);
module_exit(hello_exit);
