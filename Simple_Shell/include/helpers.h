// A header file for helpers.c
// Declare any additional functions in this file
#include "shell_util.h"
int usr_terminated;

int child_terminated;

int timeComparator(void* t1, void* t2);

void child_handler(int s);

void usr_handler(int s);

void reset_flag();

void reset_usr();

char* find_cmd(List_t* bg_list, pid_t pid, int* found);

int piped(int num, char* args[]);
