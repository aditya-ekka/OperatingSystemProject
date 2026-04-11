#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  // Create a semaphore initialized to 0
  int sem_id = sem_create(0);
  if(sem_id < 0) {
    printf("Failed to create semaphore\n");
    exit(1);
  }

  printf("Starting Semaphore Test...\n");
  int pid = fork();

  if (pid == 0) {
    // --- CHILD PROCESS ---
    
    printf("Child: I am doing some heavy work...\n");
    // Simulate time passing by keeping the CPU busy
    for (volatile int i = 0; i < 10000000; i++);
    printf("Child: Work finished. Sending signal to parent!\n");
    
    sem_up(sem_id); // Increment the semaphore (wakes up parent)
    exit(0);
  } else {
    // --- PARENT PROCESS ---
    printf("Parent: Waiting for child to finish.\n");
    
    sem_down(sem_id); // Value is 0, so parent goes to sleep here!
    
    printf("Parent: I received the signal. Child is done. Exiting.\n");
    wait(0);
    exit(0);
  }
}