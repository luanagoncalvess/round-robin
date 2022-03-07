#include <stdlib.h>
#include <stdbool.h>

enum tiposFilas{altaPrioridade, baixaPrioridade, disco, fita, impressora};

// Struct que simula um processo
typedef struct Process {
  int pid;
  int ppid;
  char* status;
  int t;
  int arrivalTime; // Instante de chegada do processo (ou instante de ativação)
  int burstTime; // Tempo de serviço do processo
  int priority;
  int* io; // Vetor de tamanho <burst_time> que armazena o tipo de I/O do processo no index correspondente ao instante de tempo em que a operação é excutada
  int current_io; // Tipo de operação I/O atual (valor é 0 caso não haja tipo)
  int t_io; // Tempo restante para a operação I/O atual
} Process;

// Fila (FIFO) dos ponteiros de <Process>
typedef struct Queue {
  Process** queue; // Vetor de ponteiros de <Process>, de tamanho <size>
  int front; // Index do primeiro elemento da fila
  int back; // Index do último elemento da fila
  int size; // Número de elementos na fila
} Queue;

// Cria um novo processo com argumentos como campos. Gera como saída um ponteiro para o <Process> criado
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

// Cria uma nova fila com argumentos como campos. Gera como saída uma fila
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

// Adiciona processo à fila
bool addProcessInQueue(Queue* q, Process* p) {
  if(q->size == 5-1 || p==NULL) return 0;
  q->size++;
  q->back++;
  q->queue[q->back-1] = p;
  return 1;
}

// Remove processo da fila
Process* removeProcessInQueue(Queue* q) {
  if(q->size == 0) return NULL;
  q->size--;
  Process* p = q->queue[q->front];
  q->queue[q->front++] = NULL;
  return p;
}

// Retorna uma fila como string
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

// Imprime a fila
void printQueue(Queue** q) {
	printf("Filas: \n\talta prioridade: %s\n\tbaixa prioridade: %s\nI/O:\n\tdisco: %s\n\tfita mag.: %s\n\timpressora: %s\n\n",
          stringQueue(q[0]),
          stringQueue(q[1]),
          stringQueue(q[2]),
          stringQueue(q[3]),
          stringQueue(q[4]));
}

// Executa a operação I/O em uma unidade de tempo
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

// Verfifca se ainda existem processos a serem rodados
bool processLeft(Queue** queues) {
  int i=0;
  for(i = altaPrioridade; i <= impressora; i++)
    if(queues[i]->size > 0) return true;
  return false;
}
