#include "debug.h"
#include "icsmm.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>

#define VALUE1_VALUE 320
#define VALUE2_VALUE 0xDEADBEEFF00D

void press_to_cont() {                                                       
    printf("Press Enter to Continue");                                      
    while (getchar() != '\n')                                               
      ;                                                                     
    printf("\n");                                                           
}

void null_check(void* ptr, long size) {
    if (ptr == NULL) {                                                       
      error(                                                                   
          "Failed to allocate %lu byte(s) for an integer using ics_malloc.\n", 
          size);                                                             
      error("%s\n", "Aborting...");                                            
      assert(false);                                                           
    } else {                                                                   
      success("ics_malloc returned a non-null address: %p\n", (void *)(ptr));  
    }                                                                          
}

void payload_check(void* ptr) {
    if ((unsigned long)(ptr) % 16 != 0) {
      warn("Returned payload address is not divisble by a quadword. %p %% 16 " 
           "= %lu\n",                                                          
           (void *)(ptr), (unsigned long)(ptr) % 16);                          
    }                                                                          
}

// This is an illustrative "crazy" macro :)
#define CHECK_PRIM_CONTENTS(actual_value, expected_value, fmt_spec, name)      \
  do {                                                                         \
    if (*(actual_value) != (expected_value)) {                                 \
      error("Expected " name " to be " fmt_spec " but got " fmt_spec "\n",     \
            (expected_value), *(actual_value));                                \
      error("%s\n", "Aborting...");                                            \
      assert(false);                                                           \
    } else {                                                                   \
      success(name " retained the value of " fmt_spec " after assignment\n",   \
              (expected_value));                                               \
    }                                                                          \
  } while (0)


int main(int argc, char *argv[]) {
  // Initialize the custom allocator
  ics_mem_init();

  // Tell the user about the fields
  info("Initialized heap\n");
  press_to_cont();

  // Print out title for first test
	printf("=== Test1: Allocation test ===\n");
	// Test #1: Allocate an integer
//	int *value1 = ics_malloc(4064);
//	null_check(value1, sizeof(int));
//	payload_check(value1);
//	ics_payload_print((void*)value1);
	int* x1 = ics_malloc(48);
	*x1 = 112;
	ics_payload_print((void*)x1);
	void *x2 = ics_malloc(32);
	ics_payload_print((void*)x2);
	
	void *x3 = ics_malloc(60);
	ics_payload_print((void*)x3);
	void *x4 = ics_malloc(144);
	ics_payload_print((void*)x4);
	void *x5 = ics_malloc(32);
	ics_payload_print((void*)x5);
	void *x6 = ics_malloc(9000);
	ics_payload_print((void*)x6);
	ics_freelist_print();
	ics_free(x2);
	printf("after free x2:--------------------------------------------------------------------------------\n");
	ics_freelist_print();
	ics_free(x4);
	printf("after free x4:--------------------------------------------------------------------------------\n");
	ics_freelist_print();
	ics_free(x3);
	printf("after free x3:--------------------------------------------------------------------------------\n");
	ics_freelist_print();
	ics_free(x5);
	printf("after free x5:--------------------------------------------------------------------------------\n");
	ics_freelist_print();
	
////	void *x6 = ics_malloc(1000000);
////	if (x6 == NULL){
////		printf("error\n");
////	}
////	null_check(x6, 1000000);
////	ics_payload_print((void*)x6);
//	ics_freelist_print();
//	printf("freelist_head: %p\n",freelist_head);
//	printf("freelist_next: %p\n",freelist_next);
//	ics_free(x2);
//
//	printf("freelist_head: %p\n",freelist_head);
//	printf("freelist_next: %p\n",freelist_next);
//	printf("--------------------------------------------------------------------------------\n");
//	ics_freelist_print();
//	printf("--------------------------------------------------------------------------------\n");
//
//	/* Malloc requests */
//	void *x7 = ics_malloc(16);
//	ics_payload_print((void*)x7);
//	ics_freelist_print();
//	printf("--------------------------------------------------------------------------------\n");
//	int *x8 = ics_realloc(x1, 7);
////	int *x8 = ics_malloc(60);
//	null_check(x8, 60);
//	ics_payload_print((void*)x8);
//	ics_freelist_print();
//	CHECK_PRIM_CONTENTS(x8, 112, "%d", "x8");
	press_to_cont();
	
	// Now assign a value
//	printf("=== Test2: Assignment test ===\n");
//	info("Attempting to assign value1 = %d\n", VALUE1_VALUE);
//	// Assign the value
//	*value1 = VALUE1_VALUE;
//	// Now check its value
//	CHECK_PRIM_CONTENTS(value1, VALUE1_VALUE, "%d", "value1");
//	ics_payload_print((void*)value1);
//	press_to_cont();
//
//	printf("=== Test3: Allocate a second variable ===\n");
//	info("Attempting to assign value2 = %ld\n", VALUE2_VALUE);
//	long *value2 = ics_malloc(sizeof(long));
//	null_check(value2, sizeof(long));
//	payload_check(value2);
//	// Assign a value
//	*value2 = VALUE2_VALUE;
//	// Check value
//	CHECK_PRIM_CONTENTS(value2, VALUE2_VALUE, "%ld", "value2");
//	ics_payload_print((void*)value2);
//	press_to_cont();

//	printf("=== Test4: does value1 still equal %d ===\n", VALUE1_VALUE);
//	CHECK_PRIM_CONTENTS(value1, VALUE1_VALUE, "%d", "value1");
//	ics_payload_print((void*)value1);
//	press_to_cont();
//
//	// Free a variable
//	printf("=== Test5: Free a block and snapshot ===\n");
//	info("%s\n", "Freeing value1...");
//	ics_free(value1);
//	ics_freelist_print();
//  press_to_cont();
//
//  // Allocate a large chunk of memory and then free it
//  printf("=== Test6: 8192 byte allocation ===\n");
//  void *memory = ics_malloc(8192);
//  ics_freelist_print();
//  press_to_cont();
//  ics_free(memory);
//  ics_freelist_print();
//  press_to_cont();

  ics_mem_fini();

  return EXIT_SUCCESS;
}
