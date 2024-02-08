#include "./s21_grep.h"

int main(int argCount, char **argValue) {
  GrepData *data = InitGrepData(argCount, argValue);
  int returnCode = 0;
  if (argCount > 1) {
    ParseOption(data);
    int startIndex = TakePattern(data);
    TakeFilesPaths(data, startIndex);
  }

  if (CheckArgumentsParse(data) && !data->config->err) {
    Grep(data);
  }

  returnCode = data->exitValue;
  FreeGrepData(data);
  return returnCode;
}

GrepData *InitGrepData(int argCount, char **argValue) {
  GrepData *data = malloc(sizeof(GrepData));
  data->exitValue = 0;
  data->scriptName = DublicateString(argValue[0]);
  data->argCount = argCount;
  data->argValue = argValue;
  data->config = InitConfig();
  data->filePaths = InitList();
  data->patterns = InitList();
  data->f_patterns = InitList();
  data->countLine = 1;
  data->matched = 0;
  data->find = false;
  data->isEmptyPattern = false;
  data->currentFile = NULL;
  return data;
}

ActiveFlags *InitConfig() {
  ActiveFlags *result = malloc(sizeof(ActiveFlags));
  result->c = false;
  result->err = false;
  result->h = true;
  result->i = false;
  result->l = false;
  result->n = false;
  result->o = false;
  result->s = false;
  result->v = false;
  return result;
}

bool CheckArgumentsParse(GrepData *data) {
  bool result = false;
  if (GetListSize(data->filePaths) > 1 &&
      (GetListSize(data->patterns) > 1 || GetListSize(data->f_patterns) > 1)) {
    result = true;
  } else if (!data->config->err) {
    PRINT_ERR_ARGUMENTS(data);
  }
  return result;
}

void FreeGrepData(GrepData *data) {
  FreeLinkedList(data->filePaths);
  FreeLinkedList(data->patterns);
  FreeLinkedList(data->f_patterns);
  free(data->scriptName);
  free(data->config);
  free(data);
}

void ParseOption(GrepData *data) {
  char flag = '\0';
  opterr = 0;
  while (!data->config->err && (flag = getopt(data->argCount, data->argValue,
                                              "e:f:ivclnhso")) != -1) {
    SetActiveFlags(data, flag);
  }
}

void SetActiveFlags(GrepData *data, char parsedFlag) {
  switch (parsedFlag) {
    case 'e':
      AddEnd(data->patterns, optarg);
      break;
    case 'f':
      TakeFilePattern(data, optarg);
      break;
    case 'i':
      data->config->i = true;
      break;
    case 'v':
      data->config->v = true;
      break;
    case 'c':
      data->config->c = true;
      break;
    case 'l':
      data->config->l = true;
      break;
    case 'n':
      data->config->n = true;
      break;
    case 'h':
      data->config->h = false;
      break;
    case 's':
      data->config->s = true;
      break;
    case 'o':
      data->config->o = true;
      break;
    case '?':
      data->config->err = true;
      PRINT_ERR_NOT_CORRECT_OPTION(data);
      break;
    default:
      break;
  }
}

int TakePattern(GrepData *data) {
  int i = 1;
  if (GetListSize(data->patterns) == 1 &&
      !(GetListSize(data->f_patterns) > 1)) {
    bool breakPoint = false;
    while (i < data->argCount && !breakPoint) {
      if (data->argValue[i][0] != '-') {
        AddEnd(data->patterns, data->argValue[i]);
        breakPoint = true;
      }
      i++;
    }
  }
  return i;
}

void TakeFilesPaths(GrepData *data, int index) {
  while (index < data->argCount) {
    if (data->argValue[index][0] != '-' &&
        strcmp(data->argValue[index - 1], "-e") != 0 &&
        strcmp(data->argValue[index - 1], "-f") != 0) {
      AddEnd(data->filePaths, data->argValue[index]);
    }
    index++;
  }
  if (GetListSize(data->filePaths) <= 2) {
    data->config->h = false;
  }
}

void TakeFilePattern(GrepData *data, char *filePath) {
  data->currentFile = filePath;
  if (CheckIsFile(data)) {
    AddEnd(data->f_patterns, filePath);
  } else {
    data->config->err = true;
  }
}

