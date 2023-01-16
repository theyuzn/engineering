#ifndef __STAIR_CLIMBING_H__
#define __STAIR_CLIMBING_H__


typedef struct Node_t{
    int id;
    char* name;
    int result;
    struct Node_t *next;
}Node;  

typedef struct List_t{
    Node *head;
    Node *tail;
}List;


#endif
