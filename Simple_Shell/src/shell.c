#include "shell_util.h"
#include "linkedList.h"
#include "helpers.h"

// Library Includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/prctl.h>

int main(int argc, char *argv[])
{
	int i, j; //loop counter
	char *args[MAX_TOKENS + 1];
	int exec_result;
	int exit_status;
	pid_t pid;
	pid_t wait_result;
	List_t bg_list;
	
	//Initialize the linked list
	bg_list.head = NULL;
	bg_list.length = 0;
	bg_list.comparator = timeComparator;  // Don't forget to initialize this to your comparator!!!
	
	char* s;
	sigset_t mask_all, prev_all, mask_child;
	
	// Setup segmentation fault handler
	if(signal(SIGSEGV, sigsegv_handler) == SIG_ERR)
	{
		perror("Failed to set signal handler");
		exit(-1);
	}
	
	sigfillset(&mask_all);
	sigemptyset(&mask_child);
	sigaddset(&mask_child,SIGCHLD);
	signal(SIGCHLD, child_handler);
	signal(SIGUSR1, usr_handler);
	
	
	while(1) {
		// DO NOT MODIFY buffer
		// The buffer is dynamically allocated, we need to free it at the end of the loop
		char * const buffer = NULL;
		size_t buf_size = 0;
		
		int ff1=0,ff2=0,ff3=0,ff4=0;
		int index[4];
		int fp1,fp2,fp3,fp4;
		int is_bg = 0;
		// Print the shell prompt
		display_shell_prompt();
		
		// Read line from STDIN
		ssize_t nbytes = getline((char **)&buffer, &buf_size, stdin);
		
		// No more input from STDIN, free buffer and terminate
		if(nbytes == -1) {
			free(buffer);
			break;
		}
		
		// Remove newline character from buffer, if it's there
		if(buffer[nbytes - 1] == '\n')
			buffer[nbytes- 1] = '\0';
		
		// Handling empty strings
		if(strcmp(buffer, "") == 0) {
			free(buffer);
			continue;
		}
		
		// Parsing input string into a sequence of tokens
		s = malloc(buf_size);
		strcpy(s,buffer);
		
		size_t numTokens;
		*args = NULL;
		numTokens = tokenizer(buffer, args);
		
		
		if(strcmp(args[0],"exit") == 0) {
			// Terminating the shell
			node_t* n = bg_list.head;
			ProcessEntry_t* p;
			while (n!=NULL){
				p = n->value;
				kill(p->pid,SIGTERM);
				printf("Background process %s, has terminated.\n",p->cmd);
				free(p->cmd);
				n = n->next;
			}
			free(buffer);
			free(s);
			return 0;
		}
		
		if (strcmp(args[0],"cd") == 0){
			if (numTokens >= 2){
				if (chdir(args[1]) != 0){
					fprintf(stderr, "%s", DIR_ERR);
				} else {
					long size = pathconf(".", _PC_PATH_MAX);
					char* buf = malloc(size);
					fprintf(stdout, "%s\n", getcwd(buf,size));
					free(buf);
				}
			}
			free(buffer);
			free(s);
			continue;
		}
		
		if (strcmp(args[0],"fg") == 0){
			int found = 0;
			if (bg_list.length > 0){
				node_t* n = bg_list.head;
				ProcessEntry_t* p;
				if (numTokens > 1 && args[1] != NULL){
					while(n != NULL){
						p = n->value;
						if (p->pid == atoi(args[1])){
							found = 1;
							wait_result = waitpid(p->pid,&exit_status,0);
							if(wait_result == -1){
								printf(WAIT_ERR);
								exit(EXIT_FAILURE);
							}
							sigprocmask(SIG_BLOCK,&mask_all,&prev_all);
							removeByPid(&bg_list,wait_result);
							sigprocmask(SIG_SETMASK,&prev_all,NULL);
							break;
						}
						n = n->next;
					}
					if (found == 0){
						fprintf(stderr,"%s",PID_ERR);
						free(buffer);
						free(s);
						continue;
					}
				}
			}
			free(buffer);
			free(s);
			continue;
		}
		
		if (strcmp(args[0],"estatus") == 0){
			printf("%d\n",WEXITSTATUS(exit_status));
			free(buffer);
			free(s);
			continue;
		}
		
		int read_err = 0;
		
		for (i=0; i<numTokens; i++){
			if (strcmp(args[i],">")==0){
				if(ff1==0 && ff2==0){ff1 = 1; index[0] = i+1;}
				else{read_err = 1;}
			}
			if (strcmp(args[i],">>")==0){
				if(ff2==0 && ff1==0){ff2 = 1; index[1] = i+1;}
				else{read_err = 1;}
			}
			if (strcmp(args[i],"2>")==0){
				if(ff3==0){ff3 = 1; index[2] = i+1;}
				else{read_err = 1;}
			}
			if (strcmp(args[i],"<")==0){
				if(ff4==0){ff4 = 1; index[3] = i+1;}
				else{read_err = 1;}
			}
		}
		
		
		if (ff1==1&&ff2==1&&strcmp(args[index[0]],args[index[1]])==0){read_err = 1;}
		if (ff1==1&&ff3==1&&strcmp(args[index[0]],args[index[2]])==0){read_err = 1;}
		if (ff1==1&&ff4==1&&strcmp(args[index[0]],args[index[3]])==0){read_err = 1;}
		if (ff2==1&&ff3==1&&strcmp(args[index[1]],args[index[2]])==0){read_err = 1;}
		if (ff2==1&&ff4==1&&strcmp(args[index[1]],args[index[3]])==0){read_err = 1;}
		if (ff3==1&&ff4==1&&strcmp(args[index[2]],args[index[3]])==0){read_err = 1;}
		
		if (read_err == 1){
			fprintf(stderr,"%s",RD_ERR);free(buffer);free(s);continue;
		}
		
		if (strcmp(args[numTokens-1], "&") == 0){
			is_bg = 1;
			args[numTokens-1] = NULL;
		}
		
		int num_pipe = piped(numTokens,args);
		
		int* x = malloc((num_pipe+1) * sizeof(*x));
		
		x [0] = 0;
		int c = 1;
		int pipe_error = 0;
		
		for (i=0; i<numTokens; i++){
			if (args[i]!=NULL && strcmp(args[i],"|") == 0){
				if (i == 0 || i == numTokens-1){
					pipe_error = 1;
					break;
				}
				args[i] = NULL;
				x[c] = i + 1;
				c ++;
			}
		}
		
		
		for (i=1; i<num_pipe; i++){
			if (x[i]+1 == x[i+1]){
				pipe_error = 1;
				break;
			}
		}
		
		if (pipe_error == 1){
			fprintf(stderr,"%s",PIPE_ERR);
			free(buffer);
			free(s);
			free(x);
			continue;
		}
		
		int fd[2*num_pipe];
		
		for (i=0; i<num_pipe; i++){
			pipe(fd+i*2);
		}
		
		
		if (ff1 == 1){
			if (args[index[0]] == NULL || strlen(args[index[0]]) == 0){
				fprintf(stderr,"%s",RD_ERR);free(buffer);free(s);free(x);continue;
			}
			if (num_pipe > 0 && index[0] <= x[num_pipe]+1){
				fprintf(stderr,"%s",PIPE_ERR);
				free(buffer);
				free(s);
				free(x);
				continue;
			}
			fp1 = open(args[index[0]], O_WRONLY | O_CREAT, 0644);
			args[index[0]-1] = NULL;
			args[index[0]] = NULL;
		}
		
		if (ff2 == 1){
			if (args[index[1]] == NULL || strlen(args[index[1]]) == 0){
				fprintf(stderr,"%s",RD_ERR);free(buffer);free(s);free(x);continue;
			}
			if (num_pipe > 0 && index[1] <= x[num_pipe]+1){
				fprintf(stderr,"%s",PIPE_ERR);
				free(buffer);
				free(s);
				free(x);
				continue;
			}
			fp2 = open(args[index[1]], O_WRONLY | O_APPEND | O_CREAT, 0644);
			args[index[1]-1] = NULL;
			args[index[1]] = NULL;
		}
		
		if (ff3 == 1){
			if (args[index[2]] == NULL || strlen(args[index[2]]) == 0){
				fprintf(stderr,"%s",RD_ERR);free(buffer);free(s);free(x);continue;
			}
			fp3 = open(args[index[2]], O_WRONLY | O_CREAT, 0644);
			args[index[2]-1] = NULL;
			args[index[2]] = NULL;
		}
		
		if (ff4 == 1){
			if (args[index[3]] == NULL || strlen(args[index[3]]) == 0){
				fprintf(stderr,"%s",RD_ERR);free(buffer);free(s);free(x);continue;
			}
			if (num_pipe > 0 && index[3] >= x[1]-1){
				fprintf(stderr,"%s",PIPE_ERR);
				free(buffer);
				free(s);
				free(x);
				continue;
			}
			fp4 = open(args[index[3]], O_RDONLY, 0);
			args[index[3]-1] = NULL;
			args[index[3]] = NULL;
			if (fp4 < 1){
				fprintf(stderr,"%s",RD_ERR);free(buffer);free(s);free(x);continue;
			}
			dup2(fp4,0);
			close(fp4);
		}
		
		pid_t ppid = getpid();
		for (i=0; i<=num_pipe; i++){
			if ((pid=fork()) == 0){ //If zero, then it's the child process
				int r = prctl(PR_SET_PDEATHSIG, SIGTERM);
				if (r == -1) {perror(0);exit(1);}

				if (getppid() != ppid){
					exit(1);
				}
				
				if (ff1 == 1){
					dup2(fp1,1);
					close(fp1);
				}
				if (ff2 == 1){
					dup2(fp2,1);
					close(fp2);
				}
				if (ff3 == 1){
					dup2(fp3,2);
					close(fp3);
				}
				if (ff4 == 1){
					dup2(fp4,0);
					close(fp4);
				}
				
				if (num_pipe > 0){
					if (i == 0){
						dup2(fd[1],1);
					} else if (i == num_pipe){
						dup2(fd[(num_pipe-1)*2],0);
					} else {
						dup2(fd[2*i+1],1);
						dup2(fd[(i-1)*2],0);
					}
					
					for(j=0;j<num_pipe*2;j++){
						close(fd[j]);
					}
				}
				
				if (args[x[i]] == NULL){
					exit(EXIT_SUCCESS);
				}
				
				exec_result = execvp(args[x[i]],&args[x[i]]);
				
				if(exec_result == -1){ //Error checking
					printf(EXEC_ERR, args[x[i]]);
					exit(EXIT_FAILURE);
				}
				
				exit(EXIT_SUCCESS);
				
			} else { // Parent Process
				
				
				if (is_bg == 1 && i == num_pipe){
					time_t begin = time(NULL);
					ProcessEntry_t* p = malloc(sizeof(*p));
					p -> cmd = s;
					p -> pid = pid;
					p -> seconds = begin;
					sigprocmask(SIG_BLOCK,&mask_all,&prev_all);
					insertInOrder(&bg_list,p);
					sigprocmask(SIG_SETMASK,&prev_all, NULL);
					//					free(buffer);
					if (num_pipe > 0){
						for(j=0;j<num_pipe*2;j++){
							close(fd[j]);
						}
					}
					break;
				}
				
				if (num_pipe > 0 && i > 0){
					close(fd[(i-1)*2]);
					close(fd[(i-1)*2+1]);
				}
				
				if (is_bg == 0){
					wait_result = waitpid(pid,&exit_status,0);
					if(wait_result == -1){
						printf(WAIT_ERR);
						exit(EXIT_FAILURE);
					}
				}
				
				if (usr_terminated == 1){
					node_t* n = bg_list.head;
					ProcessEntry_t* p;
					while (n!=NULL){
						p = n->value;
						printBGPEntry(p);
						n = n->next;
					}
					reset_usr();
				}
				
				if (child_terminated == 1){
					while((wait_result = waitpid(-1,&exit_status,WNOHANG))>0){
						int found = 0;
						char* cur_s = find_cmd(&bg_list,wait_result,&found);
						if (found == 1){
							sigprocmask(SIG_BLOCK,&mask_all,&prev_all);
							removeByPid(&bg_list,wait_result);
							printf(BG_TERM,wait_result,cur_s);
							free(cur_s);
							sigprocmask(SIG_SETMASK,&prev_all,NULL);
						}
					}
					reset_flag();
				}
			}
		} // for loop
		
		if (num_pipe > 0){
			for(j=0;j<num_pipe*2;j++){
				close(fd[j]);
			}
		}
		
		if (ff1 == 1){
			close(fp1);
		}
		if (ff2 == 1){
			close(fp2);
		}
		if (ff3 == 1){
			close(fp3);
		}
		if (ff4 == 1){
			close(fp4);
		}
		
		free(x);
		free(buffer);
	}
	return 0;
}
