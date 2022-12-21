#include <stdlib.h>
#include <stdio.h>

#define LENGTH 10

void test(int a[LENGTH]){
    for(int i = 0; i < LENGTH; i ++)
        printf("%d  ",a[i]);
    printf("\n");
}

void tch(char a[LENGTH]){
    for(int i = 0; i < LENGTH; i ++)
        printf("%c  ",a[i]);
    printf("\n");
}


int main(){

	int aa[LENGTH] = {0,1,2,3,4,5,6,7,8,9};
    char *str = "asdfhfksjd";
	test(aa);
    tch(str);

	return 0;

}
