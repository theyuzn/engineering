#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "stair_climbing.h"
#include "utils.h"

#define LOG_E(x) do{printf("[LOG] : %s\n" ,x);}while(0);
#define MAX_SIZE 65536


List* init(int no_user){
    List *list = (List*) malloc(sizeof(List));
    list->head = NULL;
    list->tail = NULL;

    int u = 0;
    while(u < no_user){
        Node *node = (Node*) malloc(sizeof(Node));
        node->name = (char*) malloc(sizeof(char)*100);
        node->id = u;
        printf("The name of user %d is : ", u);
        scanf("%s", node->name);
        node->result = -1;
        node->next = NULL;

        if(!list->head) {
            list->head = node;
            list->tail = node;
        }else{
            list->tail->next = node;
            list->tail = node;
        }
        u++;
    }

    return list;
}

 


int main(int argc, char* argv[]){
    srand(time(NULL)); 
    if(argc != 2){
        LOG_E("Please input the number of the user");
        return 0;
    }

    int no_user = atoi(argv[1]);
    List *list = init(no_user);

    char* choice[MAX_SIZE] = {0};
    gen_choice(choice, no_user);
    show_result(list, choice, no_user);

    free(list);
    return 0;
}