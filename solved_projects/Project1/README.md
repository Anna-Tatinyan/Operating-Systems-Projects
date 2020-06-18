# Project 1: The Three Commanders

This project will create three diﬀerent executables, with increasing capabilities:

The project contains:

		   1. boring.c - includes the code of the first part of the assignment. 
           2. custom.c - includes the code of the second part of the assignment.
           3. multi.c - includes the code of the third part of the assignment.
           4. custom.txt - the example test case for custom commander
           5. multi.txt - the example test case for multitasking commander
           6. customTestCases.txt - our test cases for custom commander
           7. multiTestCases.txt - our test cases for multitasking commander
           8. OutputForBoring.txt -  outputs of example test cases for boring commander 
           9. OutputForCustom.txt - outputs of example test cases for custom commander
           10. OutputForMulti.txt - outputs of example test cases for multitasking commander
           11. OutputForCustomTests.txt - outputs of our test cases for custom commander
           12. OutputForMultiTests.txt - outputs of our test cases for multitasking commander
           13. Makefile programs.


1.  **The Boring Commander:** This software runs a sequence of pre-programmed commands, outputting results and statistics as it goes. The commands the Boring Commander runs are simple and do not involve side-eﬀects between commands.
    
2.  **The Custom Commander:** This software opens a file, custom.txt, and runs the commands in order. Unlike the Boring Commander, the Custom Commander supports side-eﬀects (like changing directories).
 
3.  **The Multitasking Commander:** The Multitasking Commander supports the notion of background processes. It opens a file, multi.txt, to determine what commands to run. It also takes command line parameters that indicate what lines should be run in the background.
    
    
## Phase 1: The Boring Commander

  

For the Boring Commander, you will run an executable that, in turn, will run the following three hard-coded commands (which already exist in Linux)

>  whoami
>  last
>  ls -al /home*
> 
After running each command, the Boring Commander will print statistics about the process execution.

### Example Boring Commander Output

    user@test:~> ./boring 
    Running command: whoami user
	     -- Statistics --- 
	    Elapsed time: 1 milliseconds 
	    Page Faults: 0 
	    Page Faults (reclaimed): 0
	    -- End of Statistics --

    Running command: last user pts/1
        1.2.3.4 Fri Jan 13 03:14 still logged in
         -- Statistics --- 
        Elapsed time: 2 milliseconds 
        Page Faults: 0 
        Page Faults (reclaimed): 0
         -- End of Statistics -- 

    Running command: ls -al /home 
    total 132 drwxr-xr-x 33 root root drwxr-xr-x 24 root root drwxr-xr-x 3 user
            root 4096 Oct 5 13:40 . 4096 Dec 6 09:20 .. 4096 Aug 31 2016 user
         -- Statistics --- 
        Elapsed time: 7 milliseconds 
        Page Faults: 0 
        Page Faults (reclaimed): 0
         -- End of Statistics --



## Phase 2: The Custom Commander

  

The Custom Commander supports two new commands: ccd, which changes the current directory, and cpwd, which prints the current working directory. The Custom Commander detects these commands and process them internally rather than invoking them using an exec function call. This is so their eﬀects can persist to subsequent commands.

### Example Input File

> ccd / 
> ls -alh 
> cpwd 
> ccd /home 
> ls -alh 
> cpwd

  
### Example Custom Commander Output

 

    user@test:~> ./custom
    Running command: ccd /
    Changed to directory: /
    [Only output from ccd]
    Running command: ls

	total 256K
	drwxr-xr-x  24  root root 4.0K  Dec  4  06:30 .  
	drwxr-xr-x  24  root root 4.0K  Dec  4  06:30 ..
	drwxr-xr-x  2   root root 12K   Jan  3  09:34 bin
	drwxr-xr-x  3   root root 4.0K  Jan  3  09:38 boot

	  -- Statistics ---
    Elapsed time: 4 milliseconds
    Page Faults: 0
    Page Faults (reclaimed): 0
    -- End of Statistics --
    Running command: cpwd
    Current directory: /
    Running command: ccd
    Changed to directory: /home
    [Only output from cpwd]
    [Only output from ccd]
    4Running command: ls -alh
    total 132
    drwxr-xr-x 33 root root
    drwxr-xr-x 24 root root
    drwxr-xr-x 3  user root
    4096 Oct 5 13:40 .
    4096 Dec 6 09:20 ..
    4096 Aug 31 2016 user
    
    -- Statistics ---
    
    Elapsed time: 5 milliseconds
    Page Faults: 0
    Page Faults (reclaimed): 0
    -- End of Statistics --
    
    Running command: cpwd
    Current directory: /home
    [Only output from cpwd]

## Phase 3: The Multitasking Commander

  

The Multitasking Commander does everything the Custom Commander does, but adds two new features: the ability to run background commands and the ability to print running jobs.

To implement background commands, the Multitasking Commander reads the command-line arguments to determine which lines in multi.txt should be run in the background. To do so, it will loop through the values in the ARGV array in int main, parse each to an integer (see the atoi function), and store those line numbers. When it reads the associated line number from multi.txt, it will run it in the background. As a result, there may be multiple child processes active at once, even while multi is reading an additional command. Moreover, a background task can terminate at any time. In this case, multi should display a message that the particular task has terminated, and it should follow that message with the statistics about the command of that task.

In addition to the built-in commands of custom, multi must also handles the cproclist command, which lists all background processes currently active.

### Example Input File

> sleep 10
>   ls -alh
>  sleep 3
> cproclist
> cpwd
> 
### Example Custom Commander Output


    user@test:~> ./multi 1 3
    
    Running command: sleep 10
    Background: ID [0]: sleep 10
    Running command: ls -alh
    total 132
    drwxr-xr-x 33 root root 4096 Oct 5  13:40 .
    drwxr-xr-x 24 root root 4096 Dec 6  09:20 ..
    drwxr-xr-x 3  user root 4096 Aug 31 2016  user
    
    -- Statistics ---
    Elapsed time: 5 milliseconds
    Page Faults: 0
    Page Faults (reclaimed): 0
	  -- End of Statistics --
    Running command: sleep 3
	    Background: ID [1]: sleep 3
	Running command: cproclist
    -- Background Processes --
    [0] sleep 10
    [1] sleep 3
    Running command: cpwd
    Current directory: /
    [Only output from cpwd]
    -- Job Complete [1: sleep 3] --
    Process ID: 12345
    -- Statistics ---
    Elapsed time: 3000 milliseconds
    Page Faults: 0
    Page Faults (reclaimed): 0
    -- End of Statistics --
    -- Job Complete [0: sleep 10] --
    Process ID: 12340
    -- Statistics ---
    Elapsed time: 10000 milliseconds
    Page Faults: 0
    Page Faults (reclaimed): 0
    -- End of Statistics --
    
 
If the file processing completes before all the background tasks have completed, then multi program should wait() for those tasks to be completed.


