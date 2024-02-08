#include "./s21_cat.h"

int main(int argCount, char **argValue) {
  CatData *data = InitCatData(argCount, argValue);
  int returnCode = 0;

  if (argCount > 1) {
    ParseFilesPaths(data);
    ParseOptions(data);
  } else {
    PRINT_ERR_ARGUMENTS(data);
  }

  if (!data->config->err) {
    Cat(data);
  }

  returnCode = data->exitValue;
  FreeCatData(data);
  return returnCode;
}

CatData *InitCatData(int argCount, char **argValue) {
  CatData *data = malloc(sizeof(CatData));
  data->exitValue = 0;
  data->scriptName = DublicateString(argValue[0]);
  data->argCount = argCount;
  data->argValue = argValue;
  data->countNum = 1;
  data->currentFile = NULL;
  data->prevFreeLine = false;
  data->config = InitConfig();
  data->filePaths = InitList();
  return data;
}

ActiveFlags *InitConfig() {
  ActiveFlags *activeFlags = malloc(sizeof(ActiveFlags));
  activeFlags->b = false;
  activeFlags->e = false;
  activeFlags->err = false;
  activeFlags->n = false;
  activeFlags->s = false;
  activeFlags->t = false;
  activeFlags->v = false;
  return activeFlags;
}

void FreeCatData(CatData *data) {
  FreeLinkedList(data->filePaths);
  free(data->scriptName);
  free(data->config);
  free(data);
}

void ParseFilesPaths(CatData *data) {
  for (int i = 1; i < data->argCount; i++) {
    if (data->argValue[i][0] != '-') {
      AddEnd(data->filePaths, data->argValue[i]);
    }
  }
}

void ParseOptions(CatData *data) {
  int optIndex = 0;
  char flag = '\0';
  opterr = 0;
  struct option longOptions[] = {{"number", 0, NULL, 'n'},
                                 {"number-nonblank", 0, NULL, 'b'},
                                 {"squeeze-blank", 0, NULL, 's'},
                                 {"show-nonprinting", 0, NULL, 'v'},
                                 {0, 0, 0, 0}};
  while (!data->config->err &&
         ((flag = getopt_long(data->argCount, data->argValue, "sTtbEenv",
                              longOptions, &optIndex)) != -1)) {
    SetActiveFlags(data, flag);
  }
}

void SetActiveFlags(CatData *data, char parsedFlags) {
  switch (parsedFlags) {
    case 'b':
      data->config->b = true;
      break;
    case 'E':
      data->config->e = true;
      break;
    case 'e':
      data->config->v = true;
      data->config->e = true;
      break;
    case 'n':
      data->config->n = true;
      break;
    case 's':
      data->config->s = true;
      break;
    case 'T':
      data->config->t = true;
      break;
    case 't':
      data->config->v = true;
      data->config->t = true;
      break;
    case 'v':
      data->config->v = true;
      break;
    case '?':
      data->config->err = true;
      PRINT_ERR_NOT_CORRECT_OPTION(data);
      break;
    default:
      break;
  }
}

void Cat(CatData *data) {
  LinkedList *currentFilePath = data->filePaths->next;
  FILE *file = NULL;
  while (currentFilePath != NULL) {
    data->currentFile = currentFilePath->value;
    if ((file = fopen(data->currentFile, "r")) != NULL) {
      if (CheckIsFile(data)) {
        WriteFileText(data, file);
      }
      fclose(file);
    } else {
      PRINT_ERR_FILE(data);
    }
    currentFilePath = currentFilePath->next;
  }
}

bool CheckIsFile(CatData *data) {
  bool result = false;
  struct stat st_buf;
  if (stat(data->currentFile, &st_buf) == 0) {
    if (S_ISREG(st_buf.st_mode)) {
      result = true;
    } else if (S_ISDIR(st_buf.st_mode)) {
      PRINT_ERR_IS_DIR(data);
    } else {
      PRINT_ERR_IS_NOT_CORRECT_FILE(data);
    }
  } else if (errno == ENOENT) {
    PRINT_ERR_FILE(data);
  }
  return result;
}

void Handler_B(CatData *data, Queue *buffer) {
  if (buffer->size > 1) {
    printf("%6d\t", data->countNum);
    data->countNum++;
  }
}

void Handler_E(Queue *buffer) {
  if (buffer->size != 0) {
    Node *newTail = CreateQueueNode('\n');
    buffer->tail->value = '$';
    buffer->tail->next = newTail;
    buffer->size++;
  }
}

