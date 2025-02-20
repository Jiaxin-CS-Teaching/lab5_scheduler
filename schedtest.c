#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

void loop()
{
  int i;
  int j=0;
  for(i=0; i<1000000; i++)
    j=j+1;
}

void forktest(void)
{
  int pid1, pid2;
  int fds1[2], fds2[2];

  pipe(fds1);
  pipe(fds2);

  pid1 = fork();
  if(pid1 < 0)
    return;

  if(pid1 == 0) { // Process A
    int i;
    char buf[256];
    close(fds1[1]);
    read(fds1[0], buf, 1);
    printf(1, "\nStart process A [%d]\n", getpid());

    for (i=0; i<10; i++) {
      loop();
    }

    // Change priority and trigger rescheduling
    set_priority(getpid(), 0);
    sleep(1);  // Allow scheduler to pick the next process

    for (i=0; i<25; i++) {
      loop();
    }
    exit();
  } else {
    pid2 = fork();
    if(pid2 < 0)
      return;

    if(pid2 == 0) { // Process B
      int i;
      char buf[256];
      close(fds1[0]);
      close(fds2[1]);
      read(fds2[0], buf, 1);
      write(fds1[1], "Done", 5);
      printf(1, "\nStart process B [%d]\n", getpid());

      for (i=0; i<10; i++) {
        loop();
      }

      set_priority(getpid(), 1);
      sleep(1);  // Allow scheduler to pick the next process

      for (i=0; i<25; i++) {
        loop();
      }
      exit();
    } else { // Process C
      int i;
      close(fds1[0]);
      close(fds1[1]);
      close(fds2[0]);
      write(fds2[1], "Done", 5);
      printf(1, "\nStart process C [%d]\n", getpid());

      for (i=0; i<10; i++) {
        loop();
      }

      set_priority(getpid(), 2);
      sleep(1);  // Allow scheduler to pick the next process

      for (i=0; i<25; i++) {
        loop();
      }

      wait();
      wait();
      exit();
    }
  }
}

int main(void)
{
  forktest();
  exit();
}
