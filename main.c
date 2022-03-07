#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

#define TOTAL_PROCESSOS 5
#define QUANTUM 3

int t=0;

// Executa o próximo processo dado um queueIndex. Retorna "true" caso seja executado e "false" caso haja erro
bool executeProcess(Queue** q, Process** po, int queueIndex) {
   if(queueIndex == -1) {
    printQueue(q);
    int i;
    for(i = 0; i < QUANTUM; i++) {
      executeIo(q,po);
      t++;
    }
    return true;
  }

  printQueue(q);

  Process* p = removeProcessInQueue(q[queueIndex]);
  p->status = "executando";
  bool went_io = false;
  if(p->priority < 1) p->priority++;
  int i;
  for(i = 0; i < QUANTUM; i++) {
    if(p->current_io != 0) {
      executeIo(q,po);
      continue;
    }
    // Checa se existe operação de I/O
    if(p->io[p->t] != -1 && p->t < p->burstTime) {
      p->status = "aguardando";
      p->t_io = 4;
      p->current_io = p->io[p->t];
      addProcessInQueue(q[p->io[p->t]], p);
      went_io = true;
      p->t++;
    }

    if(p->t + 1 <= p->burstTime && p->current_io == 0) p->t++;

    executeIo(q,po);
    t++;
  }
  printf("Tempo de serviço restante: %3d\n\n",p->burstTime - p->t);

  // Caso termine de executar
  if(p->t == p->burstTime) return true;

  if(!went_io && p->current_io == 0 && !addProcessInQueue(q[p->priority], p)) {
    // Caso um erro seja encontrado
    printf("Erro: não é possível adicionar à fila %d!\n", queueIndex);
    return false;
  }
  if(p->current_io == 0) p->status = "pronto";
  return true;

}




int main(){

    //cria��o dos processos
    Process* processos[TOTAL_PROCESSOS];
    int i=0;
    for(i=0; i<TOTAL_PROCESSOS; i++){
    	//int pid, int ppid, int arrivalTime, int burstTime, int* io
        processos[i] = processCreate(i, 0, 0, 4, 0);
    }

    //cria��o dos tipos de filas
    enum tiposFilas{altaPrioridade, baixaPrioridade, disco, fita, impressora};

    Queue* filas[5];

    for(i=altaPrioridade;i<=impressora;i++){
    	// Process** procs, int size
        filas[i] = queueCreate(NULL,0);
    }

    //Simulation - scheduler
    do{

	    bool executed = false;

	    for(i = 0; i < TOTAL_PROCESSOS; i++){
	    	if(processos[i]->arrivalTime > t-QUANTUM && processos[i]->arrivalTime <= t){
	    		bool a = addProcessInQueue(filas[processos[i]->priority], processos[i]);
			}
		}


		//ainda n�o avaliado
		for(i=altaPrioridade;i<=impressora;i++){
	      if(filas[i]->size == 0) continue;
	      executed = executeProcess(filas,processos,i);
		    break;
	    }
	    if(executed == false) {
	      executed = executeProcess(filas,processos,-1);
	    }

	}while(processLeft(filas));

    return 0;
}
