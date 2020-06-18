# Project 3: Threads and Synchronization

The project contains:
	       
	       1. piratesAndNinjas.c - includes the code of the assignment.
	       2. Makefile program that compiles the code.
	       4. test1.txt - test file N1
	       5. test2.txt - test file N2
	       6. output.txt - output from test1
	       7. output2.txt - output from test2

  

## Problem: Pirates and Ninjas

After an epic feud spanning years, the leaders from the pirate and the ninja forces met to negotiate a peace agreement. They agreed that their battles weakened their forces and reputations, creating a power vacuum. A new quarrel, between some melancholy vampires and a pack of werewolves (not “swear-wolves”), threatened to dominate the media and relegate the pirate v. ninja feud to obscurity. They could not let this happen.

While both sides needed a peace to rebuild their forces, they could not let this become public knowledge for fear that it would make them look weak. If pirates and ninjas are ever seen in public together, they must fight to the death. To complicate matters, both the pirates and the ninjas used the same costume department. Before pirating or ninjaing (ninjasting?), a pirate or ninja must go through the costume department to acquire the proper garb, makeup, and even to rehearse lines (admittedly, the last of which does not take long for a ninja).

The costume department staﬀs 2–4 teams and each team can outfit one individual at a time. On any given day, the department will costume 10–50 pirates and 10–50 ninjas.

As the head of the costume department, it is your job to synchronize the pirates and the ninjas so that no ninja or pirate should ever have to fight to the death. You can assume that if any pirate is in the costume department, it will be forced to fight any ninja that enters (and vice versa). Of course, two pirates may safely use the costume department at the same time, as may two ninjas. You can assume the ninjas and pirate will only see each other if they are in the costume department (they have separate entrances/exits). The pirate/ninja peace agreement requires that neither side may deprive the other of the costume department by always occupying the department, so you must preserve fairness. It would be a shame if the peace agreement failed due to a wardrobe malfunction.

Each execution of your program represents the operation of the costume shop over a single day. Each pirate and each ninja is represented as a unique thread. You must implement code that creates the requisite number of ninja and pirate threads and use either semaphores or locks to synchronize the costume depart-ment. Each ninja and pirate thread should identify itself and which costume team it is using when it enters or leaves the costume department. For this problem, no time-of-day scheduling is allowed (e.g., ninjas in one half of each hour and pirates in the other half of each hour). All arrivals will be at random, and the amount of time that any individual takes to be outfit is also random. Though, you can expect that pirates will tend to take longer than ninjas, i.e., you should simulate this behavior by choosing appropriate random values. Simulate a pirate or ninja costume preparation using the sleep() call with a value drawn from random distribution. A pirate or ninja may need costume assistance multiple times a day (ripped costumes are a natural consequence of rough-and-tumble work). At the end of their costume session, the ninja or pirate must inform the costume department if they will be back that day. This decision is made in the spur of the moment with a probability of answering “yes” being 25%.

During operation, the costume department should keep track of the usage statistics for proper billing; specifically, each pirate and ninja must spend 1 gold piece for every minute inside of the costume department (expensive, but you run a high-end shop after all). However, if any pirate or ninja has to wait for more than 30 minutes prior to entering the shop then you have to costume them for free. At the end of the day, you should calculate and print the itemized bills for both the pirates and the ninjas. For each ninja/pirate you should list the number of visits, the amount of time of each visit, the wait times, and the total gold owed to the costume department.

You also need to keep track of your own expenses profits. Each costuming team costs 5 gold pieces per day to staﬀ. You should print also the amount of time that each team was busy, the amount of time each team was free, the average queue length, the gross revenue (amount of gold), the gold-per-visit (amount of gold divided by number of visits), and your total profits.

Once you have completed your solution, explain how your solution maximizes profits while not depriving one side or the other of the costume department. Your description should include the output of an example run and an analysis of your synchronization scheme in the context of that run. Save this in a text file, problem1 explanation.txt, accompanying your code.

  
### Command Line Arguments

