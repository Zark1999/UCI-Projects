/**
 * Do not submit your assignment with a main function in this file.
 * If you submit with a main function in this file, you will get a zero.
 * If you want to make helper functions, put them in helpers.c
 */
#include "icsmm.h"
#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

ics_free_header *freelist_head = NULL;
ics_free_header *freelist_next = NULL;

void *ics_malloc(size_t size) {
	if (size == 0){
		errno = EINVAL;
		return NULL;
	}
	if (prologue == NULL && epilogue == NULL){
		prologue = ics_inc_brk();
		prologue -> block_size = 8;
		prologue -> unused = 0xaaaaaaaa;
		prologue -> requested_size = 0;
		epilogue = (ics_header*)((char*)prologue + 4088);
		epilogue -> block_size = 8;
		epilogue -> unused = 0xaaaaaaaa;
		epilogue -> requested_size = 0;
		
		freelist_next = (ics_free_header*)((char*)prologue + 8);
		(freelist_next->header).block_size = 4080;
		(freelist_next->header).unused = 0xaaaaaaaa;
		(freelist_next->header).requested_size = 0;
		freelist_next -> next = NULL;
		freelist_next -> prev = NULL;
		
		ics_footer* f = (ics_footer*)((char*)epilogue - 8);
		f -> block_size = 4080;
		f -> unused = 0xffffffffffff;
		
		freelist_head = freelist_next;
		brk_count ++;
	}
	
	int aligned_size;
	if (size%16 == 0){
		aligned_size = size;
	} else {
		aligned_size = ((int)(size/16) + 1)*16;
	}
	
	
	if (freelist_next == NULL){
		if (brk_count == 4){
			errno = ENOMEM;
			return NULL;
		} else {
			ics_footer* f_1 = (ics_footer*)((char*)epilogue - 8);
			ics_inc_brk();
			brk_count ++;
			epilogue = (ics_header*)((char*)epilogue + 4096);
			epilogue -> block_size = 8;
			epilogue -> unused = 0xaaaaaaaa;
			epilogue -> requested_size = 0;
			ics_free_header* h_1;
			
			if (f_1->block_size % 2 == 0){
				h_1 = (ics_free_header*)((char*)f_1 - f_1->block_size + 8);
				(h_1->header).block_size = (h_1->header).block_size + 4096;
			} else {
				h_1 = (ics_free_header*)((char*)f_1 + 8);
				(h_1->header).block_size = 4096;
				(h_1->header).unused = 0xaaaaaaaa;
				(h_1->header).requested_size = 0;
				
				if(freelist_head == NULL){
					freelist_head = h_1;
					freelist_head -> prev = NULL;
					freelist_head -> next = NULL;
					freelist_next = h_1;
					freelist_next -> prev = NULL;
					freelist_next -> next = NULL;
				}
			}
			ics_footer* f_2 = (ics_footer*)((char*)h_1 + (h_1->header).block_size - 8);
			f_2 -> block_size = (h_1->header).block_size;
			f_2 -> unused = 0xffffffffffff;
			
			freelist_next = freelist_head;
		}
	}

	ics_free_header* origin = freelist_next;
	while((freelist_next->header).block_size < aligned_size + 16){
		freelist_next = freelist_next->next;
		if (freelist_next == NULL){
			freelist_next = freelist_head;
		}
		if (freelist_next == origin){
			if (brk_count == 4){
				errno = ENOMEM;
				return NULL;
			} else {
				ics_footer* f_1 = (ics_footer*)((char*)epilogue - 8);
				ics_inc_brk();
				brk_count ++;
				epilogue = (ics_header*)((char*)epilogue + 4096);
				epilogue -> block_size = 8;
				epilogue -> unused = 0xaaaaaaaa;
				epilogue -> requested_size = 0;
				ics_free_header* h_1;
				if ((int)f_1->block_size % 2 == 0){
					h_1 = (ics_free_header*)((char*)f_1 - f_1->block_size + 8);
					(h_1->header).block_size = (h_1->header).block_size + 4096;
				} else {
					h_1 = (ics_free_header*)((char*)f_1 + 8);
					(h_1->header).block_size = 4096;
					(h_1->header).unused = 0xaaaaaaaa;
					(h_1->header).requested_size = 0;
					h_1 -> prev = NULL;
					freelist_head -> prev = h_1;
					h_1 -> next = freelist_head;
					freelist_head = h_1;
				}
				ics_footer* f_2 = (ics_footer*)((char*)h_1 + (h_1->header).block_size - 8);
				f_2 -> block_size = (h_1->header).block_size;
				f_2 -> unused = 0xffffffffffff;
				
				freelist_next = freelist_head;
			}
		}
	}
	
	if ((freelist_next->header).block_size > aligned_size + 16  &&
		(freelist_next->header).block_size - aligned_size - 16 < 32){
		aligned_size = aligned_size + 16;
	}
	

	void* result = (char*)freelist_next + 8;
	if ((freelist_next->header).block_size != aligned_size + 16){
		ics_free_header* new_h = (ics_free_header*)((char*)freelist_next + aligned_size + 16);
		(new_h->header).block_size = (freelist_next->header).block_size - aligned_size - 16;
		(new_h->header).unused = 0xaaaaaaaa;
		(new_h->header).requested_size = 0;
		new_h -> prev = NULL;
		freelist_head -> prev = new_h;
		new_h -> next = freelist_head;
		freelist_head = new_h;
	
		ics_footer* f2 = (ics_footer*)((char*)new_h + (new_h->header).block_size - 8);
		f2 -> block_size = (new_h->header).block_size;
		f2 -> unused = 0xffffffffffff;
	}
	
	ics_footer* f1 = (ics_footer*)((char*)freelist_next + aligned_size + 8);
	f1 -> block_size = aligned_size + 16 + 1;
	f1 -> unused = 0xffffffffffff;
	
	(freelist_next->header).block_size = aligned_size + 16 + 1;
	(freelist_next->header).unused = 0xaaaaaaaa;
	(freelist_next->header).requested_size = size;
	
	ics_free_header* curr = freelist_head;
	ics_free_header* tobe_removed = freelist_next;
	
//	printf("prologue: %p\n",prologue);
//	printf("epilogue: %p\n",epilogue);
//	printf("freelist_head: %p\n",freelist_head);
//	printf("freelist_next: %p\n",freelist_next);
//	printf("f1: %p\n",f1);
//	printf("new_h: %p\n",new_h);
//	printf("f2: %p\n",f2);
	
	freelist_next = freelist_next -> next;
	if (freelist_next == NULL){
		freelist_next = freelist_head;
	}
	
	while (curr != NULL){
		if (curr == tobe_removed){
			if (curr->prev != NULL){
				(curr->prev)->next = curr->next;
				if (curr->next != NULL){
					(curr->next)->prev = curr->prev;
				}
			} else {
				if (curr->next == NULL){
					freelist_head = NULL;
					freelist_next = NULL;
				} else {
					(curr->next) -> prev = NULL;
					freelist_head = curr->next;
				}
			}
			break;
		}
		curr = curr -> next;
	}

	return result;
}

