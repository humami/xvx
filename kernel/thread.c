#include "thread.h"
#include "memory.h"
#include "string.h"
#include "idt.h"
#include "debug.h"

#define PG_SIZE 4096

task_struct *main_thread;
list thread_ready_list;
list thread_all_list;
static list_elem *thread_tag;

extern void switch_to(task_struct *cur, task_struct *next);

task_struct *running_thread() {
    uint32_t esp;
    asm volatile ("mov %%esp, %0" : "=g"(esp));
    return (task_struct *)(esp & 0xfffff000);
}

static void kernel_thread(thread_func *function, void *func_args) {
	intr_enable();
    function(func_args);
}

void thread_create(task_struct *pthread, thread_func function, void *func_args) {
	pthread->self_kstack -= sizeof(intr_stack);

	pthread->self_kstack -= sizeof(thread_stack);
	thread_stack *kthread_stack = (thread_stack *)pthread->self_kstack;
	kthread_stack->eip = kernel_thread;
	kthread_stack->function = function;
	kthread_stack->func_args = func_args;
	kthread_stack->ebp = kthread_stack->ebx = kthread_stack->edi = kthread_stack->esi = 0;
}

void init_thread(task_struct *pthread, char *name, int prio) {
    memset(pthread, 0, sizeof(*pthread));
    strcpy(pthread->name, name);

    if(pthread == main_thread)
        pthread->status = TASK_RUNNING;
    else
        pthread->status = TASK_READY;

    pthread->self_kstack = (uint32_t *)((uint32_t)pthread + PG_SIZE);
    pthread->priority = prio;
    pthread->ticks = prio;
    pthread->elapsed_ticks = 0;
    pthread->pgdir = NULL;
    pthread->stack_magic = 0x19950000;
}

task_struct *thread_start(char *name, int prio, thread_func function, void *func_args) {
    task_struct *thread = get_kernel_pages(1);

    init_thread(thread, name, prio);
    thread_create(thread, function, func_args);
   
    ASSERT(!elem_find(&thread_ready_list, &thread->general_tag));
    list_append(&thread_ready_list, &thread->general_tag);

    ASSERT(!elem_find(&thread_all_list, &thread->all_list_tag));
    list_append(&thread_all_list, &thread->all_list_tag);

    return thread;
}

static void make_main_thread() {
    main_thread = running_thread();
    init_thread(main_thread, "main", 10);

    ASSERT(!elem_find(&thread_all_list, &main_thread->all_list_tag));
    list_append(&thread_all_list, &main_thread->all_list_tag);
}

void schedule() {
    task_struct *cur_thread = running_thread();

    if(cur_thread->status == TASK_RUNNING) {
        ASSERT(!elem_find(&thread_ready_list, &cur_thread->general_tag));
        list_append(&thread_ready_list, &cur_thread->general_tag);
        cur_thread->ticks = cur_thread->priority;
        cur_thread->status = TASK_READY; 
    } else {

    }
    
    ASSERT(!list_empty(&thread_ready_list));
    thread_tag = NULL;
    thread_tag = list_pop(&thread_ready_list);
    task_struct *next_thread = (task_struct *)(0xfffff000 & (uint32_t)thread_tag);
    next_thread->status = TASK_RUNNING;
    switch_to(cur_thread, next_thread);
}

void thread_init() {
    list_init(&thread_all_list);
    list_init(&thread_ready_list);
    make_main_thread();
}
