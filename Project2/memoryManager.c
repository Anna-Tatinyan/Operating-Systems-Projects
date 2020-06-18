#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 64

unsigned char memory[SIZE]; 
int freeList[4];
int fileLineCount = 0;
int registers[4];
int SwapedRegister[4];

void revivePage(int to, int PTEAddress){
	int PTE = memory[PTEAddress];
	memory[PTEAddress] = (to/16 << 2) | 2 | (PTE & 1);
	PTE = PTE >> 2;
	printf("Swapped disk slot %d into frame %d\n", PTE, to/16);
	FILE * filereader = fopen("swapSpace.txt", "r");
	int count = 0;
	int j = 0;
	if (filereader!= NULL){
		char line[256];
		while (fgets(line, sizeof line, filereader) != NULL){
		    if (count >= PTE*16 && count < PTE*16 + 16){		
		      line[strcspn(line,"\n")] = 0;
		      memory[to + j] = (unsigned char) atoi(line);
		      j++;
		    }
		    count++;
		}
		fclose(filereader);
	}
}

void revivePageTable(int to, int processID){
	int PTE = registers[processID];
	registers[processID] = to/16;
	printf("Swapped disk slot %d into frame %d\n", PTE, to/16);
	FILE * filereader = fopen("swapSpace.txt", "r");
	int count = 0;
	int j = 0;
	if (filereader!= NULL){
		char line[256];
		while (fgets(line, sizeof line, filereader) != NULL){
		    if (count >= PTE*16 && count < PTE*16 + 16){		
		      line[strcspn(line,"\n")] = 0;
		      memory[to + j] = (unsigned char) atoi(line);
		      j++;
		    }
		    count++;
		}
		fclose(filereader);
	}
}

int* swap(int pageTableIndex, int pageIndex, int value, int isPage) {
  int* result = (int *) malloc(sizeof(int) * 2);
  int swapNeed = 1;
  if(pageTableIndex != -1){
    swapNeed = 0;
  }
  FILE * filePointer = fopen("swapSpace.txt", "a+");
  if (filePointer == NULL) {
      printf("The swap space does not exist!");   
      exit(1);             
  }
  for(int i = 0; i<4; i++) {
    if(freeList[i] == 1) {
      for(int k = 0; ; k++) {
        if(swapNeed < 0 || k > 15){
            break;
        }
        unsigned char pageTablePointer = memory[i*16 + k];
        if((pageTablePointer & 2) == 2){
          pageTablePointer = 16 * (pageTablePointer >> 2);
          if(pageTableIndex == -1){
            printf("Swapped frame %d to disk at swap slot %d \n", pageTablePointer/16, fileLineCount);
            pageTableIndex = pageTablePointer/16;
            freeList[pageTableIndex] = 1;
          } else {
            printf("Swapped frame %d to disk at swap slot %d \n", pageTablePointer/16, fileLineCount);
            pageIndex = pageTablePointer/16;
            freeList[pageIndex] = 0;
          }
          unsigned char protectionBit = memory[i*16 + k] & 1;
          memory[i*16 + k] =  (fileLineCount << 2) | protectionBit;
          
          for(int j = 0; j<16; j++) {
            unsigned char startOfPage = memory[pageTablePointer + j];
            fprintf(filePointer,"%d\n",startOfPage);
            memory[pageTablePointer + j] = 0;
          }
          if(isPage == 1){
              revivePage(pageTablePointer, value);
          } else if(isPage == 0){
              revivePageTable(pageTablePointer, value);
          }
          fileLineCount++;
          swapNeed--;
        }
        
      }            
    }
  }
  if(swapNeed >= 0){
    for(int i = 0; i<4; i++) {
    if(swapNeed < 0) break;
    if(freeList[i] == 1) {
      if(pageTableIndex == -1){
          pageTableIndex = i;
          freeList[pageTableIndex] = 1;
      } else {
          pageIndex = i;
          freeList[pageIndex] = 0;
      }
      for(int k = 0; k<16; k++) {
        unsigned char pageTablePointer = memory[i*16 + k];
        memory[i*16 + k] = 0;
      }
      if(isPage == 1){
              revivePage(pageIndex, value);
      } else if(isPage == 0){
              revivePageTable(pageIndex, value);
      }
      for(int m = 0; m<4; m++) {
          if(registers[m] == i) {
            registers[m] = fileLineCount;
            SwapedRegister[m] = 1;
            break;
        }
      }
      fileLineCount++;
      swapNeed--;
      }            
    }
  }
  fclose(filePointer);
  result[0] = pageTableIndex;
  result[1] = pageIndex;
  return result;
}

