#ifndef __QUEUE__H_
#define __QUEUE__H_

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#define MAX_SIZE 100


typedef struct {

    void *queue[MAX_SIZE];
	int head;
	int tail;
	int size;

} Queue;


Queue *createQueue();

void push(Queue *q, void *v);

void pop(Queue *q);

void remove(Queue *p, void *v);

int isEmpty(const Queue *q);

void *getHead(Queue *q);

void *getTail(Queue *q);

int getSize(Queue *q);

void freeQueue(Queue **q);

#endif
