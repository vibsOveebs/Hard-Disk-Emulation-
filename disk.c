#include "driver.h"


void print_list(buffer_node* list) {
  buffer_node* curr = list;
  int i = 0;
  while (curr != NULL) {
    printf("%d: %s %d", i, curr->op_name, curr->sector_number);
    i++;
  }
}

buffer_node* add_to_sorted_list(buffer_node* list, buffer_node* item) {
  if (list == NULL) {
    item->next = NULL:
    return item;
  }

  if (item->sector_number < list->sector_number) {
    item->next = list;
    return item;
  }

  list->next = add_to_sorted_list(list->next, item);
  return list;
}




void *disk_ops(void *arg)
{
  int *algo = arg;
  int dir = 0;

  while(num_request_served < n)
  {
    while(buff_count < limit); //Spin lock

    pthread_mutex_lock(&lock);
    new_buffer_head = b_head;
    b_head = b_tail = NULL;
    buff_count = 0;
    pthread_mutex_unlock(&lock);


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
      buffer_node* temp_head = new_buffer_head;
      int y;
      buffer_node* lt_list = NULL;
      buffer_node* gt_list = NULL;
      buffer_node* eq_list = NULL;
      buffer_node* next_item;
      while (temp_head != NULL) {
        next_item = temp_head->next;
        buffer_node** list;
        if (temp_head->sector_number < disk_head) {
          list = &lt_list;
        }
        else if (temp_head->sector_number == disk_head) {
          list = &eq_list;
        }
        else {
          list = &gt_list;
        }

        *list = add_to_sorted_list(*list, temp_head);
      }



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


        /*implement Elevator Algorithm here */
    }
  }
}