For the purposes of simulation, assume that 1 second of execution represents 1 minute of time in the ninja-pirate world. Your program should take the following arguments.
-   The number of costuming teams (min. 2, max. 4).
-   The number of pirates (10–50).
-   The number of ninjas (10–50).
-   The average costuming time of a pirate. This is the average amount of time (in execution seconds) they spend in the costume shop.
-   The average costuming time of a ninja. This is the average amount of time (in execution seconds) they spend in the costume shop.
-   The average arrival time of a pirate. This is the average amount of time (in execution seconds) they spend adventuring before visiting the costuming department. Note, some individuals will visit multiple times.
-   The average arrival time of a ninja. This is the average amount of time (in execution seconds) they spend adventuring before visiting the costuming department. Note, some individuals will visit multiple times.
    
### Sample Output

Your output should look similar to the following:
  

    $ ./piratesNinjas 2  10  10  5  5  5  5  1.
    
	    -- Starting Simulation --
    
    	Pirate 0 entering shop with team 1
    	Pirate 0 leaving shop of team 1
    	Ninja 9 entering shop with team 1
    	Ninja 8 entering shop with team 0
    	Ninja 9 leaving shop of team 1
    	Ninja 0 entering shop with team 1
    	Ninja 8 leaving shop of team 0
    	Ninja 0 leaving shop of team 1
    	Ninja 5 entering shop with team 1
    	Ninja 7 entering shop with team 0
    	Ninja 5 leaving shop of team 1
    	Ninja 4 entering shop with team 1
    	Ninja 7 leaving shop of team 0
    	Ninja 3 entering shop with team 0
    	Ninja 4 leaving shop of team 1
    	Ninja 3 leaving shop of team 0
    	Pirate 6 entering shop with team 1
    	Pirate 2 entering shop with team 0
    	Pirate 2 leaving shop of team 0
    	Pirate 9 entering shop with team 0
    	Pirate 6 leaving shop of team 1
    	Pirate 8 entering shop with team 1
    	Pirate 9 leaving shop of team 0
    	Pirate 5 entering shop with team 0
    	Pirate 5 leaving shop of team 0
    	Pirate 4 entering shop with team 0
    	Pirate 8 leaving shop of team 1
    	Pirate 7 entering shop with team 1
    	Pirate 4 leaving shop of team 0
    	Pirate 3 entering shop with team 0
    	Pirate 7 leaving shop of team 1
    	Pirate 3 leaving shop of team 0
    	Ninja 1 entering shop with team 1
    	Ninja 6 entering shop with team 0
    	Ninja 6 leaving shop of team 0
    	Ninja 2 entering shop with team 0
    	Ninja 1 leaving shop of team 1
    	Ninja 8 entering shop with team 1
    	Ninja 2 leaving shop of team 0
    	Ninja 4 entering shop with team 0
    	Ninja 8 leaving shop of team 1
    	Ninja 4 leaving shop of team 0
    	Pirate 1 entering shop with team 1
    	Pirate 8 entering shop with team 0
    	Pirate 8 leaving shop of team 0
    	Pirate 1 leaving shop of team 1
    	Ninja 8 entering shop with team 1
    	Ninja 8 leaving shop of team 1
    	Pirate 1 entering shop with team 1
    	Pirate 1 leaving shop of team 1
    	Pirate 1 entering shop with team 1
    	Pirate 1 leaving shop of team 1
    	-- End of Simulation --
    
    	Pirate 0:
    	Visit 1: Waited for  0 minutes and was in shop for  4 minutes.
    	Total number of visits:  1. Cost for Pirate 0:  4
    
    	Pirate 1:
    	Visit 3: Waited for  0 minutes and was in shop for  5 minutes.
    	Visit 2: Waited for  4 minutes and was in shop for  4 minutes.
    	Visit 1: Waited for  45 minutes and was in shop for  6 minutes.
    	Total number of visits:  3. Cost for Pirate 1:  9
    
    	Pirate 2:
    	Visit 1: Waited for  14 minutes and was in shop for  3 minutes.
    	Total number of visits:  1. Cost for Pirate 2:  3
    
    	Pirate 3:
    	Visit 1: Waited for  26 minutes and was in shop for  5 minutes.
    	Total number of visits:  1. Cost for Pirate 3:  5
    
    	Pirate 4:
    	Visit 1: Waited for  22 minutes and was in shop for  5 minutes.
    	Total number of visits:  1. Cost for Pirate 4:  5
    
    	Pirate 5:
    	Visit 1: Waited for  20 minutes and was in shop for  2 minutes.
    	Total number of visits:  1. Cost for Pirate 5:  2
    
    	Pirate 6:
    	Visit 1: Waited for  14 minutes and was in shop for  4 minutes.
    	Total number of visits:  1. Cost for Pirate 6:  4
    
    	Pirate 7:
    	Visit 1: Waited for  22 minutes and was in shop for  5 minutes.
    	Total number of visits:  1. Cost for Pirate 7:  5
    
    	Pirate 8:
    	Visit 2: Waited for  17 minutes and was in shop for  4 minutes.
    	Visit 1: Waited for  17 minutes and was in shop for  6 minutes.
    	Total number of visits:  2. Cost for Pirate 8:  10
    
    	Pirate 9:
    	Visit 1: Waited for  16 minutes and was in shop for  4 minutes.
    	Total number of visits:  1. Cost for Pirate 9:  4
    
    	All of the Pirates cost 51 gold pieces
    
    	Ninja 0:
    	Visit 1: Waited for  5 minutes and was in shop for  3 minutes.
    	Total number of visits:  1. Cost for Ninja 0:  3
    
    	Ninja 1:
    	Visit 1: Waited for  33 minutes and was in shop for  5 minutes.
    	Total number of visits:  1. Cost for Ninja 1:  0
    
    	Ninja 2:
    	Visit 1: Waited for  35 minutes and was in shop for  5 minutes.
    	Total number of visits:  1. Cost for Ninja 2:  0
    
    	Ninja 3:
    	Visit 1: Waited for  11 minutes and was in shop for  3 minutes.
    	Total number of visits:  1. Cost for Ninja 3:  3
    
    	Ninja 4:
    	Visit 2: Waited for  22 minutes and was in shop for  5 minutes.
    	Visit 1: Waited for  10 minutes and was in shop for  4 minutes.
    	Total number of visits:  2. Cost for Ninja 4:  9
    
    	Ninja 5:
    	Visit 1: Waited for  7 minutes and was in shop for  3 minutes.
    	Total number of visits:  1. Cost for Ninja 5:  3
    
    	Ninja 6:
    	Visit 1: Waited for  31 minutes and was in shop for  4 minutes.
    	Total number of visits:  1. Cost for Ninja 6:  0
    
    	Ninja 7:
    	Visit 1: Waited for  7 minutes and was in shop for  4 minutes.
    	Total number of visits:  1. Cost for Ninja 7:  4
    
    	Ninja 8:
    	Visit 3: Waited for  6 minutes and was in shop for  7 minutes.
    	Visit 2: Waited for  27 minutes and was in shop for  5 minutes.
    	Visit 1: Waited for  3 minutes and was in shop for  5 minutes.
    	Total number of visits:  3. Cost for Ninja 8:  17
    
    	Ninja 9:
    	Visit 1: Waited for  3 minutes and was in shop for  2 minutes.
    	Total number of visits:  1. Cost for Ninja 9:  2
    
    	All of the Ninjas cost 41 gold pieces
    	Expenses for employing the costume teams is:  10 gold pieces
    	Team 0 was busy for  44 minutes and free for  37 minutes.
    	Team 1 was busy for  59 minutes and free for  22 minutes.
    	Average queue length was 5.148148 people
    	Gross revenue is 92 gold pieces
    	Gold per visit:  3.538462
    	Total revenue:  82



## **Problem Explanations:**

### Problem: Pirates and Ninjas

  

 - Both the pirates and the ninjas have to go to the costume department to get a make up and dress up.
 -  They use the same costume department.
#### What the program has to consider
          
	 
 - Ninjas and Pirates should not meet
	 - For that purpose whenever one type is getting into department the other one can't get into, because it acquires a lock and waits till the department is free.
- There should be a fairness between two types
	- For that purpose whenever one thread (ninja or pirate) waits for more than 30 seconds, the department stops letting the same type of customers arrive and waits till it is empty to let the other type enter and have fairness.
-  There should be a randomness while picking cutomers
	- For that purpose we have a random function that generates randomly a  number and sends it to sleep function. This way each thread will come in a different time and enter costume function.
- Customer can return with 25% probability
	- For that purpose we have an array of 4 numbers: filled with three 0s and one 1. We pick randomly a number that represents index in that array, the value represents    whether or not we should repeat the customer's "appointment".
- No one should enter if all costume departments are busy
	- For that purpose we again use lock so that if a thread wants to get in they acquire a lock and wait, till the department has free spots.

