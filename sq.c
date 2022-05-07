//
//  sq.c
//  Program5
//
//  Created by Ibrahim Qazi on 4/14/22.
//

#include "sq.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int buzzerID; //value of buzzer
    struct node *next;
    struct node *prev;

} NODE;

struct service_queue {
    NODE * availableHead;
    NODE * queueHead;
    NODE * queueTail;
    NODE ** buzzerPointers;
    int queueSize;
    int currLargestBuzzerID;
    int * buzzerInQueue;
};

SQ * sq_create(){
    SQ *queueList = malloc(sizeof(SQ));
    queueList->availableHead = NULL;
    queueList->queueHead = NULL;
    queueList->queueTail = NULL;
    queueList->queueSize = 0;
    queueList->currLargestBuzzerID = 0;
    queueList->buzzerPointers = malloc(1000001*sizeof(NODE*));
    queueList->buzzerInQueue = malloc(1000001*sizeof(int));
    return queueList;
}

void sq_free(SQ *q)
{
    NODE *temp1 = q->availableHead;
    NODE *temp2 = q->queueHead;
    NODE *next1;
    NODE *next2;

    while(temp1 != NULL) {
        next1 = temp1->next;   // keeps us from de-referencing a freed ptr
        free(temp1);
        temp1 = next1;
    }
    
    while(temp2 != NULL) {
        next2 = temp2->next;   // keeps us from de-referencing a freed ptr
        free(temp2);
        temp2 = next2;
    }
    
    free(q->buzzerPointers);
    free(q->buzzerInQueue);
    free(q);
}

void sq_display(SQ *q){
    NODE *temp = q->queueHead;

      printf("[");
      while(temp != NULL) {
        printf(" %i ", temp->buzzerID);
        temp = temp->next;
      }
      printf("]\n");
}

int sq_length(SQ *q){
    return q->queueSize;
}

int sq_give_buzzer(SQ *q){
    if (q->queueHead == NULL)
    {
        if (q->availableHead == NULL)
        {
            NODE * newNode = malloc(sizeof(NODE));
            newNode->buzzerID = q->currLargestBuzzerID;
            newNode->next = NULL;
            newNode->prev = NULL;
            q->queueHead = newNode;
            q->queueTail = newNode;
            q->buzzerPointers[q->currLargestBuzzerID] = newNode;
            q->buzzerInQueue[q->currLargestBuzzerID] = 1;
            q->currLargestBuzzerID++;
            q->queueSize++;
        }
        else
        {
            NODE * tempAvailable;
            tempAvailable = q->availableHead;
            q->availableHead = q->availableHead->next;
            tempAvailable->next = NULL;
            tempAvailable->prev = NULL;
            q->queueTail = tempAvailable;
            q->queueHead = tempAvailable;
            q->buzzerInQueue[q->queueTail->buzzerID] = 1;
            q->queueSize++;
        }
    }
    else if (q->queueHead == q->queueTail)
    {
        if (q->availableHead == NULL)
        {
            NODE * newNode = malloc(sizeof(NODE));
            newNode->buzzerID = q->currLargestBuzzerID;
            newNode->next = NULL;
            newNode->prev = q->queueHead;
            q->queueHead->next = newNode;
            q->buzzerPointers[q->currLargestBuzzerID] = newNode;
            q->buzzerInQueue[q->currLargestBuzzerID] = 1;
            q->currLargestBuzzerID++;
            q->queueSize++;
            q->queueTail = newNode;
        }
        else
        {
            NODE * tempAvailable;
            tempAvailable = q->availableHead;
            q->availableHead = q->availableHead->next;
            tempAvailable->next = NULL;
            tempAvailable->prev = q->queueHead;
            q->queueTail = tempAvailable;
            q->queueHead->next = q->queueTail;
            q->buzzerInQueue[q->queueTail->buzzerID] = 1;
            q->queueSize++;
        }
    }
    else
    {
        if (q->availableHead == NULL)
        {
            NODE * newNode = malloc(sizeof(NODE));
            newNode->buzzerID = q->currLargestBuzzerID;
            newNode->next = NULL;
            NODE * temp = q->queueTail;
            q->queueTail = newNode;
            q->queueTail->prev = temp;
            temp->next = q->queueTail;
            q->buzzerPointers[q->currLargestBuzzerID] = newNode;
            q->buzzerInQueue[q->currLargestBuzzerID] = 1;
            q->currLargestBuzzerID++;
            q->queueSize++;
        }
        else
        {
            NODE * tempAvailable;
            NODE * tempSQTail;
            tempAvailable = q->availableHead;
            q->availableHead = q->availableHead->next;
            tempSQTail = q->queueTail;
            tempAvailable->next = NULL;
            q->queueTail = tempAvailable;
            q->queueTail->prev = tempSQTail;
            tempSQTail->next = q->queueTail;
            q->buzzerInQueue[q->queueTail->buzzerID] = 1;
            q->queueSize++;
        }
    }
    return q->queueTail->buzzerID;
}

