#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);
void syscall_halt (void); 
void syscall_exit (int exit_status);
#endif /* userprog/syscall.h */
