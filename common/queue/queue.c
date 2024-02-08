#include "queue.h"

Queue* InitQueue() {
  Queue* queue = malloc(sizeof(*queue));
  if (queue != NULL) {
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;
  }
  return queue;
}

void EnqueuePriority(Queue* queue, char value) {
  Node* next = queue->head;
  queue->head = CreateQueueNode(value);
  queue->head->next = next;
  queue->size++;
}

void Enqueue(Queue* queue, char value) {
  Node* prev = queue->tail;
  queue->tail = CreateQueueNode(value);
  if (queue->head == NULL) {
    queue->head = queue->tail;
  } else {
    prev->next = queue->tail;
  }
  queue->size++;
}

void EnqueueBeforeNode(Queue* queue, Node* node, char value) {
  if (node != NULL && queue != NULL) {
    if (node == queue->head) {
      EnqueuePriority(queue, value);
    } else {
      Node* newNode = CreateQueueNode(value);
      Node* check = queue->head;
      while (check->next != node) {
        check = check->next;
      }
      check->next = newNode;
      newNode->next = node;
      queue->size++;
    }
  }
}

char Dequeue(Queue* queue) {
  char value;
  Node* prev = NULL;
  if (queue->size != 0) {
    value = queue->head->value;
    prev = queue->head->next;
    free(queue->head);
    queue->head = prev;
    queue->size--;
  }
  return value;
}

void FreeQueue(Queue* queue) {
  if (queue != NULL) {
    while (queue->size > 0) {
      Dequeue(queue);
    }
    free(queue);
  }
}

int GetQueueSize(Queue* queue) { return queue->size; }

void PrintQueue(Queue* queue) {
  Node* node = queue->head;
  for (int i = 0; i < queue->size; i++) {
    printf("%c", node->value);
    node = node->next;
  }
}

Node* CreateQueueNode(char newNodeValue) {
  Node* newNode = NodeMemmoryAllocate();
  newNode->value = newNodeValue;
  newNode->next = NULL;
  return newNode;
}

Node* NodeMemmoryAllocate() {
  Node* current = malloc(sizeof(Node));
  return current;
}