int main(void) {
  fclose(fopen("swapSpace.txt", "w"));
  for(int i = 0; i < SIZE; i++){
    memory[i] = 0;
  }
  for(int i = 0; i < 4; i++){
    registers[i] = -1;
  }
  for(int i = 0; i < 4; i++){
    freeList[i] = -1;
  }
  int PagePerProcess[4];
  for(int i = 0; i < 4; i++){
    SwapedRegister[i] = 0;
  }
  for(int i = 0; i < 4; i++){
    PagePerProcess[i] = 0;
  }
  int process_id;
  char * instruction_type;
  int virtual_address;
  int value;
  char input[255];
  while(1){
    printf("Instruction? ");
    fflush(stdout);
    int result = scanf("%s", input);
    if (result == EOF){
    	printf("End of file\n");
	break;
    }
    process_id = atoi(strtok(input, ","));
    instruction_type = strtok(NULL, ",");
    virtual_address = atoi(strtok(NULL, ","));
    value = atoi(strtok(NULL, ","));
    if(strcmp(instruction_type, "map") == 0){
      if(PagePerProcess[process_id] == 16) {
        printf("Error: Sorry, no more pages allowed \n");
        break;
      }
      int pageTableIndex = -1;
      int pageIndex = -1;
      for(int i = 0; i < 4; i++){
        if(freeList[i] == -1){
          if(pageTableIndex == -1){ pageTableIndex = i; }
          else{ 
            pageIndex = i;
            break;
          }
        }
      }
      if(registers[process_id] != -1){
          pageIndex = pageTableIndex;
          pageTableIndex = registers[process_id];
      }
      int VPN = (virtual_address >> 4);
      int FindPTE = memory[16*pageTableIndex + VPN];
      if (pageTableIndex != -1 && FindPTE != 0) {
        if((FindPTE & 1) != value){
           memory[16*pageTableIndex + VPN] = (FindPTE & 254) | value;
           printf("Updating permissions for virtual page %d (frame %d)", VPN, memory[16*pageTableIndex + VPN] >> 2);
        } else {
          printf("Error: virtual page %d is already mapped with rw_bit=%d \n", VPN, memory[16*pageTableIndex + VPN] & 1);
        }
      } else {
        if(pageTableIndex == -1 || pageIndex == -1) {
          int* swapResult = swap(pageTableIndex, pageIndex, process_id, -1);
          pageTableIndex = swapResult[0];
          pageIndex = swapResult[1];
        }
        freeList[pageTableIndex] = 1;
        freeList[pageIndex] = 0;
        unsigned char PTE = pageIndex;
        PTE = (PTE << 2) | value | 2;
        if(registers[process_id] == -1){
          printf("Put page table for PID %d into physical frame %d \n", process_id, pageTableIndex);
          registers[process_id] = pageTableIndex;
        }
        memory[16*pageTableIndex + VPN] = PTE;
        ++PagePerProcess[process_id];
        printf("Mapped virtual address %d (page %d) into physical frame %d\n", virtual_address, VPN, pageIndex);
      }
    } else if(strcmp(instruction_type, "store") == 0){
        int VPN = (virtual_address >> 4);
        if(registers[process_id] == -1){
          printf("Error: I have never heard about this process. \n");
        } else if(memory[16*registers[process_id] + VPN] == 0){
          printf("Error: hmmm, this page doesn't exist! \n");
        } else {
          if(SwapedRegister[process_id] == 1){
          	SwapedRegister[process_id] == 0;
		int* swapResult = swap(-1, 0, process_id, 0);              
          }
          int PTE = memory[16*registers[process_id] + VPN];
          if((PTE & 1) == 0){
            printf("Error: Oh no, you don't have the permission! \n");
          } else {
            if((PTE & 2) == 0){
                int* swapResult = swap(registers[process_id], -1, 16*registers[process_id] + VPN, 1);
                PTE = swapResult[1];
            }
            PTE = memory[16*registers[process_id] + VPN];
            PTE = (PTE >> 2);
            int offset = (virtual_address & 15);
            int physical_location = 16*PTE + offset;
            memory[physical_location] = value;
            printf("Stored value %d at virtual address %d (physical address %d) \n", value, virtual_address, physical_location);
          }
        }
    } else if(strcmp(instruction_type, "load") == 0){
      int VPN = (virtual_address >> 4);
        if(registers[process_id] == -1){
          printf("Error: I have never heard about this process.");
        } else if(memory[16*registers[process_id] + VPN] == 0){
          printf("Error: hmmm, this page doesn't exist! \n");
        } else {
          if(SwapedRegister[process_id] == 1){
          	SwapedRegister[process_id] == 0;
		int* swapResult = swap(-1, 0, process_id, 0);              
          }
          int PTE = memory[16*registers[process_id] + VPN];
          if((PTE & 1) == 0){
            printf("Error: Oh no, you don't have the permission! \n");
          } else {
            if((PTE & 2) == 0){
                int* swapResult = swap(registers[process_id], -1, 16*registers[process_id] + VPN, 1);
                PTE = swapResult[1];
            }
            PTE = memory[16*registers[process_id] + VPN];
            PTE = (PTE >> 2);
            int offset = (virtual_address & 15);
            int physical_location = 16*PTE + offset;
            printf("The value %d is virtual address %d (physical address %d) \n", memory[physical_location], virtual_address, physical_location);
          }
        }
    }
  } 
  return 0;
}
