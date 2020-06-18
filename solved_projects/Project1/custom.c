#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <wait.h>
#include <sys/time.h>
#include <sys/resource.h>

#define BUFSIZE 32

extern int errno;

int main(void){
  struct timeval start, end;
  struct rusage buf; 
  double elapsedTime;
  char cmd[BUFSIZE];
  int err;
  FILE* filePointer;
  int bufferLength = 255;
  char currCmd[bufferLength];
  filePointer = fopen("custom.txt", "r");
  while(fgets(currCmd, bufferLength, filePointer)) {
      currCmd[strcspn(currCmd,"\n")] = 0;
      gettimeofday(&start, NULL);
      getrusage(RUSAGE_SELF, &buf);
      long prevMax = buf.ru_majflt, prevMin = buf.ru_minflt;
      printf("Running command: %s \n", currCmd);
  char* argv[32] = { strtok(currCmd, " ") };
  for(int i = 1; i < 32; i++) {
      argv[i] = strtok(NULL, " ");
  }
  argv[31] = NULL;
  if(strcmp(argv[0], "ccd") == 0) {
      chdir(argv[1]);
      printf("Changed to directory: %s \n \n", argv[1]);
      fflush(stdout);
    } else{
        int n = fork();
        if(n == 0) {
          if(strcmp(argv[0], "cpwd") == 0) {
            strcpy(argv[0],&argv[0][1]);
            printf("Current directory: ");
            fflush(stdout);
          }
          if(strcmp(argv[0], "ccd") != 0 && execvp(argv[0], argv) == -1){
            err = errno;
            printf("Ah! execl (%s) failed with error: %s\n", currCmd, strerror(err));
          }
        }
        else {
          wait(NULL);
          gettimeofday(&end, NULL);
          elapsedTime = (end.tv_usec - start.tv_usec)*0.001;
          getrusage(RUSAGE_SELF, &buf); 
          if(strcmp(currCmd, "cpwd") != 0 && strcmp(currCmd, "ccd") != 0) {
            printf("\n-- Statistics -- \n");
            printf("Elapsed time: %d milliseconds \n", (int) elapsedTime);
            printf("Page Faults: %ld \n", buf.ru_majflt - prevMax);
            printf("Page Faults (reclaimed): %ld \n", buf.ru_minflt - prevMin);
            printf("-- End of Statistics -- \n \n");
          } else {
            printf("\n");
          }
        }
	}
  }
  fclose(filePointer);
}
