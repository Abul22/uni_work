/***************************************************************************
 *
 *   File        : libintersect.c
 *   Student Id  : s3295715
 *   Name        : Chao Ruan
 *
 ***************************************************************************/
#include "libintersect.h"

    int
linear_fsearch(int *set, int len, int key)
{
    int i;

    for(i = 0; i < len; i++){
        if(set[i] == key){
            return i;
        }
        /* return the successor if not found the key */
        else if((set[i] > key)&&(set[i-1] < key)){
            return i;
        }
    }
    return (-1);
}

    int
binary_fsearch(int *set, int len, int key)
{
    int l = 0;
    int r = len - 1;
    int m;

    while (l <= r) {
        m = (l + r) >> 1;
        if (set[m] == key)
            return m;
        else {
            if (key < set[m]){
                /* return the successor if not found the key */
                if((key > set[m-1]) || (m == 0))
                    return m;
                else
                    r = m - 1;
            }
            else
                l = m + 1;
        }
    }
    return (-1);
}

    int
interpolation_fsearch(int *set, int len, int key)
{
    long long l = 0;
    long long r = len - 1;
    long long m;

    if(set[len-1] < key)
        return (-1);

    while (set[l] < key && set[r] >= key) {
        m = l + (((key - set[l]) * (r - l)) / (set[r] - set[l]));

        if (set[m] < key)
            l = m + 1;
        else {
            if (set[m] > key)
                r = m - 1;
            else
                return m;
        }
    }

    /* return the successor if not found the key */
    return l;
}

    int
exponential_fsearch(int *set, int len, int key)
{
    int prev = 0;
    int pos = 2;
    int res;

    while (pos < len && set[pos] < key) {
        prev = pos;
        pos = pos * 2;
    }

    if (pos > len - 1)
        pos = len - 1;
    if (set[pos] == key)
        return pos;
    /* return the successor if key before pos */
    else if((set[pos] > key)&&set[pos-1] <key)
        return pos;
    /* return the successor if key after preve */
    else if(set[prev] > key)
        return prev;

    res = binary_fsearch(&set[prev], pos - prev, key);

    if (res == -1)
        return (-1);
    else
        return res + prev;

    return (-1);
}

    set_t          *
svs(list_t * list, int (*srch_alg) (int *, int, int))
{
    lnode_t *curr,*next;
    int i;
    set_t *set,*n_set,*result;
    curr = list->head;
    /*set the first set be set */
    set = (set_t *)curr->data;
    /*result's set use return the restult */
    result = set_create(DEFAULT_SET_SIZE);
    /*init the result be the firest one of the set */
    for(i = 0; i< set->items;i++)
        set_insert(result,set->data[i]);
    /*start the while loop to go throw every node */
    while(curr != list->tail){
        next = curr->next;
        /*get the next set of the list */
        n_set = (set_t *)next->data;
        /*find the same element in both set*/
        set =  binary_intersection(result,n_set,srch_alg);
        /*use the to in case the memory leak */
        set_destroy(result);
        result = set_create(DEFAULT_SET_SIZE);
        for(i = 0; i< set->items;i++)
            set_insert(result,set->data[i]);
        set_destroy(set);
        curr = curr->next;
    }
    /*set the finger back */
    reset_finger(list);
    set_freeze(result);
    return result; 
}

set_t *
binary_intersection(set_t * set,set_t *n_set,int (*srch_alg)(int *,int,int)){
    set_t *result;
    int x,y,len,index;
    /*set the finger to use */
    set->finger = 0;
    /*set x be the first element */
    x = set->data[set->finger];
    /*create the empty set use to return result */
    result = set_create(DEFAULT_SET_SIZE);
    while(x){
        /*set the next set's finger and get the 
         * right length for F-search*/
        if(n_set->finger == -1){
            len = n_set->items;
            n_set->finger = 0;
        }
        else
            len = n_set->items - n_set->finger;
        /*use F-search */
        index = srch_alg(n_set->data+n_set->finger,len,x);
        /*get the return result's value */
        y = n_set->data[index+n_set->finger];
        /*move the finger */
        n_set->finger = index;
        if(x == y)
            /*if find insert it to result  */
            set_insert(result,x);

        if(set->finger < set->items){
            /*have not found keep looking */
            set->finger++;
            x = set->data[set->finger];
        }
        else
            /*end of set */
            break;

        if((len <= 0) || (index == -1))
            /*not enought of the current set
             * length or finshed search next set*/
            break;
    }
    return result;
}

    set_t          *
