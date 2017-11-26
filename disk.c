#include "driver.h"

void disk_ops(int algo)
{

  if(algo == 0)
  {
    //Following FCFS
    buffer_node* temp_head = new_buffer_head;
    int x,y;
    while(temp_head!=NULL)
    {
      x = (temp_head->sector_number/(SECTORS*TRACKS));
      y = temp_head->sector_number % (SECTORS*TRACKS);
      int movement =  abs(temp_head->sector_number - disk_head);
      printf("Head Movement: %d\n",movement);
      disk_head = temp_head->sector_number;
      if(strcmp(temp_head->op_name,"READ") == 0)
      {
        if(temp_head->sector_number >= SECTORS*TRACKS )
          printf("BAD SECTOR\n");

        else
          printf("READ %d from SECTOR %d [Track %d]\n", disk[x][y], temp_head->sector_number, (y/SECTORS) + 1);
      }
      else{
        if(temp_head->sector_number >= SECTORS*TRACKS )
          printf("BAD SECTOR\n");

        else{
          disk[x][y] = temp_head->data;
          printf("WROTE %d into SECTOR %d [Track %d]\n", disk[x][y], temp_head->sector_number, (y/SECTORS) + 1);
        }
      }
      usleep(DISK_MOVE_TIME * movement);
      printf("Request took %d usecs \n",DISK_MOVE_TIME * movement);
      temp_head = temp_head->next;
    }
  }
    else{

      /*implement Elevator Algorithm here */
    }
}