bool CheckIsFile(GrepData *data) {
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

void Grep(GrepData *data) {
  LinkedList *currentFile = data->filePaths->next;
  while (currentFile != NULL) {
    data->currentFile = currentFile->value;
    if (CheckIsFile(data)) {
      Scan(data);
      if (data->find) {
        Handler_L(data);
      }
      Handler_C(data);
    }
    currentFile = currentFile->next;
  }
}

void Scan(GrepData *data) {
  LineBuffer *buffer = malloc(sizeof(LineBuffer));
  buffer->size = 0;
  FILE *file = NULL;
  if ((file = fopen(data->currentFile, "r")) != NULL) {
    while (!feof(file)) {
      GetLine(file, buffer);
      if (buffer->size == 1 && feof(file)) {
      } else {
        SearchMatches(data, buffer->line);
        Handler_F(data, buffer->line);
        if (!data->config->o) {
          PrintResult(data, buffer->line);
        }
        if (!data->config->l && !data->config->c) {
          data->find = false;
        }
        data->countLine++;
      }
      FreeBuffer(buffer);
    }
    data->countLine = 1;
    fclose(file);
  }
  free(buffer);
}

void CompileRegex(GrepData *data, char *pattern) {
  int cflags = 0;
  if (strlen(pattern) == 0) {
    data->isEmptyPattern = true;
  }
  if (data->config->i) {
    cflags = REG_ICASE;
  }
  regcomp(&data->preg, pattern, cflags);
}

void SearchMatches(GrepData *data, char *line) {
  if (GetListSize(data->patterns) > 1) {
    LinkedList *currentPattern = data->patterns->next;
    while (currentPattern != NULL) {
      CompileRegex(data, currentPattern->value);
      SearchCurrentPattern(data, line);
      currentPattern = currentPattern->next;
      regfree(&data->preg);
    }
  }
}

void SearchCurrentPattern(GrepData *data, char *line) {
  if (line != NULL) {
    regmatch_t pmatch;
    int match = -1;
    match = regexec(&data->preg, line, 1, &pmatch, 0);
    if (match == 0) {
      if (!data->find && !data->config->v) {
        data->find = true;
      }
      data->matched++;
      Handler_O(data, &pmatch, line);
    } else if (data->config->v) {
      if (!data->find) {
        data->find = true;
      }
    }
    data->isEmptyPattern = false;
  }
}

void PrintResult(GrepData *data, char *line) {
  if (data->find && !data->config->l && !data->config->c) {
    Handler_H(data);
    Handler_N(data);
    printf("%s\n", line);
  } else if (data->config->v && strlen(line) == 0) {
    printf("\n");
  }
}

void GetLine(FILE *file, LineBuffer *buffer) {
  char ch = '\0';
  while ((ch = fgetc(file)) != '\n' && ch != EOF && buffer->size < BUFSIZ) {
    buffer->line[buffer->size] = ch;
    buffer->size++;
  }
  buffer->line[buffer->size] = '\0';
  buffer->size++;
}

void FreeBuffer(LineBuffer *buffer) {
  for (int i = buffer->size; i >= 0; i--) {
    buffer->line[i] = '\0';
  }
  buffer->size = 0;
}

void Handler_C(GrepData *data) {
  if (data->config->c && !data->config->l) {
    Handler_H(data);
    printf("%d\n", data->matched);
    data->matched = 0;
  }
}

void Handler_L(GrepData *data) {
  if (data->config->l) {
    printf("%s\n", data->currentFile);
  }
}

void Handler_N(GrepData *data) {
  if (data->config->n) {
    printf("%d:", data->countLine);
  }
}

void Handler_H(GrepData *data) {
  if (data->config->h) {
    printf("%s:", data->currentFile);
  }
}

void Handler_F(GrepData *data, char *line) {
  if (GetListSize(data->f_patterns) > 1) {
    LinkedList *currentF_patterns = data->f_patterns->next;
    FILE *patternFile = NULL;
    while (currentF_patterns != NULL) {
      char *patternPath = currentF_patterns->value;
      if ((patternFile = fopen(patternPath, "r")) != NULL) {
        F_Scan(data, line, patternFile);
        fclose(patternFile);
      }
      currentF_patterns = currentF_patterns->next;
    }
  }
}

void F_Scan(GrepData *data, char *line, FILE *patternFile) {
  LineBuffer *f_buffer = malloc(sizeof(LineBuffer));
  f_buffer->size = 0;
  while (!feof(patternFile)) {
    GetLine(patternFile, f_buffer);
    if (f_buffer->size == 1 && feof(patternFile)) {
    } else {
      CompileRegex(data, f_buffer->line);
      SearchCurrentPattern(data, line);
      regfree(&data->preg);
    }
    FreeBuffer(f_buffer);
  }
  free(f_buffer);
}

void Handler_O(GrepData *data, regmatch_t *pmatch, char *line) {
  ActiveFlags *config = data->config;
  if (config->o && !config->c && !config->l && !config->v &&
      !data->isEmptyPattern) {
    int start = pmatch->rm_so;
    int end = pmatch->rm_eo;
    Handler_H(data);
    Handler_N(data);
    for (int i = start; i < end; i++) {
      printf("%c", line[i]);
    }
    printf("\n");
    if (line[end] != '\0') {
      SearchCurrentPattern(data, line + end);
    }
  }
}

void PRINT_ERR_NOT_CORRECT_OPTION(GrepData *data) {
  if (optopt == 'e') {
    printf("%s: No pattern after \"e\"\n", data->scriptName);
  } else if (optopt == 'f') {
    printf("%s: No file after \"f\"\n", data->scriptName);
  } else {
    printf("%s: Not correct option - \"%c\"\n", data->scriptName, (char)optopt);
  }
  data->exitValue = 1;
}

void PRINT_ERR_IS_NOT_CORRECT_FILE(GrepData *data) {
  if (!data->config->s) {
    printf("%s: No correct file: \"%s\"", data->scriptName, data->currentFile);
  }
  data->exitValue = 1;
}

void PRINT_ERR_ARGUMENTS(GrepData *data) {
  printf("Use: %s [OPTIONS] [PATTERNS] [FILE]\n", data->scriptName);
  data->exitValue = 1;
}

void PRINT_ERR_FILE(GrepData *data) {
  if (!data->config->s) {
    printf("%s: %s: No such file or directory\n", data->scriptName,
           data->currentFile);
  }
  data->exitValue = 1;
}

void PRINT_ERR_IS_DIR(GrepData *data) {
  if (!data->config->s) {
    printf("%s: %s: Is a directory\n", data->scriptName, data->currentFile);
  }
  data->exitValue = 1;
}
