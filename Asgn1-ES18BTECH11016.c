#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/shm.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
  if (argc < 2 || argc > 2)
  {
    printf("Invalid arguments! Look at README for instructions.\n");
    _exit(1);
  }
  // int fd = shm_open("memory", O_CREAT | O_RDWR, 0666);
  pid_t pid = fork();
  if (pid == 0)
  {
    execlp(argv[1], "", NULL);
    printf("child~");
  }
  else if (pid > 0)
  {
    wait(NULL);
    printf("parent~");
  }
  else
  {
    printf("Some error occured!");
    _exit(1);
  }
  printf("\nover\n");
}