void Handler_V(Queue *buffer) {
  Node *check = buffer->head;
  while (check != NULL) {
    int currentValue = check->value;
    if (currentValue == '\n' || currentValue == '\t') {
    } else if (currentValue < 32 && currentValue >= 0) {
      check->value = currentValue + 64;
    } else if (currentValue >= -128 && currentValue < -96) {
      check->value = currentValue + 192;
    } else if (currentValue >= -96 && currentValue < -1) {
      check->value = currentValue + 128;
    } else if (currentValue == 127 || currentValue == -1) {
      check->value = '?';
    }
    SetAll_V_Markers(buffer, check, currentValue);
    check = check->next;
  }
}

void SetAll_V_Markers(Queue *buffer, Node *node, int currentValue) {
  if (currentValue == '\n' || currentValue == '\t') {
  } else if ((currentValue < 32 && currentValue >= 0) || currentValue == 127) {
    Add_V_Marker(buffer, node, "^");
  } else if ((currentValue >= -128 && currentValue < -96) ||
             currentValue == -1) {
    Add_V_Marker(buffer, node, "M-^");
  } else if (currentValue >= -96 && currentValue < -1) {
    Add_V_Marker(buffer, node, "M-");
  }
}

void Add_V_Marker(Queue *buffer, Node *node, char *string) {
  int length = strlen(string);
  for (int i = 0; i < length; i++) {
    EnqueueBeforeNode(buffer, node, string[i]);
  }
}

void Handler_N(CatData *data, Queue *buffer) {
  if (buffer->size != 0) {
    printf("%6d\t", data->countNum);
    data->countNum++;
  }
}

void Handler_S(CatData *data, Queue *buffer, FILE *file) {
  if (data->prevFreeLine && CheckIsEmtyLine(buffer)) {
    Dequeue(buffer);
    while (!TakeLineInBuffer(buffer, file) && CheckIsEmtyLine(buffer)) {
      Dequeue(buffer);
    }
  }
  if (CheckIsEmtyLine(buffer)) {
    data->prevFreeLine = true;
  } else {
    data->prevFreeLine = false;
  }
}

bool CheckIsEmtyLine(Queue *buffer) {
  bool result = false;
  if (buffer->size == 1 && buffer->head->value == '\n') {
    result = true;
  }
  return result;
}

void Handler_T(Queue *buffer) {
  Node *check = buffer->head;
  while (check != NULL) {
    if (check->value == '\t') {
      check->value = 'I';
      EnqueueBeforeNode(buffer, check, '^');
    }
    check = check->next;
  }
}

bool TakeLineInBuffer(Queue *buffer, FILE *file) {
  bool result = false;
  char ch;
  while ((ch = fgetc(file)) != '\n' && ch != EOF) {
    Enqueue(buffer, ch);
  }
  if (ch == '\n') {
    Enqueue(buffer, ch);
  }
  if (ch == EOF) {
    if (feof(file)) {
      result = true;
    } else {
      Enqueue(buffer, ch);
      result = TakeLineInBuffer(buffer, file);
    }
  }
  return result;
}

void WriteFileText(CatData *data, FILE *file) {
  Queue *buffer = InitQueue();
  while (!TakeLineInBuffer(buffer, file)) {
    WriteLine(data, buffer, file);
  }
  WriteLine(data, buffer, file);
  FreeQueue(buffer);
}

void WriteLine(CatData *data, Queue *buffer, FILE *file) {
  if (buffer->size > 0) {
    ModifyLine(data, buffer, file);
    while (buffer->size > 0) {
      printf("%c", Dequeue(buffer));
    }
  }
}

void ModifyLine(CatData *data, Queue *buffer, FILE *file) {
  if (data->config->s) {
    Handler_S(data, buffer, file);
  }
  if (data->config->v) {
    Handler_V(buffer);
  }
  if (data->config->t) {
    Handler_T(buffer);
  }
  if (data->config->b && !data->config->n) {
    Handler_B(data, buffer);
  }
  if (data->config->e) {
    Handler_E(buffer);
  }
  if (data->config->n && !data->config->b) {
    Handler_N(data, buffer);
  }
}

void PRINT_ERR_NOT_CORRECT_OPTION(CatData *data) {
  printf("%s: Not correct option - \"%c\"\n", data->scriptName, (char)optopt);
  data->exitValue = 1;
}

void PRINT_ERR_ARGUMENTS(CatData *data) {
  printf("Use: %s [OPTIONS] [FILE]\n", data->scriptName);
  data->exitValue = 1;
}

void PRINT_ERR_IS_NOT_CORRECT_FILE(CatData *data) {
  printf("%s: No correct file: \"%s\"", data->scriptName, data->currentFile);
  data->exitValue = 1;
}

void PRINT_ERR_FILE(CatData *data) {
  printf("%s: %s: No such file or directory\n", data->scriptName,
         data->currentFile);
  data->exitValue = 1;
}

void PRINT_ERR_IS_DIR(CatData *data) {
  printf("%s: %s: Is a directory\n", data->scriptName, data->currentFile);
  data->exitValue = 1;
}
