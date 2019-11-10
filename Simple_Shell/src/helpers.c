// Your helper functions need to be here.
#include "helpers.h"

int child_terminated = 0;

int usr_terminated = 0;

int timeComparator(void* t1, void* t2){
	ProcessEntry_t* x1 = (ProcessEntry_t*)t1, *x2 = (ProcessEntry_t*)t2;
	if (x1->seconds > x2->seconds){
		return 1;
	} else {
		return -1;
	}
}

void child_handler(int s){
	child_terminated = 1;
}

void usr_handler(int s){
	usr_terminated = 1;
}

void reset_flag(){
	child_terminated = 0;
}

void reset_usr(){
	usr_terminated = 0;
}

char* find_cmd(List_t* bg_list, pid_t pid, int* found){
	node_t* n = bg_list->head;
	ProcessEntry_t* p;
	while (n!=NULL){
		p = n->value;
		if (p->pid == pid){
			*found = 1;
			return p->cmd;
		}
		n = n->next;
	}
}

int piped(int num, char* args[]){
	int i, count = 0;
	for (i=0; i<num; i++){
		if (args[i]!=NULL && strcmp(args[i],"|") == 0){
			count ++;
		}
	}
	return count;
}