void *ics_realloc(void *ptr, size_t size) {
	if (size == 0){
		errno = EINVAL;
		if (ics_free(ptr) == -1){
			return NULL;
		}
		return NULL;
		
	} else {
	
		if ((char*)ptr < (char*)prologue+8 || (char*)ptr > (char*)epilogue){
			errno = EINVAL;
			return NULL;
		}
		ics_header* h = (ics_header*)((char*)ptr-8);
		ics_footer* f = (ics_footer*)((char*)h + h->block_size - 9);
		if (h->unused != 0xaaaaaaaa || f->unused != 0xffffffffffff || h->block_size != f->block_size
			|| h->requested_size >= h->block_size || (int)h->block_size%2 == 0 || (int)f->block_size%2 == 0){
			errno = EINVAL;
			return NULL;
		}
		
		ics_header* new_h = (ics_header*)((char*)ics_malloc(size)-8);

		if (new_h == NULL){
			return NULL;
		}
		void* result = (char*)new_h + 8;

		char* curr1 = (char*)ptr;
		char* curr2 = (char*)result;
		int i, loop_size;
		if (size <= h->requested_size){
			loop_size = size;
		} else {
			loop_size = h->requested_size;
		}
		for (i = 0; i < loop_size; i++){
			*curr2 = *curr1;
			curr1++;
			curr2++;
		}
		if (ics_free(ptr) == -1){
			return NULL;
		}
		return result;
	}
}

