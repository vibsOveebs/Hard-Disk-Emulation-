#include "driver.h"

void disk_ops(int algo)
{

  if(algo == 0)
  {
    //Following FCFS
    buffer_node* temp_head = new_buffer_head;
    int y;
    while(temp_head!=NULL)
    {
      y = temp_head->sector_number;
      int movement =  abs((temp_head->sector_number/SECTORS) - (disk_head/SECTORS));
      usleep(AVG_ROT_DELAY + movement);
      printf("Movement took %d usecs \n", AVG_ROT_DELAY + movement);
      disk_head = temp_head->sector_number;
      if(strcmp(temp_head->op_name,"READ") == 0)
      {
        if(temp_head->sector_number >= SECTORS*TRACKS )
          printf("BAD SECTOR\n");

        else
          printf("READ %d from SECTOR %d [Track %d]\n", disk[y], temp_head->sector_number, (y/SECTORS) + 1);
      }
      else{
        if(temp_head->sector_number >= SECTORS*TRACKS )
          printf("BAD SECTOR\n");

        else{
          disk[y] = temp_head->data;
          printf("WROTE %d into SECTOR %d [Track %d]\n", disk[y], temp_head->sector_number, (y/SECTORS) + 1);
        }
      }
      temp_head = temp_head->next;
    }
  }
    else{

      /*implement Elevator Algorithm here */
    }
}
