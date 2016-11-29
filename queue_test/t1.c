#include "queue.h"



#include "common.h"

int test1()
{
     TAILQ_HEAD(tailhead, entry) head;

     struct entry {
           TAILQ_ENTRY(entry) entries;         /* Tail queue. */
          int i;
       } *n1, *n2, *np;
     

     TAILQ_INIT(&head);                      /* Initialize the queue. */

     int i ;
     for (i=0; i<5; i++)
     {
          n1 = malloc(sizeof(struct entry));      /* Insert at the head. */
          n1->i = i;
          TAILQ_INSERT_TAIL(&head, n1, entries);
     }

       n2 = malloc(sizeof(struct entry));      /* Insert after. */
       n2->i = 6;
       TAILQ_INSERT_AFTER(&head, n1, n2, entries);
     
       TAILQ_REMOVE(&head, n2, entries);
       free(n2);

       np = NULL;
       TAILQ_FOREACH(np, &head, entries)
            printf("%d\n", np->i);
       
       
       struct entry * tmp = NULL;
       np = NULL;
       TAILQ_FOREACH_FROM_SAFE(np, &head, entries, tmp)
       {
            TAILQ_REMOVE(&head, np, entries);
            free(np);
       }
       return 0;
}

typedef struct {
     TAILQ_HEAD(tailhead2, entry2) h;     
}comp_t;
struct entry2 {
           TAILQ_ENTRY(entry2) link;         /* Tail queue. */
          int i;
};


void test2()
{
     comp_t c;
     struct entry2 *np, *n1;
     
      TAILQ_INIT(&c.h);
      
     int i ;
     for (i=0; i<5; i++)
     {
          n1 = malloc(sizeof(struct entry2));      /* Insert at the head. */
          n1->i = i;
          TAILQ_INSERT_TAIL(&c.h, n1, link);
     }

     struct entry2 * tmp = NULL;
     np = NULL;
     TAILQ_FOREACH_FROM_SAFE(np, &c.h, link, tmp)
     {
          printf("%d\n", np->i);
          TAILQ_REMOVE(&c.h, np, link);
          free(np);
     }

}

int main()
{
     //test1();
     test2();
}