int ics_free(void *ptr) {
	if ((char*)ptr < (char*)prologue+8 || (char*)ptr > (char*)epilogue){
		errno = EINVAL; 
		return -1;
	}
	ics_header* h = (ics_header*)((char*)ptr-8);
	ics_footer* f = (ics_footer*)((char*)h + h->block_size - 9);
	if (h->unused != 0xaaaaaaaa || f->unused != 0xffffffffffff || h->block_size != f->block_size
		|| h->requested_size >= h->block_size || (int)h->block_size%2 == 0 || (int)f->block_size%2 == 0){
		errno = EINVAL;
		return -1;
	}
	
	int check_prev = 1; // 1 if free
	int check_next = 1;
	ics_free_header* new_h;
	ics_header *prev_h, *next_h;
	ics_footer *prev_f, *next_f;
	
	
	if ((char*)h == (char*)prologue+8){
		check_prev = 0;
	} else {
		prev_f = (ics_footer*)((char*)h - 8);
		if ((int)(prev_f->block_size)%2 == 1){
			check_prev = 0;
		}
	}
	
	if ((char*)f == (char*)epilogue-8){
		check_next = 0;
	} else {
		next_h = (ics_header*)((char*)f + 8);
		if ((int)(next_h->block_size)%2 == 1){
			check_next = 0;
		}
	}
	
	h -> block_size = h -> block_size & -2;
	f -> block_size = f -> block_size & -2;
	
	if (check_prev == 0 && check_next == 0){
		new_h = (ics_free_header*) h;
		
	} else if (check_prev == 0 && check_next == 1){
		next_h = (ics_header*)((char*)h + h->block_size);
		next_f = (ics_footer*)((char*)next_h + next_h->block_size - 8);
		h -> block_size = h->block_size + next_h->block_size;
		next_f -> block_size = h->block_size;
		new_h = (ics_free_header*) h;
		
	} else if (check_prev == 1 && check_next == 0){
		prev_f = (ics_footer*)((char*)h - 8);
		prev_h = (ics_header*)((char*)h - prev_f->block_size);
		prev_h -> block_size = prev_h->block_size + h->block_size;
		f -> block_size = prev_h->block_size;
		new_h = (ics_free_header*)prev_h;
		
	} else if (check_prev == 1 && check_next == 1){
		prev_f = (ics_footer*)((char*)h - 8);
		prev_h = (ics_header*)((char*)h - prev_f->block_size);
		next_h = (ics_header*)((char*)h + h->block_size);
		next_f = (ics_footer*)((char*)next_h + next_h->block_size - 8);
		
		prev_h -> block_size = prev_h->block_size + h->block_size + next_h->block_size;
		next_f -> block_size = prev_h -> block_size;
		new_h = (ics_free_header*)prev_h;
	}
	
	
	ics_free_header* curr = freelist_head;
	if (check_prev == 1){
		while (curr != NULL){
			if (curr == (ics_free_header*)prev_h){
				if (curr->prev != NULL){
					(curr->prev)->next = curr->next;
					if (curr->next != NULL){
						(curr->next)->prev = curr->prev;
					}
				} else {
					if (curr->next == NULL){
						freelist_head = NULL;
						freelist_next = NULL;
					} else {
						(curr->next) -> prev = NULL;
						freelist_head = curr->next;
					}
				}
				break;
			}
			curr = curr -> next;
		}
	}
	
	curr = freelist_head;
	if (check_next == 1){
		while (curr != NULL){
			if (curr == (ics_free_header*)next_h){
				if (curr->prev != NULL){
					(curr->prev)->next = curr->next;
					if (curr->next != NULL){
						(curr->next)->prev = curr->prev;
					}
				} else {
					if (curr->next == NULL){
						freelist_head = NULL;
						freelist_next = NULL;
					} else {
						(curr->next) -> prev = NULL;
						freelist_head = curr->next;
					}
				}
				break;
			}
			curr = curr -> next;
		}
	}
	
	if (freelist_head == NULL){
		freelist_head = new_h;
		freelist_head -> prev = NULL;
		freelist_head -> next = NULL;
		freelist_next = new_h;
		freelist_next -> prev = NULL;
		freelist_next -> next = NULL;
	} else {
		new_h -> prev = NULL;
		freelist_head -> prev = new_h;
		new_h -> next = freelist_head;
		freelist_head = new_h;
		
		if ((check_prev == 1 && freelist_next == (ics_free_header*)prev_h)
			|| (check_next == 1 && freelist_next == (ics_free_header*)next_h)){
			freelist_next = freelist_head;
		}
	}
	
	return 0;
}
