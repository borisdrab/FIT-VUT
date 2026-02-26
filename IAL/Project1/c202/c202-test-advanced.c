#include "c202.h"
#include "c202-test-utils.h"

#include <stdio.h>
#include <stdlib.h>

#define TEST_COUNT ((long int) (sizeof(tests) / sizeof(void *)))

//EDIT: 
TEST(test_null, "Test null pointer in stack init") //Křena testy
	Stack_Init(NULL);

	Stack_Init(test_stack); // Musí to tady být kvůli tomu že nesmí nastal stack null u dispose
ENDTEST

TEST(test_empty_pop, "Pop from empty stack") //00
	Stack_Init(test_stack);
	test_print_stack(test_stack);

	Stack_Pop(test_stack);
	test_print_stack(test_stack);
ENDTEST

TEST(test_empty_top, "Top of empty stack") //01
	Stack_Init(test_stack);
	test_print_stack(test_stack);

	char top;
	Stack_Top(test_stack, &top);
ENDTEST

TEST(test_pop_top, "Top of stack after popping") //02
	Stack_Init(test_stack);
	Stack_Push(test_stack, 'a');
	test_print_stack(test_stack);

	Stack_Pop(test_stack);
	test_print_stack(test_stack);
	char top;
	Stack_Top(test_stack, &top);
ENDTEST

TEST(test_overpush, "Push over the stack capacity") //03
	STACK_SIZE = 1;
	Stack_Init(test_stack);
	test_print_stack(test_stack);

	Stack_Push(test_stack, 'a');
	Stack_Push(test_stack, 'b');
	Stack_Push(test_stack, 'c');
	test_print_stack(test_stack);
ENDTEST

TEST(test_overpush_pop_overpush, "Push over the stack capacity, free it and do it again") //04
	STACK_SIZE = 1;
	
	Stack_Init(test_stack);
	test_print_stack(test_stack);

	Stack_Push(test_stack, 'a');
	Stack_Push(test_stack, 'b');
	Stack_Push(test_stack, 'c');
	test_print_stack(test_stack);

	Stack_Pop(test_stack);

	Stack_Push(test_stack, 'b');
	Stack_Push(test_stack, 'c');
	Stack_Push(test_stack, 'd');
	test_print_stack(test_stack);
ENDTEST

TEST(test_overpush_many, "Push over the stack capacity many times") //05
	STACK_SIZE = 1;

	Stack_Init(test_stack);
	test_print_stack(test_stack);

	for(int i=0; i<10; i++)
		Stack_Push(test_stack, '0' + i);
	test_print_stack(test_stack);
ENDTEST

TEST(test_overpop_many, "Pop from stack when empty many times") //06
	STACK_SIZE = 1;

	Stack_Init(test_stack);
	test_print_stack(test_stack);

	for(int i=0; i<100; i++)
		Stack_Pop(test_stack);

	test_print_stack(test_stack);
ENDTEST

TEST(test_large_fill, "Fill a large stack") //07
	STACK_SIZE = 26*5;

	Stack_Init(test_stack);
	test_print_stack(test_stack);

	for(int i=0; i<5; i++)
		for(int y=0; y<26; y++)
			Stack_Push(test_stack, 'a' + y);
	
	test_print_stack(test_stack);
ENDTEST

//Tohle jsem udělal trošku jinak protože nechápu jak to chtěl dělat autor zadání
TEST(test_large_fill_empty, "Fill and empty a large stack") //08
	STACK_SIZE = 26*5;

	Stack_Init(test_stack);
	for(int i=0; i<5; i++)
		for(int y=0; y<26; y++)
			Stack_Push(test_stack, 'a' + y);
	
	test_print_stack(test_stack);

	test_stack->topIndex = -1;
	test_print_stack(test_stack);
ENDTEST

TEST(test_dispose, "Dispose stack") //09
	STACK_SIZE = 26;
	Stack_Init(test_stack);
	for(int y=0; y<26; y++)
		Stack_Push(test_stack, 'a' + y);
	test_print_stack(test_stack);

	Stack_Dispose(test_stack);
	test_print_stack(test_stack);
ENDTEST

void (*tests[])(void) = {
		// null
		test_null,
		// pop
		test_empty_pop,
		// top
		test_empty_top,
		test_pop_top,
		// overpush
		test_overpush,
		test_overpush_pop_overpush,
		test_overpush_many,
		//overpop
		test_overpop_many,
		// filling
		test_large_fill,
		test_large_fill_empty,
		// dispose
		test_dispose,
};

int main( int argc, char *argv[] ) {
	if (argc > 2)
	{
		fprintf(stderr, "Usage: %s [test_id]\n", argv[0]);
		return 1;
	}

	char *test_id_reminder = NULL;
	long test_id;
	if (argc == 2)
	{
		test_id = strtol(argv[1], &test_id_reminder, 10);
		if (test_id_reminder[0] != 0)
		{
			fprintf(stderr, "Usage: %s {test_id}\n", test_id_reminder);
			fprintf(stderr, "Unexpected test_id: %s\n", test_id_reminder);
			return 1;
		}

		if (test_id < 0)
		{
			test_id = TEST_COUNT + test_id;
		}

		if (test_id + 1 > TEST_COUNT)
		{
			fprintf(stderr, "Unknown test: %ld (test count: %ld)\n", test_id, TEST_COUNT);
			return 1;
		}

		tests[test_id]();
	}
	else
	{
		printf("C202 - Stack Implemented Using an Array - Advanced Tests\n");
		printf("========================================================\n");

		for (test_id = 0; test_id < TEST_COUNT; test_id++)
		{
			tests[test_id]();
		}
	}

	return 0;
}
