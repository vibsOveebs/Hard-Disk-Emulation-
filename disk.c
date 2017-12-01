#include "driver.h"

void *disk_ops(void *arg)
{
  int *algo = arg;

  while(num_request_served < n)
  {
    while(buff_count < limit); //Spin lock

    new_buffer_head = b_head;
    b_head = b_tail = NULL;
    buff_count = 0;

    if(*algo == 0)
    {
      //Following FCFS
      buffer_node* temp_head = new_buffer_head;
      int y;
      while(temp_head!=NULL)
      {
        y = temp_head->sector_number;
        int movement =  abs(sectorToTrack(y) - sectorToTrack(disk_head));
        usleep(AVG_ROT_DELAY + movement);
        printf("Head Movement: %d\n",movement);
        disk_head = temp_head->sector_number;
        if(strcmp(temp_head->op_name,"READ") == 0)
        {
          printf("DISK READ\n" );
          if(temp_head->sector_number >= SECTORS*TRACKS )
            printf("BAD SECTOR\n");

          else
            printf("READ %d from SECTOR %d [Track %d]\n", disk[y], y, sectorToTrack(y) + 1);
        }
        else{
          if(temp_head->sector_number >= SECTORS*TRACKS )
            printf("BAD SECTOR\n");

          else{
            disk[y] = temp_head->data;
            printf("WROTE %d into SECTOR %d [Track %d]\n", disk[y], y, sectorToTrack(y) + 1);
          }
        }
        temp_head = temp_head->next;
        num_request_served++;
      }
    }
      else{

        /*implement Elevator Algorithm here */
      }
    }
}