int sq_seat(SQ *q)
{
    if (q->queueHead != NULL)
    {
        NODE * temp;
        temp = q->queueHead;
        q->queueHead = q->queueHead->next;
        if (q->queueHead != NULL)
        {
            q->queueHead->prev = NULL;
        }
        temp->next = q->availableHead;
        q->availableHead = temp;
        q->queueSize--;
        q->buzzerInQueue[temp->buzzerID] = 0;
        return temp->buzzerID;
    }
    else
    {
        return -1;
    }
}

int sq_kick_out(SQ *q, int buzzer)
{
    if (q->queueHead != NULL && q->buzzerInQueue[buzzer] == 1)
    {
        if (q->queueHead == q->queueTail)
        {
            if (q->availableHead == NULL)
            {
                q->availableHead = q->queueHead;
            }
            else
            {
                NODE * temp = q->queueHead;
                temp->next = q->availableHead;
                q->availableHead = temp;
            }
            q->queueHead = NULL;
            q->queueTail = NULL;
            q->queueSize--;
            q->buzzerInQueue[buzzer] = 0;
        }
        else
        {
            NODE * temp1;
            temp1 = q->buzzerPointers[buzzer];
            if (temp1->next != NULL)
            {
                temp1->next->prev = temp1->prev;
            }
            
            if (temp1->prev != NULL)
            {
                temp1->prev->next = temp1->next;
            }
            
            if (temp1 == q->queueHead)
            {
                q->queueHead = q->queueHead->next;
            }
            else if (temp1 == q->queueTail)
            {
                q->queueTail = q->queueTail->prev;
            }
            
            if (q->availableHead == NULL)
            {
                q->availableHead = temp1;
                q->availableHead->next = NULL;
                q->availableHead->prev = NULL;
            }
            else
            {
                temp1->prev = NULL;
                temp1->next = q->availableHead;
                q->availableHead = temp1;
            }
            q->queueSize--;
            q->buzzerInQueue[buzzer] = 0;
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

int sq_take_bribe(SQ *q, int buzzer)
{
    if (q->queueHead != NULL && buzzer < q->currLargestBuzzerID && q->buzzerInQueue[buzzer] == 1)
    {
        if (q->queueHead->buzzerID != buzzer)
        {
            if (q->queueTail->buzzerID != buzzer)
            {
                NODE * temp;
                temp = q->buzzerPointers[buzzer];
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
                temp->prev = NULL;
                temp->next = q->queueHead;
                q->queueHead->prev = temp;
                q->queueHead = temp;
            }
            else
            {
                NODE * temp;
                temp = q->queueTail;
                q->queueTail = q->queueTail->prev;
                q->queueTail->next = NULL;
                temp->prev = NULL;
                temp->next = q->queueHead;
                q->queueHead->prev = temp;
                q->queueHead = temp;
            }
        }
        return 1;
    }
    else
    {
        return 0;
    }
}
