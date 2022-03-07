#include <stdlib.h>
#include <stdbool.h>

enum tiposFilas{altaPrioridade, baixaPrioridade, disco, fita, impressora};

typedef struct Process {
  int pid;
  int ppid;
  char* status;
  int t;
  int arrivalTime;
  int burstTime;
  int priority;
  int* io;
  int current_io;
  int t_io;
} Process;

typedef struct Queue {
  Process** queue;
  int front;
  int back;
  int size;
} Queue;

Process* processCreate(int pid, int ppid, int arrivalTime, int burstTime, int* io) {
  Process* p = (Process*) malloc(sizeof(Process));
  p->pid = pid;
  p->ppid = 0;
  p->status = "pronto";
  p->t=0;
  p->arrivalTime = arrivalTime;
  p->burstTime = burstTime;
  p->priority = 0;
  p->io = io;
  p->current_io = 0;
  p->t_io = 0;
  return p;
}

Queue* queueCreate(Process** procs, int size) {
  Queue* q = (Queue*) malloc(sizeof(Queue));
  q->front = 0;
  q->back = size;
  q->size = size;
  q->queue = (Process**) malloc(sizeof(Process*) * 5);
  if(procs == NULL) return q;
  int i;
  for(i = 0; i < size; i++)
    q->queue[i] = procs[i];
  return q;
}


Process* removeProcessInQueue(Queue* q) {
  if(q->size == 0) return NULL;
  q->size--;
  Process* p = q->queue[q->front];
  q->queue[q->front++] = NULL;
  return p;
}

bool addProcessInQueue(Queue* q, Process* p) {
  if(q->size == 5-1 || p==NULL) return 0;
  q->size++;
  q->back++;
  q->queue[q->back-1] = p;
  return 1;
}



char* stringQueue(Queue* q) {

  	char* s = (char*) malloc(sizeof(char) * (5)*2);
	  int s_idx = 0;
	  int i = q->front;
	  s[s_idx++] = '[';
	  s[s_idx++] = ' ';
	  if(q->size != 0) {
	    do {
	      s[s_idx++] = q->queue[i]->pid + '0';
	      s[s_idx++] = ' ';
	     do {
	    		i = (i + 1) % (q->back);
	  	  } while(q->queue[i] == NULL && i != q->front);
   
	    } while(i != q->front);
	  }
	  s[s_idx++] = ']';
	  s[s_idx] = '\0';
	  	  
	  return s;
}


void printQueue(Queue** q) {
	printf("queues: \n\thigh: %s\n\tlow: %s\nio:\n\tdisk: %s\n\tmag_tape: %s\n\tprinter: %s\n\n",
          stringQueue(q[0]),
          stringQueue(q[1]),
          stringQueue(q[2]),
          stringQueue(q[3]),
          stringQueue(q[4]));
}

void executeIo(Queue** queues, Process** process) {
  int i;
  for(i = disco; i <= impressora; i++) {
    Queue* q = queues[i];
    if(q->size == 0) continue;
    Process* p = removeProcessInQueue(q);
    p->t_io--;
    if(p->t_io == 0) {
      switch(p->current_io) {
        case disco:
          p->priority = 1;
          addProcessInQueue(queues[1], p);
          break;
        case fita:
        case impressora:
          p->priority = 0;
          addProcessInQueue(queues[0], p);
          break;
      }
      p->current_io = 0;
      continue;
    }
    addProcessInQueue(q, p);
  }
}


bool processLeft(Queue** queues) {
  int i=0;
  for(i = altaPrioridade; i <= impressora; i++)
    if(queues[i]->size > 0) return true;
  return false;
}









