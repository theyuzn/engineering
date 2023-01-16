#include <stdio.h>
#include <stdlib.h>

// 12 bytes = 4 * 3
typedef struct test_t{
	int a;
	char aa;
	char bb;

	int b;
	int c;
	
	int *te;
	int *testss;
}test;


int main(){
	test *t = (test*) malloc(sizeof(test));
	int *a = NULL;
	char *b = NULL;
	test t_arr[2];


	printf("sizeof pointer %lu \n", sizeof(t));
	printf("sizeof structure %ld\n", (char*)(&(t_arr[1])) - (char*)(&(t_arr[0])));
	printf("sizeof strucuture %lu \n", sizeof(test));
	return 0;
}
