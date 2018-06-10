#include "sync.h"
#include "idt.h"
#include "debug.h"

void sema_init(semaphore *psema, uint8_t value)
{
    psema->value = value;
    list_init(&psema->waiters);
}

void lock_init(lock *plock) {
    plock->holder = NULL;
    plock->holder_repeat_nr = 0;
    sema_init(&plock->semaphore, 1);
}

void sema_down(semaphore *psema) {
    enum intr_status old_status = intr_disable();

    while(psema->value == 0) {
        ASSERT(!elem_find(&psema->waiters, &running_thread()->general_tag));
        list_append(&psema->waiters, &running_thread()->general_tag);
        thread_block(TASK_BLOCKED);
    }

    psema->value--;
    ASSERT(psema->value == 0);
    intr_set_status(old_status);
}

void sema_up(semaphore *psema) {
    enum intr_status old_status = intr_disable();

    ASSERT(psema->value == 0);
    if(!list_empty(&psema->waiters)) {
        list_elem *thread_tag = list_pop(&psema->waiters);
        task_struct *thread_blocked = (task_struct *)(0xfffff000 & (uint32_t)thread_tag);
        thread_unblock(thread_blocked);
    }

    psema->value++;
    ASSERT(psema->value == 1);
    intr_set_status(old_status);
}

void lock_acquire(lock *plock) {
    if(plock->holder != running_thread()) {
        sema_down(&plock->semaphore);
        plock->holder = running_thread();
        plock->holder_repeat_nr = 1;
    } else {
        plock->holder_repeat_nr++;
    }
}

void lock_release(lock *plock) {
    ASSERT(plock->holder == running_thread());
    if(plock->holder_repeat_nr > 1) {
        plock->holder_repeat_nr--;
        return;
    }

    ASSERT(plock->holder_repeat_nr == 1);
    plock->holder = NULL;
    plock->holder_repeat_nr = 0;
    sema_up(&plock->semaphore);
}
