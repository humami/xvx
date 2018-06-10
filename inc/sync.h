#ifndef SYNC_H
#define SYNC_H

#include "types.h"
#include "list.h"
#include "thread.h"

typedef struct semaphore {
    uint8_t value;
    list waiters;
}semaphore;

//mutex
typedef struct lock {
    task_struct *holder;
    semaphore semaphore;
    uint32_t holder_repeat_nr; 
}lock;

void sema_init(semaphore *psema, uint8_t value);
void lock_init(lock *plock);
void sema_down(semaphore *psema);
void sema_up(semaphore *psema);
void lock_acquire(lock *plock);
void lock_release(lock *plock);

#endif