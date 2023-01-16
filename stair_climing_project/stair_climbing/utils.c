#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"


void dump_users(List *list){
    if(list->head){
        Node *node = list->head;
        printf("|=======================================|\n");
        do{
            printf("| The user ID\t\t: %10d\t|\n", node->id);
            printf("| The user name\t\t: %10s\t|\n", node->name);
            printf("| The user result\t: %10d\t|\n", node->result);
            printf("|=======================================|\n");
            node = node->next;
        }while(node);
    }
}

void gen_choice(char** choice, int length){
    while(length--){
        choice[length] = (char*) malloc(100*sizeof(char));
        printf("The opt. is :");
        scanf("%s", choice[length]);
    }
}

void dump_choice(char **choice, int length){
    printf("|==========================|\n");
    while(length--){
        printf("|The opt. is\t:%10s|\n", choice[length]);
    }
    printf("|==========================|\n");
    return;
}


void show_result(List* list, char** choice, int length){
    if(!list || !list->head || !choice || !*choice || length == 0) return;

    int i = 0;
    Node* node = list->head;
    while(node){
        printf("%10s", node->name);
        node = node->next;
    }
    printf("\n");

    for(int i = 0; i< HEIGHT; i ++){
        node = list->head;
        while(node){
            
            if(node->next){
                if(rand()%100 <= SHUFFLE) printf("%10s","|---------");
                else printf("%10s", "|");
            }
            else printf("%10s", "|");
            node = node->next;
        }
        printf("\n");
    }


    
    


}
