 # Project 2: Implementing Virtual Memory

The project contains:


		    1. memoryManager.c - includes the code of the first part of the assignment.
		    2. Makefile program that compiles the code.
		    3. Test1.txt - Test file
		    4. Test2.txt - Test file
		    5. Test3.txt - Test file
		    6. outputTest1.txt - output of the instructions from test1
		    7. outputTest2.txt - output of the instructions from test2
		    8. outputTest3.txt - output of the instructions from test3
		    9. swapSpace.txt - file for virtualizing swap space (disk)

Virtual memory is a powerful construction that allows programs to believe they have access to a larger amount of memory resources than is present in the physical RAM on a computer. In this project we will simulate a virtual memory system with paging and swapping.

## Part 1

  
Objective is to create a memory manager that takes instructions from simulated processes and modifies physical memory accordingly. In part 1 of this project, it must implement the basics of paging, including a per-process page table, address translation, and support for multiple processes residing in memory concurrently.

### Memory Implementation.

To keep the implementation simple, simulated memory is represented as an array of bytes, e.g., unsigned char memory[SIZE]). It is the responsibility of the memory manager to logically partition those bytes into pages, perform translations from virtual to physical addresses, load and store values in memory, and update the page tables.

### Input.

Instructions will be supplied to your memory manager via  stdin  and will always be a 4-tuple in the format described below:

  

> `process_id,instruction_type,virtual_address,value`


***The process id*** is an integer value in the range [0, 3] used to simulate instructions from diﬀerent pro-cesses. The instruction type specifies the desired memory operation

***The virtual address*** is an integer value in the range [0,  63] specifying the virtual memory location for given  process. The meaning of value depends on the instruction, but must be an integer value in the range [0, 255].

**Instructions.** The memory manager supports the following instructions:

-   ***map*** tells the memory manager to allocate a physical page, i.e., it creates a mapping in the page table  between a virtual and physical address. The manager must determine the appropriate virtual page number using the virtual address. For example, virtual address of 3 corresponds to virtual page 0. value argument represents the write permission for the page. 
	- If value=1 then the page is writeable and readable. 
	- If value=0, then the page is only readable, i.e., all mapped pages are readable. 
	These permissions can be modified by using a second map instruction for the target page.
-   ***store*** instructs the memory manager to write the supplied value into the physical memory location  associated with the provided virtual address, performing translation and page swapping as necessary. Note, page swapping is a requirement for part 2 only.
-   ***load*** instructs the memory manager to return the byte stored at the memory location specified by virtual address. Like the store instruction, it is the memory manager’s responsibility to translate  and swap pages as needed. Note, the value parameter is not used for this instruction, but a dummy value (e.g., 0) should always be provided.
    

### Output.

The memory manager should output the results of its actions by printing messages to stdout.

### Simulation parameters.

-   Physical memory consists of 64 bytes divided into pages of 16 bytes. Likewise, the virtual address space is also 64 bytes.
-   For the purposes of this simulation, a process is simply a PID provided in the input command. Each process is given its own, isolated, virtual address space.
-   Your memory manager should be single-threaded.
-   All pages are readable, but only some are writeable—as specified by the value argument for the map instruction. We do not consider other protection bits for this project (e.g., executable).
-   The there can be at most 4 diﬀerent processes issuing instructions. These processes will have PIDs in the range [0, 3].
-   The manager will create page tables ON-DEMAND. That is, the manager will create the page table for a process when it receives the first command for that process. Each page table will be placed in its own page in the simulated memory array.
-   The manager will simply print an error if it cannot allocate a physical page to satisfy a request. In part 2 of the project, you will relax this assumption using swapping.
-   Each process will have a simulated hardware register pointing to the start of their respective page tables. You can simulate these registers with an array indexed by process id. This array does not need to be stored in your simulated physical memory array.
-   You may store limited information outside of your simulated physical memory array, e.g., local variables, a free list, and the page table registers. Yes, technically, these objects could also be put in the simulated memory, but it would complicate the implementation. However, as mentioned above, all page tables  must be placed in the simulated physical memory as well as all loads and stores from the simulated processes.
    
### Example output.

An example run of the memory manager might look like the following.

     	  Instruction? 0,map,0,1
          Put page table for PID 0 into physical frame 0
          Mapped virtual address 0 (page 0) into physical frame 1
          Instruction? 0,store,12,24
          Stored value 24 at virtual address 12 (physical address 28)
          Instruction? 0,load,12,0
          The value 24 is virtual address 12  (physical address 28)

## Part 2

 
In part 2, it extends the memory manager to support swapping memory pages to disk. The manager must ensure that the address spaces remain isolated. That is, memory from one process should not be readable from another process. The manager is expected to swap out page tables as necessary to free up memory.
Specifically, when the manager cannot satisfy a request because it does not have a free page it will do the following.

-   **First**, the manager will pick a page to evict. You get to pick the eviction strategy, but a simple round-robin should be suﬃcient. Note, we do not consider “always evict page 1” to be an acceptable eviction strategy. The manager must be able to swap out all pages, even those containing a page table.
-   **Second**, the manager will write the evicted page to disk. The swap space will be modeled by reading from and writing to a file. You can assume your swap space is very large.
-   **Third**, the manager will update the appropriate page table to record the swap-file location for the evicted page. Hint: you need a way to specify in the page table whether the page is in memory or swapped to disk.
-   **Finally**, the manager will use the newly-freed page to satisfy the request.
    
### Example output.

An example run of the memory manager might look like the following. Note, we manually added annotations to help clarify the example.

    Instruction?  0,map,0,0
	    Put page table for PID 0 into physical frame 0
	    Mapped virtual address 0  (page 0) into physical frame 1
	    #this should error
    
    Instruction?  0,store,7,255
	    Error: writes are not allowed to this page
	    #this should update the permissions
    
    Instruction?  0,map,0,1
	    Updating permissions for virtual page 0  (frame 1)
    
    Instruction?  0,store,7,255
	    Stored value 255 at virtual address 7  (physical address 23)
    
    Instruction?  0,load,7,0
	    The value 255 is virtual address 7  (physical address 23)
	    #this should print an error
	    
    Instruction?  0,map,10,1
	    Error: virtual page 0 is already mapped with rw_bit=1 #let’s map a couple other pages Instruction?  0,map,16,1
	    Mapped virtual address 16  (page 1) into physical frame 2 Instruction?  0,map,32,1
	    Mapped virtual address 32  (page 2) into physical frame 3
	    #Our physical memory should be full at this point, now we need to swap Instruction?  1,map,0,0
	    Swapped frame 2 to disk at swap slot 0
	    Put page table for PID 1 into physical frame 2 Swapped frame 1 to disk at swap slot 1
	    Mapped virtual address 0  (page 0) into physical frame 1 Instruction?  0,load,7,0
	    Swapped frame 3 to disk at swap slot 2
	    Swapped disk slot 1 into frame 3
	    The value 10 is virtual address 7  (physical address 55)
	    
    Instruction? End of file

