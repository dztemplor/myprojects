


#include "common.h"

int main()
{
     TAILQ_HEAD(tailhead, entry) head;

     struct entry {
           TAILQ_ENTRY(entry) entries;         /* Tail queue. */
          int i;
       } *n1, *n2, *np;
     

     TAILQ_INIT(&head);                      /* Initialize the queue. */

       n1 = malloc(sizeof(struct entry));      /* Insert at the head. */
       n1->i = 1;
       TAILQ_INSERT_HEAD(&head, n1, entries);

       n1 = malloc(sizeof(struct entry));      /* Insert at the tail. */
       n1->i = 2;
       TAILQ_INSERT_TAIL(&head, n1, entries);

       n2 = malloc(sizeof(struct entry));      /* Insert after. */
       n2->i = 3;
       TAILQ_INSERT_AFTER(&head, n1, n2, entries);
     
       TAILQ_REMOVE(&head, n2, entries);

       for (np = head.tqh_first; np != NULL; np = np->entries.tqe_next)
            printf("%d\n", np->i);

 
       
      
       
}

