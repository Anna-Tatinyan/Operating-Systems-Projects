#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <wait.h>
#include <sys/time.h>
  
pthread_mutex_t  lock;
pthread_mutex_t  counterLock;

int teams;
int team[4];
int departmentsBusy[4] = {0,0,0,0};
int probabilityOf25percent[] = {1, 0, 0, 0};


int counter = 0;
int typeNowIn = -1;

int isFair = 1;

struct costumeArgs {
  int number;
  int type;
  int arriveTime;
  int costumeTime;
};

struct costumeReturn {
  int number;
  int type;
  int waited;
  int wasInShop;
};

void *costume(void *args){
  struct costumeReturn* ret = (struct costumeReturn*) malloc(sizeof(struct costumeReturn) * 100);
  int visit = -1;
  struct costumeArgs arg = (*((struct costumeArgs *) args));
  struct timeval start, end;
  struct timeval startD, endD;
  double elapsedTime;
  int n = arg.number;
  while(1){
    visit++;
    ret[visit].number = arg.number; 
    ret[visit].type = arg.type;
    int arrTime = rand()%(2*arg.arriveTime+1);
    sleep(arrTime);
    gettimeofday(&start, NULL);
    if(typeNowIn == -1){
      typeNowIn = arg.type;
    }
    while(typeNowIn != arg.type) {
      if(counter == 0) {
        typeNowIn = arg.type;
        isFair = 1;
        break;
      }
      gettimeofday( &end, NULL);
      elapsedTime = (end.tv_sec - start.tv_sec);
      
      if(elapsedTime >= 30) {
      	isFair = 0;
      }
    }
    while(isFair == 0){

    }
    if(counter >= teams - 1){
      pthread_mutex_lock(&lock);
    }
    while(counter >= teams || typeNowIn != arg.type){
    }
    counter++;
    int index = 0;
    for(int i = 0; i < teams; i++){
      if(team[i] == 0){
        index = i;
        gettimeofday( &startD, NULL);
        team[i] = 1;
        break;
      }
    }
    if(arg.type == 0){
      printf("Pirate %d entering shop with team %d \n", arg.number, index);
    } else {
      printf("Ninja %d entering shop of team %d \n", arg.number, index);
    }
    gettimeofday( &end, NULL);
    ret[visit].waited = (end.tv_sec - start.tv_sec);  
    int randomCostumeTime = rand()%(2*arg.costumeTime+1);
    ret[visit].wasInShop = randomCostumeTime;
    sleep(randomCostumeTime);
    gettimeofday( &endD, NULL);
    elapsedTime = (endD.tv_sec - startD.tv_sec);
    departmentsBusy[index] += elapsedTime;
    team[index] = 0;
    if(arg.type == 0){
      printf("Pirate %d leaving shop with team %d \n", arg.number, index);
    } else {
      printf("Ninja %d leaving shop of team %d \n", arg.number, index);
    }
    counter--;
    if(counter < teams - 1){
      pthread_mutex_unlock(&lock);
    }
    int random = rand()%4;
    if(probabilityOf25percent[random] == 0){
      break;
    }
  }
  visit++;
  ret[visit].number = -1;   
  return (void *) ret;
}

int main(void) {
  int pirates;
  int ninjas;
  int costumep;
  int costumen;
  int arrivalp;
  int arrivaln;
  scanf("%d %d %d %d %d %d %d", &teams, &pirates, &ninjas, &costumep, &costumen, &arrivalp, &arrivaln);
  printf("-- Starting Simulation -- \n");
  int arrOfCustomers[pirates + ninjas];
  for(int i = 0; i < pirates; i++){
  	arrOfCustomers[i] = i;
  }
  for(int i = pirates; i < pirates + ninjas; i++){
  	arrOfCustomers[i] = i - pirates;
  }
  pthread_t threads[pirates + ninjas];
  for(int i = 0; i < pirates + ninjas; i++){
   struct costumeArgs* p = malloc(sizeof(struct costumeArgs));
	 if(i >= pirates) {
   		(*p).number = i-pirates;
   		(*p).type = 1;
      (*p).arriveTime = arrivaln;
      (*p).costumeTime = costumen;
   	}
   	else {
  		(*p).number = i;
  		(*p).type = 0;
      (*p).arriveTime = arrivalp;
      (*p).costumeTime = costumep;
  	}
      pthread_t t;
    	int err = pthread_create(&t, NULL, costume, p);
      if (err){
        printf("An error occured: %d", err);
      }
    	threads[i] = t;
  }
  struct costumeReturn* returns[pirates + ninjas];
  for(int i = 0; i < pirates + ninjas; i++){
    void *retval;
  	pthread_join(threads[i], &retval);
    struct costumeReturn* ret = (struct costumeReturn *) retval;
    returns[i] = ret;
  }
  printf("\n");
  printf("-- End of Simulation --");
  int totalCoinP = 0;
  int totalCoinN = 0;
  int customers = 0;
  for(int i = 0; i < pirates + ninjas; i++){
    printf("\n");
    struct costumeReturn* ret = returns[i];
    if(ret[0].type == 0){
      printf("Pirate %d: \n", ret[0].number);
    } else {
      printf("Ninja %d: \n", ret[0].number);
    }
    int i;
    int coins = 0;
    for(i = 0; i < 100; i++){
      customers++;
      if(ret[i].number == -1){
        break;
      }
      printf("Visit %d: Waited for %d minutes and was in shop for %d minutes. \n", i + 1, ret[i].waited, ret[i].wasInShop);
      if(ret[i].waited < 30){
        coins += ret[i].wasInShop;
      }
    }
    if(ret[0].type == 0){
      printf("Total number of visits: %d. Cost for Pirate %d: %d \n", i, ret[0].number, coins);
      totalCoinP += coins;
    } else {
      printf("Total number of visits: %d. Cost for Ninja %d: %d \n", i, ret[0].number, coins);
      totalCoinN += coins;
    }
  }

  printf("All of the Pirates cost %d gold pieces \n", totalCoinP);

  printf("All of the Ninjas cost %d gold pieces \n", totalCoinN);
  
  printf("Expenses for employing the costume teams is: %d gold pieces \n", teams * 5);
  for(int i = 0; i<teams; i++) {
  	  printf("Team %d was busy for %d minutes and free for 37 minutes.\n", i, departmentsBusy[i]);
  }
  
  printf("Gross revenue is: %d \n", totalCoinP + totalCoinN);
  printf("Gold per visit: %f \n", (totalCoinP + totalCoinN)/(double)customers);
  printf("Total revenue: %d \n" ,totalCoinP + totalCoinN - teams * 5);

  
  return 0;
}
