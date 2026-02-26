#include "c204.h"
#include "c204-test-utils.h"

#include <stdio.h>
#include <stdlib.h>

#define TEST_COUNT ((long int) (sizeof(tests) / sizeof(void *)))


TEST(test_01, "Multiplication operator does not have higher priority than division") //00
	if (convert("a/B*C=", &expr))
	{
		verify(expr, "aB/C*=");
	}
ENDTEST

TEST(test_02, "Multiplication operator has higher priority than plus") //01
	if (convert("a*B+c=", &expr))
	{
		verify(expr, "aB*c+=");
	}
ENDTEST

TEST(test_03, "Parentheses change operator priority") //02
	if (convert("A*(b/c)=", &expr))
	{
		verify(expr, "Abc/*=");
	}
ENDTEST

TEST(test_04, "Parentheses change operator priority") //03
	if (convert("A*(b-C)=", &expr))
	{
		verify(expr, "AbC-*=");
	}
ENDTEST

TEST(test_05, "Complex expression conversion") //04
	if (convert("(A*0+b)*((c*(1+D))-(e/(3*f+g)))=", &expr))
	{
		verify(expr, "A0*b+c1D+*e3f*g+/-*=");
	}
ENDTEST

TEST(test_06, "All lower case characters support") //05
	if (convert("a+b+c+d+e+f+g+h+i+j+k+l+m+n+o+p+q+r+s+t+u+v+x+y+z=", &expr))
	{
		verify(expr, "ab+c+d+e+f+g+h+i+j+k+l+m+n+o+p+q+r+s+t+u+v+x+y+z+=");
	}
ENDTEST

TEST(test_07, "All upper case characters support") //06
	if (convert("A+B+C+D+E+F+G+H+I+J+K+L+M+N+O+P+Q+R+S+T+U+V+X+Y+Z=", &expr))
	{
		verify(expr, "AB+C+D+E+F+G+H+I+J+K+L+M+N+O+P+Q+R+S+T+U+V+X+Y+Z+=");
	}
ENDTEST

TEST(test_08, "All digits support") //07
	if (convert("1+2+3+4+5+6+7+8+9+0=", &expr))
	{
		verify(expr, "12+3+4+5+6+7+8+9+0+=");
	}
ENDTEST

TEST(test_09, "Operands in extra parentheses") //08
	if (convert("(a)+(b)=", &expr))
	{
		verify(expr, "ab+=");
	}
ENDTEST

TEST(test_10, "Expression in double extra parentheses") //09
	if (convert("((a+b))=", &expr))
	{
		verify(expr, "ab+=");
	}
ENDTEST

TEST(test_11, "Expression evaluation form the left to the right also for minus operator") //10
	if (convert("a-b-c=", &expr))
	{
		verify(expr, "ab-c-=");
	}
ENDTEST

TEST(test_12, "Expression evaluation form the left to the right also for multiply operator") //11
	if (convert("a*b*c=", &expr))
	{
		verify(expr, "ab*c*=");
	}
ENDTEST

TEST(test_13, "Expression evaluation form the left to the right also for divide operator") //12
	if (convert("a/b/c=", &expr))
	{
		verify(expr, "ab/c/=");
	}
ENDTEST

TEST(test_14, "Multiply operator has higher priority than plus (inverse order)") //13
	if (convert("A+B*C=", &expr))
	{
		verify(expr, "ABC*+=");
	}
ENDTEST

TEST(test_15, "Divide operator has higher priority than minus (inverse order)") //14
	if (convert("A-b/C=", &expr))
	{
		verify(expr, "AbC/-=");
	}
ENDTEST

TEST(test_16, "Paretntheses change operator priority") //15
	if (convert("(A+b)/c=", &expr))
	{
		verify(expr, "Ab+c/=");
	}
ENDTEST

TEST(test_17, "Paretntheses change operator priority") //16
	if (convert("(a-b)*C=", &expr))
	{
		verify(expr, "ab-C*=");
	}
ENDTEST

TEST(test_18, "Paretntheses change operator priority") //17
	if (convert("a*(B*C)=", &expr))
	{
		verify(expr, "aBC**=");
	}
