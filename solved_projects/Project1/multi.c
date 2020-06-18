#include <stdio.h>

#include <unistd.h>

#include <string.h>

#include <errno.h>

#include <wait.h>

#include <sys/time.h>

#include <sys/resource.h>

#include <stdlib.h>

#include <stdbool.h>

#define BUFSIZE 32

extern int errno;
struct entry {
  int processId;
  char name[255];
};

int main(int argc, char ** args) {
  int backgroundLines[argc-1];
  for (int i = 1; i < argc; i++) {
  	backgroundLines[i-1] = atoi(args[i]);
  }
  for (int i = 0; i < argc-1; ++i){
 	for (int j = i + 1; j < argc-1; ++j){
 		if (backgroundLines[i] > backgroundLines[j]){
                    int a =  backgroundLines[i];
                    backgroundLines[i] = backgroundLines[j];
                    backgroundLines[j] = a;
                }
        }
  }    
  struct timeval start, end;
  struct rusage buf;
  double elapsedTime;
  int err;
  
  FILE * filePointer;
  int bufferLength = 255;
  char currCmd[bufferLength];
  filePointer = fopen("multi.txt", "r");
  
  int line = 0;
  int argi = 0;
  
  struct entry map[argc];
  
  bool isBackground = false;
  bool wasPrevBackground = false;
  
  char BackgroundName[255]; 
  
  for (int i = 0; i < argc; i++) {
  	map[i].processId = 0;
  	strcpy(map[i].name, "");
  }
  
  long prevMin = 0, prevMax = 0;
  
  while (fgets(currCmd, bufferLength, filePointer)) {
    line++;
    wasPrevBackground = isBackground;
    if(argi < argc && line == backgroundLines[argi]){
    	isBackground = true;
    } else {
    	isBackground = false;
    }
    currCmd[strcspn(currCmd, "\n")] = 0;
    
    gettimeofday( &start, NULL);
    
    printf("Running command: %s \n", currCmd);
    char name[255];
    strcpy(name, currCmd);
    char * argv[32] = { strtok(currCmd, " ") };
    
    for (int i = 1; i < 32; i++) {
      argv[i] = strtok(NULL, " ");
    }
    
    if (strcmp(argv[0], "ccd") == 0) {
      chdir(argv[1]);
      printf("Changed to directory: %s \n", argv[1]);
      fflush(stdout);
    } else if (strcmp(argv[0], "cproclist") == 0) {
          printf("-- Background Processes -- \n");
          for (int i = 0; i < argc; i++) {
          	if(map[i].processId > 0){
          		printf("[%d] %s \n", i, map[i].name);
          	}
          }
    } else {
      int n = fork();
      
      getrusage(RUSAGE_SELF, &buf);
      prevMax = buf.ru_majflt, prevMin = buf.ru_minflt;
      
      int waitResponse = wait3(NULL, WNOHANG, &buf);
      elapsedTime = (end.tv_usec - start.tv_usec) * 0.001;
      if(waitResponse > 0){
      	for (int i = 0; i < argc; i++) {
      	    if(map[i].processId == waitResponse){
      	            getrusage(RUSAGE_SELF, &buf);
	      	    printf("\n -- Job Complete [%d: %s] -- \n", i, map[i].name);
	      	    printf("Process ID: %d \n", map[i].processId);
	      	    printf("\n-- Statistics -- \n");
	      	    printf("Elapsed time: %d milliseconds \n", (int) elapsedTime);
		    printf("Page Faults: %ld \n", buf.ru_majflt - prevMax);
		    printf("Page Faults (reclaimed): %ld \n", buf.ru_minflt - prevMin);
		    printf("-- End of Statistics -- \n");
		    prevMax += buf.ru_majflt;
	      	    prevMin += buf.ru_minflt;
		    map[i].processId = 0;
		    break;
	    }
      	}
      }
      if (n == 0) {
        if (strcmp(argv[0], "cpwd") == 0) {
          strcpy(argv[0], &argv[0][1]);
          printf("Current directory: ");
          fflush(stdout);
        }
        if (strcmp(argv[0], "ccd") != 0 && execvp(argv[0], argv) == -1) {
          exit(errno);
        }
      } else {
          if (isBackground) {
		  printf("Background: ID [%d]: %s\n", argi,name);
		  struct entry e;
          	  strcpy(e.name, name);
	       	  e.processId = n;
		  map[argi] = e;
	  } else {
	  	  getrusage(RUSAGE_SELF, &buf);
	          prevMax = buf.ru_majflt, prevMin = buf.ru_minflt;
          	  wait4(n, NULL, 0, &buf);
          	  getrusage(RUSAGE_SELF, &buf);
		  gettimeofday( & end, NULL);
		  elapsedTime = (end.tv_usec - start.tv_usec) * 0.001;
		  getrusage(RUSAGE_SELF, & buf);
		  if (!isBackground && strcmp(currCmd, "cpwd") != 0 && strcmp(currCmd, "ccd") != 0) {
		    printf("\n-- Statistics -- \n");
		    printf("Elapsed time: %d milliseconds \n", (int) elapsedTime);
		    printf("Page Faults: %ld \n", buf.ru_majflt - prevMax);
		    printf("Page Faults (reclaimed): %ld \n", buf.ru_minflt - prevMin);
		    printf("-- End of Statistics -- \n");
		    prevMax += buf.ru_majflt;
	      	    prevMin += buf.ru_minflt;
		  }
	}
      }
    }
    if(isBackground){
    	argi++;
    }
    printf("\n");
  }
  while(true){
        int pid = wait3(NULL, 0, &buf);
        if(pid <= 0) {
        	break;
        }
  	for (int i = 0; i < argc; i++) {
	      	if(pid == map[i].processId){
	      		 getrusage(RUSAGE_SELF, &buf);
	      		 prevMax = buf.ru_majflt, prevMin = buf.ru_minflt;
			 wait4(map[i].processId, NULL, 0, &buf);
			 getrusage(RUSAGE_SELF, &buf);
			 printf("-- Job Complete [%d: %s] -- \n", i, map[i].name);
			 printf("Process ID: %d \n", map[i].processId);
			 printf("\n-- Statistics -- \n");
		      	 printf("Elapsed time: %d milliseconds \n", (int) elapsedTime);
			 printf("Page Faults: %ld \n", buf.ru_majflt - prevMax);
		    	 printf("Page Faults (reclaimed): %ld \n", buf.ru_minflt - prevMin);
		    	 printf("-- End of Statistics -- \n");
		    	 prevMax += buf.ru_majflt;
	      	   	 prevMin += buf.ru_minflt;
			 break;
		}
	  }	
  }
  fclose(filePointer);
}
