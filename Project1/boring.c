#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <wait.h>
#include <sys/time.h>
#include <sys/resource.h>

#define BUFSIZE 32

extern int errno;

int main(void) {
  struct timeval start, end;
  struct rusage buf;
  double elapsedTime;
  char cmd[BUFSIZE];
  int err;

  char arrOfCmd[3][BUFSIZE] = {
    "whoami",
    "last",
    "ls -al /home"
  };
  char * argv[] = {
    "bin/ls",
    "-al",
    "/home",
    0
  };
  char currCmd[BUFSIZE];
  for (int i = 0; i < 3; i++) {
    gettimeofday( &start, NULL);
    getrusage(RUSAGE_SELF, &buf);
    long prevMax = buf.ru_majflt, prevMin = buf.ru_minflt;
    strcpy(currCmd, arrOfCmd[i]);
    char path[BUFSIZE] = "/usr/bin/";
    strcat(path, currCmd);
    printf("Running command: %s \n", currCmd);
    if (fork() == 0) {
      if ((i == 2 && execv("/bin/ls", argv) == -1) || execl(path, path, NULL) == -1) {
        err = errno;
        printf("Ah! execl (%s) failed with error: %s\n", currCmd, strerror(err));
      }
    } else {
      wait(NULL);
      gettimeofday( &end, NULL);
      elapsedTime = (end.tv_usec - start.tv_usec) * 0.001;
      getrusage(RUSAGE_SELF, & buf);
      printf("\n-- Statistics -- \n");
      printf("Elapsed time: %d milliseconds \n", (int) elapsedTime);
      printf("Page Faults: %ld \n", buf.ru_majflt - prevMax);
      printf("Page Faults (reclaimed): %ld \n", buf.ru_minflt - prevMin);
      printf("-- End of Statistics -- \n \n");

    }
  }
  
  return 0;
}
