#include <n7OS/queue.h>
#include <stdio.h>


Queue *createQueue() {
	Queue *q = malloc(sizeof(Queue));
	if (q == NULL) return NULL;
	q->head = 0;
    q->tail = -1;
	q->size = 0;

	return(q);
}

void push(Queue *q, void *v) {
	q->tail = (q->tail + 1) % MAX_SIZE;
	q->queue[q->tail] = v;
	++(q->size);
}

void pop(Queue *q) {
	q->head = (q->head + 1) % MAX_SIZE;
	--(q->size);
}

static void shiftLeft(Queue *q, int start) {
	for (int i = start; i < q->size - 1; i++) {
		int from_index = (q->head + i + 1) % MAX_SIZE;
		int to_index = (q->head + i) % MAX_SIZE;
		q->queue[to_index] = q->queue[from_index];
	}
}

void remove(Queue *p, void *v) {
	int i;
	for (i = 0; i < p->size; i++) {
		int index = (p->head + i) % MAX_SIZE;
		if (p->queue[index] == v) {
			shiftLeft(p, i);
			p->tail = (p->tail - 1 + MAX_SIZE) % MAX_SIZE;
			--(p->size);
			return;
		}
	}
}

int isEmpty (const Queue *q){
	return q->size == 0;
}

void *getHead(Queue *q) {
	return q->queue[q->head];
}

void *getTail(Queue *q) {
	return q->queue[q->tail];
}

int getSize(Queue *q) {
	return q->size;
}

void freeQueue(Queue **q) {
	if (*q != NULL) {
		free(*q);
		*q = NULL;
	}
}
