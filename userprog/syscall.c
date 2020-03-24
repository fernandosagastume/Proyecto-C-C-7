#include "userprog/syscall.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "devices/shutdown.h"
#include "threads/synch.h"
#include "filesys/filesys.h"

static void syscall_handler (struct intr_frame *);

//---------------------------------------------------------

//Se crea una variable tipo lock para el fylesystem
struct lock lockFS;

/*Struct que lleva control de cada file, file descriptor
y la lista de file descriptors del thread actual*/
static struct fileDescriptor
{
	int fileDescriptor;
	struct file* file;
    struct list_elem felem;
};

//----------------------------------------------------------

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
  lock_init(&lockFS);
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  printf ("system call!\n");
  thread_exit ();
}
 void 
 syscall_halt (void){
 	shutdown_power_off();
 } 

void 
syscall_exit (int exit_status UNUSED){
	thread_exit();
}


bool 
syscall_create(const char* file, unsigned initial_size){
	//Si el nombre del archivo es incorrecto, se hace exit
	if(file == NULL){
		syscall_exit(-1);//exit_status not successful
	}
	lock_acquire(&lockFS);
	bool success = filesys_create(file, initial_size);
	lock_release(&lockFS);
	return success;
}

bool 
syscall_remove(const char* file){
	lock_acquire(&lockFS);
	bool success = filesys_remove(file);
	lock_release(&lockFS);
	return success;
}

int 
syscall_open(const char* file){
	lock_acquire(&lockFS);
	int fileDescriptor;
	struct file* file_ = filesys_open(file);
	if(file_ == NULL){
		lock_release(&lockFS);
		//The file could not be opened, file descriptor -> -1
		fileDescriptor = -1;
		return fileDescriptor;
	}

	int fdSize = sizeof(struct fileDescriptor);
	struct fileDescriptor* fd = malloc(fdSize);
	struct thread* curr = thread_current();
	//Se guarda el archivo que se intenta abrir
	fd->file = file_;
	//El fileDescriptor del thread actual
	fileDescriptor = curr->fdSZ;
	//Se incrementa el file descriptor size en caso de el thread abra mas archivos
	curr->fdSZ+=1;
	fd->fileDescriptor = fileDescriptor;
	//Se ingresa el File D. a la lista de file descriptors del thread actual
  	list_push_front(&curr->fdList, &fd->felem);
  	lock_release(&lockFS);
  	free(fd);
	return fileDescriptor;
}