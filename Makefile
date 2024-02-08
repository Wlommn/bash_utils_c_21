CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
CAT = ./cat/s21_cat.c
GREP = ./grep/s21_grep.c
COMMON = ./common/linkedList/linkedList.c ./common/queue/queue.c
COMMON_OBJECTS = $(COMMON:.c=.o)
CAT_OBJECTS = $(CAT:.c=.o)
GREP_OBJECTS = $(GREP:.c=.o)


all: s21_cat s21_grep
	$(MAKE) clean
	clear

s21_cat: $(COMMON_OBJECTS) $(CAT_OBJECTS)
	$(CC) $(CFLAGS) $(COMMON_OBJECTS) $(CAT_OBJECTS) -o ./cat/s21_cat

s21_grep: $(COMMON_OBJECTS) $(GREP_OBJECTS)
	$(CC) $(CFLAGS) $(COMMON_OBJECTS) $(GREP_OBJECTS) -o ./grep/s21_grep

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o ./cat/*.o ./grep/*.o ./common/queue/*.o ./common/linkedList/*.o

rebuild:
	$(MAKE) clean
	$(MAKE) all 