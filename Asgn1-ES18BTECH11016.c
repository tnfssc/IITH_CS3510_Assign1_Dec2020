#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  const int SIZE = 2048;
  if (argc < 2) // If there are too few arguments, show error
  {
    printf("Invalid arguments! Look at README for instructions.\n");
    _exit(1);
  }
  const char *shared_mem_name = "shm"; 
  int fd = shm_open(shared_mem_name, O_CREAT | O_RDWR, 0666); // Open shared memory segment
  ftruncate(fd, SIZE);
  void *ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, fd, 0); // Mapping `fd` to pointer
  struct timeval start, end;

  pid_t pid = fork(); // Clone the calling process, creating an exact copy and return PID of the new process to the old process
  if (pid == 0) // Child process
  {
    gettimeofday(&start, NULL); // Write current time to `start`
    sprintf(ptr, "%ld", start.tv_usec); // Storing `start` epoch time as string in `ptr` which maps to the shared memory `fd`
    execvp(argv[1], &argv[1]); // Executing given command with given arguments
  }
  else if (pid > 0) // Parent process
  {
    wait(NULL); // Wait till child processes exit
    gettimeofday(&end, NULL); // Write current time to `end`
    suseconds_t start_time = atoi((char *)ptr); // Converting `start` epoch time stored as string by child process to `long int`
    printf("Elapsed time: %ld\n", end.tv_usec - start_time);
  }
  else // PID < 0 => Child process wasn't created
  {
    printf("Some error occured!");
    _exit(1);
  }
  shm_unlink(shared_mem_name); // Remove shared memory segment
}