sequential(list_t * list, int (*srch_alg) (int *, int, int))
{
    lnode_t *curr;
    set_t *result,*set,*n_set;
    int x,y,k,index,len;
    curr = list->head;
    set = (set_t *)curr->data;
    set->finger = 0;
    /*set k be the counter of the number of
     * success ive mathces*/
    k = 1;
    x = set->data[set->finger];
    /*create empty the return result*/
    result = set_create(DEFAULT_SET_SIZE);
    /*set the tail's next be the head of list
     * to become a circle*/
    list->tail->next = list->head;
    while((x != -1)&&(set->items > set->finger) ){
        /*set the next set's finger and get the 
         * right length for F-search*/
        n_set = (set_t *)curr->data;
        if(n_set->finger == -1){
            len = n_set->items;
            n_set->finger = 0;
        }
        else
            len = n_set->items - n_set->finger;
        /*use F-search */
        index = srch_alg(n_set->data+n_set->finger,len,x);
        /*get the return result's value */
        y = n_set->data[index + n_set->finger];
        /*move the finger */
        n_set->finger = index + n_set->finger;
        if(x == y){
            /*if it is match add the counter */
            k++;
            /*use the counter it match in everyset */
            if(k == list->list_len)
                set_insert(result,x);
        }
        if((x != y) || (k == list->list_len)){
            /*have not found reset thte counter
             * and move to next one*/
            k = 1;
            set->finger++;
            x = set->data[set->finger];
        }
        curr = curr->next;
        /*make sure not finde ih themself */
        if(curr == list->head)
            curr = curr->next;
    }
    /*reset the tail*/
    list->tail->next = NULL;
    reset_finger(list);
    /*set the finger back */
    set_freeze(result);
    return result;
}

    set_t          *
adaptive(list_t * list, int (*srch_alg) (int *, int, int))
{
    set_t *result,*set,*n_set,*i_set;
    lnode_t *curr, *next,*init;
    int x,index,y,i,len;
    /*init a set just for out use while loop */
    init = list->head;
    i_set = (set_t *)init->data;
    x = i_set->data[0];
    /*create empty the return result*/
    result = set_create(DEFAULT_SET_SIZE);
    while(x != -1){
        /*sort the list every times */
        list = list_sort(list,fingercmp);
        /*get the firest node */
        curr = list->head;
        set = (set_t *)curr->data;
        /*set the finger to use */
        if(set->finger == -1)
            set->finger = 0;
        x = set->data[set->finger];
        /*break when it finished*/
        if(set->items <= set->finger)
            break;
        for(i = 1; i < list->list_len; i++){
            next = curr->next;
            n_set = (set_t *)next->data;
            /*set the next set's finger and get the 
             * right length for F-search*/
            if(n_set->finger == -1){
                n_set->finger = 0;
                len = n_set->items;
            }
            else
                len = n_set->items - n_set->finger;
            /*use F-search */
            index = srch_alg(n_set->data + n_set->finger,len,x);
            /*get the return result's value */
            y = n_set->data[index+n_set->finger];
            /*move the finger */
            n_set->finger = index + n_set->finger;
            if(x != y){
                /*not match break */
                set->finger++;
                break;
            }
            else if(i == list->list_len - 1){
                /*mathch, and also go throw all nodes */
                set_insert(result,x);
                set->finger++;
            }
            curr = curr->next;
        }
    }
    set_freeze(result);
    /*set the finger back */
    reset_finger(list);
    return result;
}
/*
 * reset_finger
 * reset the set's finger
 */
void
reset_finger(list_t *list){
    lnode_t *curr;
    set_t *set;
    curr = list->head;
    while(curr){
        /*reset every node'set in the list */
        set = (set_t *)curr->data;
        set->finger = -1;
        curr = curr->next;
    }
}
