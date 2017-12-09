#include "driver.h"
#define DIR_LEFT 0
#define DIR_RIGHT 1  

buffer_node* reverse_list(buffer_node* list) {
  if (list == NULL) {
    return NULL;
  } 
  else if (list->next == NULL) {
    return list;
  }
  else {
    buffer_node* rest = reverse_list(list->next);
    rest->next = list;
    list->next = NULL;
    return rest;
  }
}

void print_list(buffer_node* list) {
  buffer_node* curr = list;
  int i = 0;
  while (curr != NULL) {
    printf("%d: %s %d\n", i, curr->op_name, curr->sector_number);
    i++;
    curr = curr->next;
  }
}

buffer_node* add_to_sorted_list(buffer_node* list, buffer_node* item) {
  if (list == NULL) {
    item->next = NULL;
    return item;
  }

  if (item->sector_number < list->sector_number) {
    item->next = list;
    return item;
  }

  item->next = NULL;
  list->next = add_to_sorted_list(list->next, item);
  return list;
}

void process_list(buffer_node* list, int dir) {
  if (list == NULL) {
    return;
  }

  int max_sector = SECTORS * TRACKS - 1;
  int movement = 0;
  int y = list->sector_number;
  if (dir == DIR_LEFT && disk_head < y) {
    movement = abs(sectorToTrack(0) - sectorToTrack(disk_head)) + abs(sectorToTrack(0) - sectorToTrack(y));
  }
  else if (dir == DIR_RIGHT && disk_head > y) {
    movement = abs(sectorToTrack(max_sector) - sectorToTrack(disk_head)) + abs(sectorToTrack(max_sector) - sectorToTrack(y));
  }
  else {
    movement = abs(sectorToTrack(y) - sectorToTrack(disk_head));
  }
  usleep(AVG_ROT_DELAY + movement);
  printf("Head Movement: %d\n",movement);
  disk_head = list->sector_number;

  if(strcmp(list->op_name,"READ") == 0)
  {
    printf("DISK READ\n" );
    if(list->sector_number >= SECTORS*TRACKS )
      printf("BAD SECTOR\n");

    else
      printf("READ %d from SECTOR %d [Track %d]\n", disk[y], y, sectorToTrack(y) + 1);
  }
  else{
    if(list->sector_number >= SECTORS*TRACKS )
      printf("BAD SECTOR\n");

    else{
      disk[y] = list->data;
      printf("WROTE %d into SECTOR %d [Track %d]\n", disk[y], y, sectorToTrack(y) + 1);
    }
  }
  num_request_served++;
  //printf("request served: %d\n", num_request_served);
  process_list(list->next, dir);
}


void *disk_ops(void *arg)
{
  int *algo = arg;
  int dir = DIR_RIGHT;

  while(num_request_served < n)
  {

    while(buff_count < limit && !should_run); //Spin lock
    should_run = 0;
    pthread_mutex_lock(&lock);
    new_buffer_head = b_head;
    b_head = b_tail = NULL;
    buff_count = 0;
    pthread_mutex_unlock(&lock);


    if(*algo == 0)
    {
      //Following FCFS
      printf("FCFS\n");
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
      //printf("ELEVATOR\n");
      //printf("current disk head: %d\n", disk_head);
      //printf("buffer:\n");
      //print_list(new_buffer_head);
      buffer_node* temp_head = new_buffer_head;
      int y;
      buffer_node* lt_list = NULL;
      buffer_node* gt_list = NULL;
      buffer_node* eq_list = NULL;
      buffer_node* next_item = NULL;
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
        temp_head = next_item;
      }
      // printf("less than list:\n");
      // print_list(lt_list);
      // printf("equal to list:\n");
      // print_list(eq_list);
      // printf("greater than list:\n");
      // print_list(gt_list);


      //handle equal to requests
      buffer_node* curr = eq_list;
      //dont change dir if no list
      buffer_node* first_pass = dir == DIR_LEFT ? reverse_list(lt_list) : gt_list;
      buffer_node* second_pass = dir == DIR_LEFT ? gt_list : reverse_list(lt_list);
      process_list(eq_list, dir);
      process_list(first_pass, dir);
      dir = dir == DIR_LEFT ? DIR_RIGHT : DIR_LEFT;
      process_list(second_pass, dir);
    }
  }
}
