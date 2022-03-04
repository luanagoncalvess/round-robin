#include <stdio.h>

struct processo
{
    int pid;
    int arrival_time, burst_time, waiting_time, turn_around_time, remaining_time, CT;
} Q1[10], Q2[10], Q3[10]; /* Três filas: alta prioridade, baixa prioridade e I/O */

int main()
{

    int i = 0, j, k = 0, r = 0, time = 0, tq1 = 5, tq2 = 5, flag = 0, n;

    printf("Insira o número de processos: ");
    scanf("%d", &n);

    while (i < n)
    {
        printf("\nInsira o arrival time e burst time do processo %c: ", Q1[i].pid);
        scanf("%d%d", &Q1[i].arrival_time, &Q1[i].burst_time);
        Q1[i].pid = i;
        Q1[i].remaining_time = Q1[i].burst_time; /* guardamos o burst time no remaining time para cada processo */
        i++;
    }

    struct processo temp;
    for (i = 0; i < n; i++)
    {
        for (j = i + 1; j < n; j++)
        {
            if (Q1[i].arrival_time > Q1[j].arrival_time)
            {
                temp = Q1[i];
                Q1[i] = Q1[j];
                Q1[j] = temp;
            }
        }
    }

    time = Q1[0].arrival_time;

    printf("\nProcessos na primeira fila seguindo RR com quantum = 5");
    printf("\nProcesso\tRemaining Time\tWaiting Time\tTurn Around Time\t\t");

    for (i = 0; i < n; i++)
    {
        if (Q1[i].remaining_time <= tq1)
        {
            time += Q1[i].remaining_time; /* desde o arrival time do 1º processo até que esse processo seja finalizado */
            Q1[i].remaining_time = 0;
            Q1[i].waiting_time = time - Q1[i].arrival_time - Q1[i].burst_time; /* tempo que o processo esteve esperando na 1ª fila */
            Q1[i].turn_around_time = time - Q1[i].arrival_time;                /* tempo para executar o processo */
            printf("\n%d\t\t%d\t\t%d\t\t%d", Q1[i].pid, Q1[i].burst_time, Q1[i].waiting_time, Q1[i].turn_around_time);
        }
        else
        { /* processo é movido para a 2ª fila (baixa prioridade) com quantum = 5 */
            Q2[k].waiting_time = time;
            time += tq1;
            Q1[i].remaining_time -= tq1;
            Q2[k].burst_time = Q1[i].remaining_time;
            Q2[k].remaining_time = Q2[k].burst_time;
            Q2[k].pid = Q1[i].pid;
            k = k + 1;
            flag = 1;
        }
    }

    if (flag == 1)
    {
        printf("\n\nProcessos na segunda fila seguindo RR com quantum = 5");
        printf("\nProcesso\tRemaining Time\tWaiting Time\tTurn Around Time\t\t");
    }

    for (i = 0; i < k; i++)
    {
        if (Q2[i].remaining_time <= tq2)
        {
            time += Q2[i].remaining_time; /* desde o arrival time do 1º processo + burst time desse processo*/
            Q2[i].remaining_time = 0;
            Q2[i].waiting_time = time - tq1 - Q2[i].burst_time; /* tempo que o processo esteve esperando na fila de pronto */
            Q2[i].turn_around_time = time - Q2[i].arrival_time; /* tempo para executar o processo */
            printf("\n%d\t\t%d\t\t%d\t\t%d", Q2[i].pid, Q2[i].burst_time, Q2[i].waiting_time, Q2[i].turn_around_time);
        }
        else
        { /* processo é movido para a 3ª fila (I/O) com FCFS */
            Q3[r].arrival_time = time;
            time += tq2;
            Q2[i].remaining_time -= tq2;
            Q3[r].burst_time = Q2[i].remaining_time;
            Q3[r].remaining_time = Q3[r].burst_time;
            Q3[r].pid = Q2[i].pid;
            r = r + 1;
            flag = 2;
        }
    }

    if (flag == 2)
    {
        printf("\n\nProcessos na terceira fila seguindo FCFS ");
    }

    for (i = 0; i < r; i++)
    {
        if (i == 0)
        {
            Q3[i].CT = Q3[i].burst_time + time - tq1 - tq2;
        }
        else
        {
            Q3[i].CT = Q3[i - 1].CT + Q3[i].burst_time;
        }
    }

    for (i = 0; i < r; i++)
    {
        Q3[i].turn_around_time = Q3[i].CT;
        Q3[i].waiting_time = Q3[i].turn_around_time - Q3[i].burst_time;
        printf("\n%d\t\t%d\t\t%d\t\t%d\t\t", Q3[i].pid, Q3[i].burst_time, Q3[i].waiting_time, Q3[i].turn_around_time);
    }
}
