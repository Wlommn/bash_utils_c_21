#include "./linkedList.h"

LinkedList* InitList() {
  LinkedList* newList = malloc(sizeof(LinkedList));
  newList->id = 0;
  newList->next = NULL;
  newList->value = NULL;
  return newList;
}

LinkedList* CreateNewListNode(char* newNodeValue, int newNodeId) {
  LinkedList* newNode = malloc(sizeof(LinkedList));
  newNode->next = NULL;
  newNode->id = newNodeId;
  newNode->value = DublicateString(newNodeValue);
  return newNode;
}

void AddEnd(LinkedList* list, char* value) {
  if (list != NULL) {
    LinkedList* current = list;
    while (current->next != NULL) {
      current = current->next;
    }
    LinkedList* newNode = CreateNewListNode(value, current->id + 1);
    current->next = newNode;
  }
}

void DeleteListNode(LinkedList* listNode) {
  if (listNode != NULL) {
    free(listNode->value);
    free(listNode);
  }
}

void FreeLinkedList(LinkedList* list) {
  if (list != NULL) {
    while (list->next != NULL) {
      LinkedList* prev = list;
      list = list->next;
      DeleteListNode(prev);
    }
    DeleteListNode(list);
  }
}

char* DublicateString(char* string) {
  char* result = NULL;
  result = malloc(strlen(string) + 1);
  if (result != NULL) {
    result = strcpy(result, string);
  }
  return result;
}

int GetListSize(LinkedList* list) {
  int count = 0;
  if (list != NULL) {
    LinkedList* current = list;
    count++;
    while (current->next != NULL) {
      count++;
      current = current->next;
    }
  }
  return count;
}
