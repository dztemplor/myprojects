/*                                                     
 * $Id: hello.c,v 1.5 2004/10/26 03:32:21 corbet Exp $ 
 */                                                    
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/mutex.h>
#include <linux/gfp.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/highmem.h>

/*
this examples demos memory and page usage.
testing this example on arm64 and i386-32 has different results.
i386:
    alloc_page(__GFP_HIGHMEM), page_address() is null.

page_address(): works on normal memory and pkmap memory
kmap_to_page() and kmap_atomic_to_page(), changes virt address to page
alloc_page(__GFP_HIGHMEM): not always return high memory, can retry with PageHighMem() to ensure get high memory


*/

MODULE_LICENSE("Dual BSD/GPL");
static int test1(void)
{
	struct page *page;
	unsigned long mem;
	void *km, *vm;

	printk("test1 ==============\n");
	printk("page 0  %p\n", pfn_to_page(0));
	printk("vmalloc_start %lx\n", VMALLOC_START);
	page = alloc_page(GFP_KERNEL);
	printk("%p %p %p %ld\n", 	page, page_address(page),
		   virt_to_page(page_address(page)), page_to_pfn(page));
	printk("pa %lx\n", __pa(page_address(page)));
	__free_pages(page, 0);

	page = alloc_page(__GFP_HIGHMEM);
	printk("valid %d\n",  virt_addr_valid(page_address(page)));
	printk("%p %p %p %ld\n", 	page, page_address(page),
		   virt_to_page(page_address(page)), page_to_pfn(page));
	__free_pages(page, 0);
	page = alloc_page(__GFP_DMA);
	printk("valid %d\n",  virt_addr_valid(page_address(page)));
	printk("%p %p %p %ld\n", 	page, page_address(page),
		   virt_to_page(page_address(page)), page_to_pfn(page));
	__free_pages(page, 0);


	mem= __get_free_page(__GFP_HIGHMEM);
	printk("valid %d\n",  virt_addr_valid(mem));
	printk("%lx %p \n", mem, virt_to_page(mem));
	free_page(mem);

	km = kmalloc(100, GFP_KERNEL);
	printk("valid %d\n",  virt_addr_valid(km));
	printk("km %p %p\n", km, virt_to_page(km));
	if (km)
		kfree(km);

	vm = vmalloc(100);
	printk("valid %d %p\n",  virt_addr_valid(vm), high_memory);
	printk("vm %p %p %p\n", vm, virt_to_page(vm), page_address(virt_to_page(vm)));//should not ues virt_to_page()
	if (vm)
		vfree(vm);
	
	return 0;
}

void test2(void)
{
	struct page *page;
	void *mem2;
	int i ;
	pte_t pte;

	printk("test2 ==============\n");
	for (i=0; i<10000; i++) {
		page = alloc_page(__GFP_HIGHMEM);
		if (PageHighMem(page)) {
			printk("found high %d\n", i);
			break;
		}
		__free_page(page);
	}
	mem2= kmap(page);
	printk("%p %p %lx %p %p \n", 	page, mem2, __pa(mem2),
		   kmap_to_page(mem2),
		   page_address(page));
	pte = mk_pte(page, PAGE_KERNEL);
	printk("physical address %lx\n", (unsigned long)pte_val(pte));
	
	kunmap(page);

	
	mem2= kmap_atomic(page);
	printk("%p %p %lx %lx %p\n", 	page, mem2, __pa(mem2),
		   virt_to_fix((unsigned long)mem2),
		   kmap_atomic_to_page(mem2));
	kunmap_atomic(mem2);

	__free_page(page);
}

static int hello_init(void)
{
	test1();
	test2();
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Goodbye, cruel world\n");
	//kthread_stop(poll_thread1);
	//kthread_stop(poll_thread2);
}


module_init(hello_init);
module_exit(hello_exit);
