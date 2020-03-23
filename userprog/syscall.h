#include <stdbool.h>
#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);
void syscall_halt (void); 
void syscall_exit (int exit_status);
bool syscall_create(const char* file, unsigned initial_size);
bool syscall_remove(const char* file);
#endif /* userprog/syscall.h */
