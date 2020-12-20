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
  if (argc < 2)
  {
    printf("Invalid arguments! Look at README for instructions.\n");
    _exit(1);
  }
  const char *shared_mem_name = "shm";
  int fd = shm_open(shared_mem_name, O_CREAT | O_RDWR, 0666);
  ftruncate(fd, SIZE);
  void *ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
  struct timeval start, end;

  pid_t pid = fork();
  if (pid == 0)
  {
    gettimeofday(&start, NULL);
    sprintf(ptr, "%ld", start.tv_usec);
    execvp(argv[1], &argv[2]);
  }
  else if (pid > 0)
  {
    wait(NULL);
    gettimeofday(&end, NULL);
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
    suseconds_t start_time = atoi((char *)ptr);
    printf("Elapsed time: %ld\n", end.tv_usec - start_time);
  }
  else
  {
    printf("Some error occured!");
    _exit(1);
  }
  shm_unlink(shared_mem_name);
}
