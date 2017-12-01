#include "driver.h"

void init()
{
  for (int i=0 ; i<SECTORS*TRACKS ; i++)
  {
      disk[i] = 0;
  }
  srand(1000);
  clock_gettime(CLOCK_MONOTONIC, &globalClock);
  buff_count = 0;
  limit = 1;
  disk_head = 0;
  num_request_served = 0;

  int *algo;
  algo = (int *) malloc (sizeof(int));
  *algo = 0;
  pthread_create(&disk_thread,NULL,disk_ops,algo);
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

    if(b_head == NULL && b_tail == NULL)
    {
      b_head = b_tail = temp1;
    }
    else{
      b_tail->next = temp1;
      b_tail = temp1;
    }
    buff_count ++;

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

    if(b_head == NULL && b_tail == NULL)
    {
      b_head = b_tail = temp1;
    }
    else{
      b_tail->next = temp1;
      b_tail = temp1;
    }
    buff_count ++;

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
