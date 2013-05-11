/***************************************************************************
 *
 *   File        : sl_list.c
 *   Student Id  : s3295715
 *   Name        : Chao Ruan
 *
 ***************************************************************************/
#include "sl_list.h"

/* create an empty list */
    list_t         *
list_create(void)
{
    /* malloc the memory for the list */
    list_t* list =(list_t*) safe_malloc(sizeof(list_t));
    if(!list){
        fprintf(stderr,"Not enough memory for list_create()!\n"); 
        exit(EXIT_FAILURE);
    }
    /* initialize the variables */
    list->head = NULL;
    list->tail = NULL;
    list->list_len = 0;

    return (list);
}

/* free all memory associated with list */
    void
list_free(list_t * list, void (*free_fptr)(set_t*))
{
    lnode_t *current, *next;
    assert(list != NULL);

    current = list->head;
    while(current != NULL){
        next = current->next;
        free_fptr((set_t*)current->data);
        free(current);
        current = next;
    }
    free(list);
}

/* Insert items in an order defined by cmp_fptr */
    list_t         *
list_insert_inorder(list_t * list, void *value,
        int (*cmp_fptr) (const void *, const void *))
{
    lnode_t *prev,*current,*new;
    prev = NULL;
    current = list->head;
    new = safe_malloc(sizeof(lnode_t));
    while(current != NULL &&cmp_fptr(current->data,value) > 0){
        /*use the cmp_fptr to insert the node to list */
        prev = current;
        current = current->next;
    }
    /*set the date be the passing value */
    new->data = value;
    new->next = NULL;
    if(prev){
        new->next = prev->next;
        prev->next = new;
    }else{
        /*head*/
        new->next = list->head;
        list->head = new;
    }
    if(current == NULL)
        /*tail */
        list->tail = new;
    list->list_len++;
    return (list);
}

/* Insertion sort for a generic link list */
    list_t         *
list_sort(list_t * list, int (*cmp_fptr) (const void *, const void *))
{
    lnode_t *sorted,*last,*next;
    lnode_t *prev,*todo,*temp;

    /* only one do not need sort */
    if(list->list_len < 2)
        return (list);

    /* head of the sorted list*/
    sorted = list->head;
    /* tail of the sorted list*/
    last = sorted;
    todo = sorted->next;
    sorted->next = NULL;

    while(todo){
        /* add the node into the sorted list */
        temp = sorted;
        prev = NULL;
        while(temp != NULL && cmp_fptr(temp->data,todo->data) > 0){
            prev = temp;
            temp = temp->next;
        }
        /* set the next node */
        next = todo->next;

        if(prev){
            todo->next = prev->next;
            prev->next = todo;
        }
        else{
            todo->next = sorted;
            sorted = todo;
        }
        if (temp == NULL)
            last = todo;
        /* finished, go to next node */
        todo = next;
    }
    /* put the head and tail back */
    list->head = sorted;
    list->tail = last;

    return (list);

}

