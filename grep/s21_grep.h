#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <errno.h>
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../common/bool.h"
#include "../common/linkedList/linkedList.h"

typedef struct ActiveFlags {
  bool i;
  bool v;
  bool c;
  bool l;
  bool n;
  bool h;
  bool s;
  bool o;
  bool err;
} ActiveFlags;

typedef struct LineBuffer {
  char line[BUFSIZ];
  int size;
} LineBuffer;

typedef struct GrepData {
  int exitValue;
  char *scriptName;
  int argCount;
  char **argValue;
  ActiveFlags *config;
  LinkedList *patterns;
  LinkedList *filePaths;
  LinkedList *f_patterns;
  int countLine;
  int matched;
  bool find;
  bool isEmptyPattern;
  regex_t preg;
  char *currentFile;
} GrepData;

GrepData *InitGrepData(int argCount, char **argValue);
ActiveFlags *InitConfig();
bool CheckArgumentsParse(GrepData *data);
void FreeGrepData(GrepData *data);

void ParseOption(GrepData *data);
void SetActiveFlags(GrepData *data, char parsedFlag);

int TakePattern(GrepData *data);
void TakeFilesPaths(GrepData *data, int index);
void TakeFilePattern(GrepData *data, char *filePath);

bool CheckIsFile(GrepData *data);

void Grep(GrepData *data);
void Scan(GrepData *data);

void CompileRegex(GrepData *data, char *pattern);
void SearchMatches(GrepData *data, char *line);
void SearchCurrentPattern(GrepData *data, char *line);
void PrintResult(GrepData *data, char *line);

void GetLine(FILE *file, LineBuffer *buffer);
void FreeBuffer(LineBuffer *buffer);

void Handler_C(GrepData *data);
void Handler_L(GrepData *data);
void Handler_N(GrepData *data);
void Handler_H(GrepData *data);

void Handler_F(GrepData *data, char *line);
void F_Scan(GrepData *data, char *line, FILE *patternFile);

void Handler_O(GrepData *data, regmatch_t *pmatch, char *line);

void PRINT_ERR_NOT_CORRECT_OPTION(GrepData *data);
void PRINT_ERR_IS_NOT_CORRECT_FILE(GrepData *data);
void PRINT_ERR_ARGUMENTS(GrepData *data);
void PRINT_ERR_FILE(GrepData *data);
void PRINT_ERR_IS_DIR(GrepData *data);

#endif  //  SRC_GREP_S21_GREP_H_
