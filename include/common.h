#ifndef COMMON_H
#define COMMON_H


#include <sys/types.h>          /* See NOTES */
       #include <sys/socket.h>
  #include <sys/types.h>          /* See NOTES */
       #include <sys/socket.h>
     #include <sys/un.h>
 #include <unistd.h>
#include <errno.h>
 #include <stdio.h>
#include <stdlib.h>


 #include <sys/types.h>
     #include <sys/stat.h>
#include <semaphore.h>
//#include <sys/queue.h>
#include "queue.h"

#include <sys/file.h>

       #include <fcntl.h>

       #include <semaphore.h>
 #include <time.h>


#include <sys/ipc.h>
#include <sys/sem.h>


union semun {
               int              val;    /* Value for SETVAL */
               struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
               unsigned short  *array;  /* Array for GETALL, SETALL */
               struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
           };


#endif
