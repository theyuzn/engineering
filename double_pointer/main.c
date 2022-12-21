#include <stdio.h>
#include <stdlib.h>

void simple_sample();
void usage();
void cpr_charArr_twoDimArr();
void ptr_sameple();
void ptr_foo(int *, int);
void double_foo(int **, int *, int);
void non_ctinu_malloc();
void ctinu_malloc();
int **findSolution(int (*customFunction)(int, int), int z, int *returnSize, int **returnColumnSizes);

int add(int a, int b) { return a + b; }
int times(int a, int b) { return a * b; }

void print_arr(int **pp, int size, int colSize)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < colSize; j++)
        {
            printf("%4d ", pp[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    char diliminator[11] = "==========";
    // printf("%s usage %s\n", diliminator, diliminator);
    // usage();

    printf("\n%s cpr_charArr_twoDimArr %s\n", diliminator, diliminator);
    cpr_charArr_twoDimArr();

    // printf("\n%s ptr_sameple %s\n", diliminator, diliminator);
    // ptr_sameple();

    // printf("\n%s non_ctinu_malloc %s\n", diliminator, diliminator);
    // non_ctinu_malloc();

    // printf("\n%s ctinu_malloc %s\n", diliminator, diliminator);
    // ctinu_malloc();

    // int returnSize = 0;
    // int *returnColSize;
    // int **ret = findSolution(&add, 67, &returnSize, &returnColSize);
    // print_arr(ret, returnSize, *returnColSize);
    // printf("\n%s%s\n", diliminator, diliminator);

    // returnSize = 0;
    // ret = findSolution(&times, 64, &returnSize, &returnColSize);
    // print_arr(ret, returnSize, *returnColSize);
    // printf("\n%s%s\n", diliminator, diliminator);

    return 0;
}

void simple_sample()
{
    int num = 123;
    int *ptr = &num;
    int **double_ptr = &ptr;
    printf("The val is %d address is %p\n", num, &num);
    printf("The val is %p address is %p\n", ptr, &ptr);
    printf("The val is %p address is %p\n", double_ptr, &double_ptr);
    return;
}

void usage()
{
    // The simplest usage
    simple_sample();

    /* initial Declare */
    int data = 0; // data

    int **ppData = NULL; // double pointer
    int *pData = NULL;   // pointer

    int(*ptr_to_arr)[20]; // pointer to array   陣列的指標 （二維陣列）
    int *ptr_arr[20];     // pointer array      指標陣列

    int x[10][20]; // two dimention array; x's type is * (int[20])

    /* Define */
    ppData = &pData;
    pData = &data;
    **ppData = 10; // Now, data = 10

    // ppData = x;              // warning : incompatible pointer types assigning to 'int **' from 'int [10][20]'
    ppData = ptr_arr;        // OK : same type
    ptr_to_arr = x;          // *[] = [][]
    ptr_to_arr[5][10] = 100; // x[5][10] = 100
                             // *(*(p_arr + 5) + 10) = 100
}

void cpr_charArr_twoDimArr()
{
    char *names1[] = {"Chen Wei Kang", "phd."};
    char names2[][26] = {"prof. Kang", "Keep fighting for 5 years"};

    printf("size of names1 : %ld (16)\nsize of names2 : %ld (2 * 26)\n", sizeof(names1), sizeof(names2));
    printf("%s %s\n%s %s\n", names1[0], names1[1], names2[0], names2[1]);

    #define col 4
    #define row 3

    


    // 1.
    /*=========================*/
    int *ptr; // = ptr[row];
    ptr = calloc(row, sizeof(int));
    ptr = malloc(row * sizeof(int));
    for(int i = 0; i < row; i++)
        ptr[i] = i;
    
    printf("\n========== *ptr\n");
    printf("sizeof(ptr) : %ld\n", sizeof(ptr));
    printf("sizeof(*ptr) : %ld\n", sizeof(*ptr));
    for(int i = 0; i < row; i ++)
        printf("%d\t",ptr[i]);
    printf("\n");
    /*=========================*/

    // 2.
    /*=========================*/
    int (*arr)[col];    // pointer to array   （陣列的）指標 （二維陣列）
                        // 指標裡面存陣列，同時，陣列也可以用指標表示
                        // 所以才有以下這種寫法
    // arr = calloc(row, sizeof(*arr)); // *arr -> int [col] sizeof(*arr)
    arr = malloc(row * sizeof(int *));
    for(int i = 0 ; i < row ; i++)
        for(int j = 0 ; j < col; j++)
            arr[i][j] = i*row + j + 1;

    printf("\n========== (*arr)[col]\n");
    printf("sizeof(*arr) : %ld\nzizeof(int) : %ld\nsizeof(arr)  : %ld\nsizeof(int*) : %ld\n\n", sizeof(*arr), sizeof(int), sizeof(arr), sizeof(int*));
    for(int i = 0 ; i < row ; i++){
        for(int j = 0 ; j < col; j++)
            printf("%d\t", arr[i][j]);
        printf("\n");
    }
    /*=========================*/


    // 3.
    /*=========================*/
    int *ptr_arr[col]; // pointer array      指標的陣列
                       // 陣列裡面存指標
    int *a = calloc(row, sizeof(int));
    int *b = calloc(row, sizeof(int));
    int *c = calloc(row, sizeof(int));
    ptr_arr[0] = a;
    ptr_arr[1] = b;
    ptr_arr[2] = c;

    int *ptr_arr1[] = {a, b, c};

    /*=========================*/


    // 4.
    /*=========================*/

    int **d_ptr;
    d_ptr = calloc(row, sizeof(*d_ptr));
    /*=========================*/


}

void ptr_sameple()
{
    int a = 100;
    int b = 1000;
    int *p = &a;
    int **pp = &p;
    printf("a's addr : %p\nb's addr : %p\n", &a, &b);

    printf("a's value : %d\n", a);
    ptr_foo(&a, 10);
    printf("a's value : %d\n", a);
    ptr_foo(p, 20);
    printf("a's value : %d\n", a);

    printf("p's value : %p and *p's value : %d\n", p, *p);
    double_foo(pp, &a, 10);
    printf("p's value : %p and *p's value : %d\n", p, *p);
    double_foo(pp, &b, b);
    printf("p's value : %p and *p's value : %d\n", p, *p);
    double_foo(pp, &b, 1234);
    printf("p's value : %p and *p's value : %d\n", p, *p);

    return;
}

void ptr_foo(int *a, int num)
{
    *a = num;
    return;
}

void double_foo(int **pp, int *p, int num)
{
    *pp = p;
    **pp = num;
    return;
}

#define COLUMN_S 10
#define ROW_S 5
void non_ctinu_malloc()
{
    int **pp = (int **)malloc(ROW_S * sizeof(int *));

    for (int i = 0; i < ROW_S; i++)
    {
        pp[i] = (int *)malloc(COLUMN_S * sizeof(int));
    }

    for (int i = 0; i < ROW_S; i++)
    {
        for (int j = 0; j < COLUMN_S; j++)
        {
            pp[i][j] = i * (j + 1) + 3;
        }
    }

    for (int i = 0; i < ROW_S; i++)
    {
        for (int j = 0; j < COLUMN_S; j++)
        {
            printf("%4d ", pp[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < ROW_S; i++)
    {
        free(pp[i]);
    }
    free(pp);
}

void ctinu_malloc()
{
    int **pp = (int **)malloc(ROW_S * sizeof(int *));

    int *p = (int *)malloc(ROW_S * COLUMN_S * sizeof(int));

    for (int r = 0; r < ROW_S; r++)
    {
        pp[r] = p;
        p += COLUMN_S;
    }

    for (int i = 0; i < ROW_S; i++)
    {
        for (int j = 0; j < COLUMN_S; j++)
        {
            pp[i][j] = i * (j + 1) + 3;
        }
    }

    for (int i = 0; i < ROW_S; i++)
    {
        for (int j = 0; j < COLUMN_S; j++)
        {
            printf("%4d ", pp[i][j]);
        }
        printf("\n");
    }

    free(pp[0]);
    free(pp);
}

int **findSolution(int (*customFunction)(int, int), int z, int *returnSize, int **returnColumnSizes)
{

    int **ret = NULL;
    *returnColumnSizes = (int *)malloc(sizeof(int));
    **returnColumnSizes = 2;

    for (int x = 1; x <= 1000; x++)
    {
        int y_low = 1;
        int y_high = 1000;
        int y_mid;

        while (y_low <= y_high)
        {
            y_mid = (y_low + y_high) / 2;

            int val = (*customFunction)(x, y_mid);

            if (val < z)
                y_low = y_mid + 1;

            else if (val > z)
                y_high = y_mid - 1;

            else
            {
                if (ret == NULL)
                    ret = (int **)malloc(1 * sizeof(void *));
                else
                    ret = (int **)realloc(ret, ((*returnSize) + 1) * sizeof(int *));

                ret[*returnSize] = (int *)malloc(2 * sizeof(int));
                ret[*returnSize][0] = x;
                ret[*returnSize][1] = y_mid;
                (*returnSize)++;
                break;
            }
        }
    }

    return ret;
}
