#include <pthread.h>
#include <signal.h>
#include "driver.h"
void alarm_event() {
  should_run = 1;
  //printf("alarm\n");
}

void init()
{
  for (int i=0 ; i<SECTORS*TRACKS ; i++)
  {
      disk[i] = 0;
  }
  srand(1000);
  clock_gettime(CLOCK_MONOTONIC, &globalClock);
  buff_count = 0;
  limit = 5;
  disk_head = 0;
  num_request_served = 0;
  //lock = PTHREAD_MUTEX_INITIALIZER; 

  pthread_mutex_init(&lock, NULL);
  int *algo;
  algo = (int *) malloc (sizeof(int));
  *algo = 1;
  pthread_create(&disk_thread,NULL,disk_ops,algo);

  //timer setup
  struct sigaction action;
  action.sa_handler = &alarm_event;
  action.sa_flags = 0;  // Restart interrupted system calls
  sigemptyset(&action.sa_mask);
  sigaction(SIGALRM, &action, NULL);
}

int sectorToTrack(int sector_number)
{
  return sector_number/SECTORS;
}

void read_disk(int sector_number)
{
  char t[11];
  strcpy(t,"READ");
  ENTER_OPERATION(t,sector_number);


  buffer_node*  temp1 = (buffer_node*)malloc(sizeof(buffer_node));
  strcpy(temp1->op_name,t);
  temp1->sector_number = sector_number;
  temp1->next = NULL;
  temp1->data = 0;
  
  while (1) {
    pthread_mutex_lock(&lock);
    if (buff_count < limit) {
      if(b_head == NULL && b_tail == NULL)
      {
        b_head = b_tail = temp1;
      }
      else{
        b_tail->next = temp1;
        b_tail = temp1;
      }
      buff_count ++;
      alarm(1);
      pthread_mutex_unlock(&lock);
      break;
    }
    pthread_mutex_unlock(&lock);
  }

  //wait for operation to finish up
  EXIT_OPERATION(t,sector_number);
}

void write_disk(int sector_number, int data)
{
  char t[11];
  strcpy(t,"WRITE");
  ENTER_OPERATION(t,sector_number);
  buffer_node*  temp1 = (buffer_node*)malloc(sizeof(buffer_node));
  strcpy(temp1->op_name,t);
  temp1->sector_number = sector_number;
  temp1->next = NULL;
  temp1->data = data;

  while (1) {
    pthread_mutex_lock(&lock);
    if (buff_count < limit) {
      if(b_head == NULL && b_tail == NULL)
      {
        b_head = b_tail = temp1;
      }
      else{
        b_tail->next = temp1;
        b_tail = temp1;
      }
      buff_count ++;
      pthread_mutex_unlock(&lock);
      alarm(1);
      break;
    }
    pthread_mutex_unlock(&lock);
  }

  //wait for operation to finish up
  EXIT_OPERATION(t,sector_number);

}

void ENTER_OPERATION(char *op_name, int sector_number)
{
  /*Enter Synchronization code here */
}

void EXIT_OPERATION(char *op_name, int sector_number)
{
  /*Exit Synchronization code here */
}