ENDTEST

TEST(test_19, "Paretntheses change operator priority") //18
	if (convert("a+(B+c)=", &expr))
	{
		verify(expr, "aBc++=");
	}
ENDTEST

TEST(test_20, "Mixed operators expression") // 19
	if (convert("a+b*c-d=", &expr))
	{
		verify(expr, "abc*+d-=");
	}
ENDTEST

TEST(test_21, "Mixed operators expression with parenthesis") //20
	if (convert("(a+b)*(c-d)=", &expr))
	{
		verify(expr, "ab+cd-*=");
	}
ENDTEST

TEST(test_22, "Mixed operators expression with parenthesis") //21
	if (convert("a+b*(c-d)=", &expr))
	{
		verify(expr, "abcd-*+=");
	}
ENDTEST

TEST(test_23, "Mixed operators expression") //22
	if (convert("a*b+c/d=", &expr))
	{
		verify(expr, "ab*cd/+=");
	}
ENDTEST

TEST(test_24, "Complex expression") //23
	if (convert("(A*0+b/l)-((c+D)*(x)/k*8)*(e/(3*f/g+r/t*(j+o/p)))=", &expr))
	{
		verify(expr, "A0*bl/+cD+x*k/8*e3f*g/rt/jop/+*+/*-=");
	}
ENDTEST

TEST(test_eval_01, "Evaluate expression") //24
	STACK_SIZE = 128;
	expr = "a*b+c/d=";
	VariableValue values[] = {
			{'a', 1000},
			{'b', 300},
			{'c', 10000},
			{'d', 500},
	};
	PRINT_EXPR_EVAL_CONFIG();
	if (eval(expr, values, valuesCount, &result) == false) {
		FAIL("eval() call returned false\n");
	}
	printf("eval result: %d\n", result);
ENDTEST

TEST(test_eval_02, "Evaluate expression") //25
	STACK_SIZE = 128;
	expr = "(a-b)*C=";
	VariableValue values[] = {
			{'a', 300},
			{'b', 500},
			{'C', -500},
	};
	PRINT_EXPR_EVAL_CONFIG();
	if (eval(expr, values, valuesCount, &result) == false) {
		FAIL("eval() call returned false\n");
	}
	printf("eval result: %d\n", result);
ENDTEST

TEST(test_eval_03, "Evaluate expression") //26
	STACK_SIZE = 128;
	expr = "(a+b)*(c-d)=";
	VariableValue values[] = {
			{'a', 500},
			{'b', 500},
			{'c', 1200},
			{'d', 200},
	};
	PRINT_EXPR_EVAL_CONFIG();
	if (eval(expr, values, valuesCount, &result) == false) {
		FAIL("eval() call returned false\n");
	}
	printf("eval result: %d\n", result);
ENDTEST

TEST(test_eval_04, "Evaluate expression") //27
	STACK_SIZE = 128;
	expr = "a+b*(c-d)=";
	VariableValue values[] = {
			{'a', 500},
			{'b', -1},
			{'c', -1200},
			{'d', -200},
	};
	PRINT_EXPR_EVAL_CONFIG();
	if (eval(expr, values, valuesCount, &result) == false) {
		FAIL("eval() call returned false\n");
	}
	printf("eval result: %d\n", result);
ENDTEST

TEST(test_eval_05, "Evaluate expression")
	STACK_SIZE = 128;
	expr = "a+b+c+d+e+f+g+h+i+j+k+l+m+n+o+p+q+r+s+t+u+v+x+y+z=";
	VariableValue values[] = {
			{'a', 100},
			{'b', 100},
			{'c', 100},
			{'d', 100},
			{'e', 100},
			{'f', 100},
			{'g', 100},
			{'h', 100},
			{'i', 100},
			{'j', 100},
			{'k', 100},
			{'l', 100},
			{'m', 100},
			{'n', 100},
			{'o', 100},
			{'p', 100},
			{'q', 100},
			{'r', 100},
			{'s', 100},
			{'t', 100},
			{'u', 100},
			{'v', 100},
			{'x', 100},
			{'y', 100},
			{'z', 100},
	};
	PRINT_EXPR_EVAL_CONFIG();
	if (eval(expr, values, valuesCount, &result) == false) {
		FAIL("eval() call returned false\n");
	}
	printf("eval result: %d\n", result);
