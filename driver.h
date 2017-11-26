#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#define SECTORS 100
#define TRACKS 10
#define PLATTERS 1
#define DISK_MOVE_TIME 900 //in usec

int disk [PLATTERS][SECTORS*TRACKS];

// structure of thread
typedef struct _thread_info{
  int tid;
  int sector_number;
  char op_name[11]; 
  int data;
  int arrival_time;
  long int exit_time;
}thread_info;

thread_info *incoming[10001];

typedef struct _buffer_node{
  char op_name[11];
  int sector_number;
  int data;
  struct _buffer_node *next;
}buffer_node;

buffer_node *b_head,*b_tail,*new_buffer_head;

int disk_head;
int buff_count;
int limit;

void ENTER_OPERATION(char *op_name, int sector_number);
void EXIT_OPERATION(char *op_name, int sector_number);
void read_disk(int sector_number);
void write_disk(int sector_number, int data);

void *_thread_handler(void* arg);

void init();

struct timespec globalClock;

void disk_ops(int algo);