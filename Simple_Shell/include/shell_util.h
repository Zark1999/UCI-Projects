#ifndef SHELLUTIL_H
#define SHELLUTIL_H

// Library includes
#include <unistd.h>
#include <time.h> 
#include "linkedList.h"

#define MAX_TOKENS 40

#define RD_ERR "REDIRECTION ERROR: Invalid operators or file combination.\n"
#define BG_TERM "Background process %d: %s, has terminated.\n"
#define DIR_ERR "DIRECTORY ERROR: Directory does not exist.\n"
#define EXEC_ERR "EXEC ERROR: Cannot execute %s.\n"
#define WAIT_ERR "WAIT ERROR: An error ocured while waiting for the process.\n"
#define PID_ERR "PROCESS ERROR: Process pid does not exist.\n"
#define PIPE_ERR "PIPE ERROR: Invalid use of pipe operators.\n"

typedef struct ProcessEntry
{
    char *cmd;  // full command entered, including the &
    pid_t pid;  // pid of the (first) background process
    time_t seconds;   // time at which the command recieved by the shell
                      // used to sort linkedList 
} 
ProcessEntry_t;

/*
 * Deletes a ProcessEntry_t with specified pid from
 * from the List.
 * 
 * Returns 0 for succes, -1 on error
 */
int removeByPid(List_t* list, pid_t p);

/* 
 * Prints out a single ProcessEntry struct to STDERR
 * Async_singal_safe implementation
 */
void printBGPEntry(ProcessEntry_t * p); 

/*
 * Prints message to STDERR prior to termination.
 * Let's you know the SEGFAULT occured in your shell code, not the grader.
 */
void sigsegv_handler();

/*
 * Tokenize the user command in buffer.
 * Places null-terminated tokens in argv array.
 * Assumes argvc must be <= MAX_TOKENS.
 *
 * Returns number of tokens placed in argv (aka argc).
 */
size_t tokenizer(char *buffer, char *argv[]);

/*
 * Prints out the "<53shell>$" prompt to STDOUT
 */
void display_shell_prompt();
    
#endif
