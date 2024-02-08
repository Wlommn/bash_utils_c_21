CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
CAT = ./s21_cat.c
COMMON = ../common/linkedList/linkedList.c ../common/queue/queue.c
COMMON_OBJECTS = $(COMMON:.c=.o)
CAT_OBJECTS = $(CAT:.c=.o)


all: s21_cat
	$(MAKE) clean
	clear

s21_cat: $(COMMON_OBJECTS) $(CAT_OBJECTS)
	$(CC) $(CFLAGS) $(COMMON_OBJECTS) $(CAT_OBJECTS) -o ./s21_cat

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf ./*.o ../common/linkedList/*.o ../common/queue/*.o

rebuild:
	$(MAKE) clean
	$(MAKE) all 