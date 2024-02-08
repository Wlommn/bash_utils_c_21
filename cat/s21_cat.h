#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../common/bool.h"
#include "../common/linkedList/linkedList.h"
#include "../common/queue/queue.h"

typedef struct ActiveFlags {
  bool b;
  bool e;
  bool n;
  bool s;
  bool t;
  bool v;
  bool err;
} ActiveFlags;

typedef struct CatData {
  int exitValue;
  char *scriptName;
  int argCount;
  char **argValue;
  int countNum;
  char *currentFile;
  bool prevFreeLine;
  ActiveFlags *config;
  LinkedList *filePaths;
} CatData;

CatData *InitCatData(int argCount, char **argValue);
ActiveFlags *InitConfig();
void FreeCatData(CatData *data);

void ParseFilesPaths(CatData *data);
void ParseOptions(CatData *data);
void SetActiveFlags(CatData *data, char parsedFlags);

void Cat(CatData *data);
bool CheckIsFile(CatData *data);

void Handler_B(CatData *data, Queue *buffer);

void Handler_E(Queue *buffer);

void Handler_V(Queue *buffer);
void SetAll_V_Markers(Queue *buffer, Node *node, int currentValue);
void Add_V_Marker(Queue *buffer, Node *node, char *string);

void Handler_N(CatData *data, Queue *buffer);

void Handler_S(CatData *data, Queue *buffer, FILE *file);
bool CheckIsEmtyLine(Queue *buffer);

void Handler_T(Queue *buffer);

bool TakeLineInBuffer(Queue *buffer, FILE *file);
void WriteFileText(CatData *data, FILE *file);
void WriteLine(CatData *data, Queue *buffer, FILE *file);
void ModifyLine(CatData *data, Queue *buffer, FILE *file);

void PRINT_ERR_NOT_CORRECT_OPTION(CatData *data);
void PRINT_ERR_ARGUMENTS(CatData *data);
void PRINT_ERR_IS_NOT_CORRECT_FILE(CatData *data);
void PRINT_ERR_FILE(CatData *data);
void PRINT_ERR_IS_DIR(CatData *data);

#endif  //  SRC_CAT_S21_CAT_H_
