#ifndef COMMON_LINKEDLIST_LINKEDLIST_H_
#define COMMON_LINKEDLIST_LINKEDLIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct LinkedList {
  struct LinkedList* next;
  char* value;
  int id;
} LinkedList;

LinkedList* InitList();

LinkedList* CreateNewListNode(char* newNodeValue, int newNodeId);

void AddEnd(LinkedList* list, char* value);

void DeleteListNode(LinkedList* listNode);
void FreeLinkedList(LinkedList* list);

char* DublicateString(char* string);
int GetListSize(LinkedList* list);

#endif  //  COMMON_LINKEDLIST_LINKEDLIST_H_
