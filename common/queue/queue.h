#ifndef COMMON_QUEUE_QUEUE_H_
#define COMMON_QUEUE_QUEUE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
  char value;
  struct Node* next;
} Node;

typedef struct Queue {
  Node* head;
  Node* tail;
  int size;
} Queue;

Queue* InitQueue();

void EnqueuePriority(Queue* queue, char value);
void Enqueue(Queue* queue, char value);
void EnqueueBeforeNode(Queue* queue, Node* node, char value);

char Dequeue(Queue* queue);
void FreeQueue(Queue* queue);

int GetQueueSize(Queue* queue);
void PrintQueue(Queue* queue);

Node* CreateQueueNode(char newNodeValue);
Node* NodeMemmoryAllocate();

#endif  //  COMMON_QUEUE_QUEUE_H_