ENDTEST

TEST(test_eval_06, "Evaluate expression") //29
	STACK_SIZE = 128;
	expr = "(A*0+b)*((c*(1+D))-(e/(5*f+g)))=";
	VariableValue values[] = {
			{'A', 99},
			{'b', -20},
			{'c', 12},
			{'D', 149},
			{'e', 10000},
			{'f', 50},
			{'g', 750},
	};
	PRINT_EXPR_EVAL_CONFIG();
	if (eval(expr, values, valuesCount, &result) == false) {
		FAIL("eval() call returned false\n");
	}
	printf("eval result: %d\n", result);
ENDTEST

TEST(test_eval_07, "Evaluate expression") //30
	STACK_SIZE = 128;
	expr = "(A*0+b/l)-((c+D)*(x)/k*5)*(e/(3*f/g+h/i*(j+k/m)))=";
	VariableValue values[] = {
			{'A', 99},
			{'b', 20},
			{'c', 51},
			{'D', 149},
			{'e', 26350},
			{'f', 500},
			{'g', 20},
			{'h', 350},
			{'i', 10},
			{'j', 15},
			{'k', 20},
			{'l', 5},
			{'m', 4},
			{'x', -10},
	};
	PRINT_EXPR_EVAL_CONFIG();
	if (eval(expr, values, valuesCount, &result) == false) {
		FAIL("eval() call returned false\n");
	}
	printf("eval result: %d\n", result);
ENDTEST

TEST(test_eval_zero_div_01, "Evaluate expression with division by zero") //31
	STACK_SIZE = 128;
	expr = "(a+b)/0=";
	VariableValue values[] = {
			{'a', 10},
			{'b', 20},
	};
	PRINT_EXPR_EVAL_CONFIG();
	if (eval(expr, values, valuesCount, &result) == false) {
		FAIL("eval() call returned false\n");
	}
ENDTEST

TEST(test_eval_zero_div_02, "Evaluate expression with division by zero") //32
	STACK_SIZE = 128;
	expr = "(a+b)/c=";
	VariableValue values[] = {
			{'a', 10},
			{'b', 20},
			{'c', 0},
	};
	PRINT_EXPR_EVAL_CONFIG();
	if (eval(expr, values, valuesCount, &result) == false) {
		FAIL("eval() call returned false\n");
	}
ENDTEST

TEST(test_eval_zero_div_03, "Evaluate expression with division by zero") //33
	STACK_SIZE = 128;
	expr = "(a+b)/(b+(4*c))=";
	VariableValue values[] = {
			{'a', 10},
			{'b', 20},
			{'c', -5},
	};
	PRINT_EXPR_EVAL_CONFIG();
	if (eval(expr, values, valuesCount, &result) == false) {
		FAIL("eval() call returned false\n");
	}
ENDTEST

void (*tests[])(void) = {
		test_01,
		test_02,
		test_03,
		test_04,
		test_05,
		test_06,
		test_07,
		test_08,
		test_09,
		test_10,
		test_11,
		test_12,
		test_13,
		test_14,
		test_15,
		test_16,
		test_17,
		test_18,
		test_19,
		test_20,
		test_21,
		test_22,
		test_23,
		test_24,
		test_eval_01,
		test_eval_02,
		test_eval_03,
		test_eval_04,
		test_eval_05,
		test_eval_06,
		test_eval_07,
		test_eval_zero_div_01,
		test_eval_zero_div_02,
		test_eval_zero_div_03,
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
		printf("C204 - Infix to Postfix Expression Conversion - Basic Tests\n");
		printf("===========================================================\n");

		for (test_id = 0; test_id < TEST_COUNT; test_id++)
		{
			tests[test_id]();
		}
	}

	return 0;
}

/* Konec c204-test.c */
