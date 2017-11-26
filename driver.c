#include "driver.h"

void init()
{
    for (int j=0 ; j<SECTORS*TRACKS ; j++)
    {
      disk[j] = 0;
    }

  srand(1000);
  clock_gettime(CLOCK_MONOTONIC, &globalClock);
  limit = 1;
  disk_head = 0;
  b_head = b_tail = NULL;
  /*do all other initializations here*/
}

void read_disk(int sector_number)
{
  char t[11];
  strcpy(t,"READ");
  ENTER_OPERATION(t,sector_number);
   
   //Adding request in the buffer
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

      new_buffer_head = b_head;

      disk_ops(0);

      b_head = b_tail = NULL;

  EXIT_OPERATION(t,sector_number);
}

void write_disk(int sector_number, int data)
{
  char t[11];
  strcpy(t,"WRITE");
  ENTER_OPERATION(t,sector_number);
    
    //Adding request in the buffer
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

      new_buffer_head = b_head;

      disk_ops(0);

      b_head = b_tail = NULL;

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
