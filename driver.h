#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define SECTORS 10000
#define TRACKS 100
#define PLATTERS 1
#define AVG_ROT_DELAY 2000 //in usec

int disk [SECTORS*TRACKS];

// structure of thread
typedef struct _thread_info{
  int tid;
  int sector_number;
  char op_name[11];
  int data;
  int arrival_time;
  struct timespec exit_time;
}thread_info;

thread_info *incoming[10001];
pthread_t disk_thread;
pthread_mutex_t lock;
pthread_mutex_t req_lock;

typedef struct _buffer_node{
  int req_id;
  char op_name[11];
  int sector_number;
  int data;
  struct _buffer_node *next;
}buffer_node;

buffer_node *b_head,*b_tail,*new_buffer_head;

int disk_head;
int buff_count;
int limit;
int should_run;
int n,num_request_served;

void ENTER_OPERATION(char *op_name, int sector_number);
void EXIT_OPERATION(char *op_name, int sector_number);
void read_disk(int sector_number);
void write_disk(int sector_number, int data);

void *_thread_handler(void* arg);
void *disk_ops(void *arg);
int sectorToTrack(int);
void init();

struct timespec globalClock;
