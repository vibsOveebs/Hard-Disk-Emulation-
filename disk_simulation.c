#include "driver.h"

struct timespec* times;


struct timespec get_time_diff(struct timespec start, struct timespec end) {
  struct timespec diff;

  if ((end.tv_nsec - start.tv_nsec) < 0) {
      diff.tv_sec = end.tv_sec - start.tv_sec - 1;
      diff.tv_nsec = end.tv_nsec - start.tv_nsec + 1000000000;
  } 
  else {
      diff.tv_sec = end.tv_sec - start.tv_sec;
      diff.tv_nsec = end.tv_nsec - start.tv_nsec;
  }
  return diff;
}

void *_thread_handler(void *arg)
{
  thread_info *myinfo = (thread_info*)arg;
  sleep(myinfo->arrival_time);
    //Reading from disk


  struct timespec startTime;
  clock_gettime(CLOCK_MONOTONIC, &startTime);

  if(strcmp(myinfo->op_name,"READ") == 0)
  {
      read_disk(myinfo->sector_number);
  }

    //Writing a random data in the disk
  else
  {
      write_disk(myinfo->sector_number,rand()%512+1);
  }

  struct timespec localTime;
  clock_gettime(CLOCK_MONOTONIC, &localTime);

  myinfo->exit_time.tv_sec= localTime.tv_sec - globalClock.tv_sec;
  myinfo->exit_time.tv_nsec = localTime.tv_nsec - globalClock.tv_nsec;
  struct timespec diff = get_time_diff(startTime, localTime);
  times[myinfo->tid] = diff;
  printf("THREAD %d finished at %lld.%ld\n",myinfo->tid,(long long) myinfo->exit_time.tv_sec, myinfo->exit_time.tv_nsec);
  //printf("THREAD %d took %lld.%ld\n",myinfo->tid,(long long) diff.tv_sec, (long long)diff.tv_nsec);

}

int main(int argc, char const *argv[]) {
  int op;
  // n denotes number of requests
  // in 'sample_input.txt' the paramters are op, sector_number, arrival_time
  // if op == 0, then its a READ operation, else it is a WRITE operation


  scanf("%d",&n );
  pthread_t threads[101];

  for (int i= 0 ; i<n ; i++)
  {
    incoming[i] = (thread_info*)malloc(sizeof(thread_info));
    scanf("%d",&op);
    if(op == 0)
      strcpy(incoming[i]->op_name,"READ");
    else
      strcpy(incoming[i]->op_name,"WRITE");
    scanf("%d",&incoming[i]->sector_number);
    scanf("%d",&incoming[i]->arrival_time);
    incoming[i]->tid = i+1;
  }


  times = malloc(sizeof(struct timespec) * (n+1));
  //Initializer
  init();

  for(int i=0 ; i<n ; i++)
  {
    pthread_create(&threads[i],NULL,_thread_handler,incoming[i]);
  }

  for(int i=0 ; i<n ; i++)
  {
    pthread_join(threads[i],NULL);
  }
  pthread_join(disk_thread,NULL);

  printf("service times:\n");
  for (int i = 1; i < n+1; i++) {
    struct timespec t = times[i];
    printf("%lld.%ld\n", (long long) t.tv_sec, (long long)t.tv_nsec);

  }

  return 0;
}
