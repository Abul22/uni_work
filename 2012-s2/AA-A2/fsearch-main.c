/***************************************************************************
 *
 *   File        : fsearch-main.c
 *   Student Id  : s3295715
 *   Name        : Chao Ruan
 *
 ***************************************************************************/
#include "libutil.h" 
#include "libset.h"
#include "libintersect.h" 

#define DEF_SET_SIZE 128

void
test(char* alg,char* txt,int b)
{
   if(b) {
      fprintf(stderr,"TEST %23s - %15s %10s\n",alg,txt,"OK");  
   } else {
      fprintf(stderr,"TEST %23s - %15s %10s\n",alg,txt,"FAILED");
   }
}

int 
main (int argc, char **argv)
{
    int i;
    int test_size = DEF_SET_SIZE;
    set_t* set_test;
    
    int testdata[8] = { 2, 3 ,5 ,7 ,9 ,10 ,20 ,100};
    set_test = set_create(test_size);
    
    fprintf(stderr,"     %23s   %15s %10s\n\n","OBJECT","FUNCTION","RESULT");  
    
    // SET INIT TEST
    if( set_test->data != NULL &&
        set_test->items == 0 &&
        set_test->lock == FALSE &&
        set_test->finger == -1 ) {
        test("set_t","initialsation",TRUE);
    } else 
        test("set_t","initialsation",FALSE);

    for (i = 0;i < 8; i++) set_insert(set_test,testdata[i]);
    
    // SET INSERT TEST
    if( set_test->items == 8 &&
        set_test->data[0] == 2 &&
        set_test->data[1] == 3 &&
        set_test->data[2] == 5 &&
        set_test->data[3] == 7 &&
        set_test->data[4] == 9 &&
        set_test->data[5] == 10 &&
        set_test->data[6] == 20 &&
        set_test->data[7] == 100 ) {
        
        test("set_t","insert",TRUE);
    } else test("set_t","insert",FALSE);
    
    //  FSEARCH - TEST SEARCH MATCH
    if( linear_fsearch(set_test->data,set_test->items,2) == 0 &&
        linear_fsearch(set_test->data,set_test->items,5) == 2 &&
        linear_fsearch(set_test->data,set_test->items,10) == 5 &&
        linear_fsearch(set_test->data,set_test->items,100) == 7 ) {
        test("linear_fsearch","match",TRUE);
    } else test("linear_fsearch","match",FALSE);
    
    if( binary_fsearch(set_test->data,set_test->items,2) == 0 &&
        binary_fsearch(set_test->data,set_test->items,5) == 2 &&
        binary_fsearch(set_test->data,set_test->items,10) == 5 &&
        binary_fsearch(set_test->data,set_test->items,100) == 7 ) {
        test("binary_fsearch","match",TRUE);
    } else test("binary_fsearch","match",FALSE);
    
    if( interpolation_fsearch(set_test->data,set_test->items,2) == 0 &&
        interpolation_fsearch(set_test->data,set_test->items,5) == 2 &&
        interpolation_fsearch(set_test->data,set_test->items,10) == 5 &&
        interpolation_fsearch(set_test->data,set_test->items,100) == 7 ) {
        test("interpolation_fsearch","match",TRUE);
    } else test("interpolation_fsearch","match",FALSE);
    
    if( exponential_fsearch(set_test->data,set_test->items,2) == 0 &&
        exponential_fsearch(set_test->data,set_test->items,5) == 2 &&
        exponential_fsearch(set_test->data,set_test->items,10) == 5 &&
        exponential_fsearch(set_test->data,set_test->items,100) == 7 ) {
        test("exponential_fsearch","match",TRUE);
    } else test("exponential_fsearch","match",FALSE);
      
    // FSEARCH - TEST SEARCH MISS  
    if( linear_fsearch(set_test->data,set_test->items,1) == 0 &&
        linear_fsearch(set_test->data,set_test->items,8) == 4 &&
        linear_fsearch(set_test->data,set_test->items,12) == 6 &&
        linear_fsearch(set_test->data,set_test->items,55) == 7 ) {
        test("linear_fsearch","miss",TRUE);
    } else test("linear_fsearch","miss",FALSE);
    
    if( binary_fsearch(set_test->data,set_test->items,1) == 0 &&
        binary_fsearch(set_test->data,set_test->items,8) == 4 &&
        binary_fsearch(set_test->data,set_test->items,12) == 6 &&
        binary_fsearch(set_test->data,set_test->items,55) == 7 ) {
        test("binary_fsearch","miss",TRUE);
    } else test("binary_fsearch","miss",FALSE);
    
    if( interpolation_fsearch(set_test->data,set_test->items,1) == 0 &&
        interpolation_fsearch(set_test->data,set_test->items,8) == 4 &&
        interpolation_fsearch(set_test->data,set_test->items,12) == 6 &&
        interpolation_fsearch(set_test->data,set_test->items,55) == 7 ) {
        test("interpolation_fsearch","miss",TRUE);
    } else test("interpolation_fsearch","miss",FALSE);
    
    if( exponential_fsearch(set_test->data,set_test->items,1) == 0 &&
        exponential_fsearch(set_test->data,set_test->items,8) == 4 &&
        exponential_fsearch(set_test->data,set_test->items,12) == 6 &&
        exponential_fsearch(set_test->data,set_test->items,55) == 7 ) {
        test("exponential_fsearch","miss",TRUE);
    } else test("exponential_fsearch","miss",FALSE);
    
    // FSEARCH - TEST SEARCH EOS
    if( linear_fsearch(set_test->data,set_test->items,123) == -1 &&
        linear_fsearch(set_test->data,set_test->items,456) == -1 &&
        linear_fsearch(set_test->data,set_test->items,101) == -1 &&
        linear_fsearch(set_test->data,set_test->items,789) == -1 ) {
        test("linear_fsearch","key>max(set)",TRUE);
    } else test("linear_fsearch","key>max(set)",FALSE);
    
    if( binary_fsearch(set_test->data,set_test->items,123) == -1 &&
        binary_fsearch(set_test->data,set_test->items,456) == -1 &&
        binary_fsearch(set_test->data,set_test->items,101) == -1 &&
        binary_fsearch(set_test->data,set_test->items,789) == -1 ) {
        test("binary_fsearch","key>max(set)",TRUE);
    } else test("binary_fsearch","key>max(set)",FALSE);
    
    if( interpolation_fsearch(set_test->data,set_test->items,123) == -1 &&
        interpolation_fsearch(set_test->data,set_test->items,456) == -1 &&
        interpolation_fsearch(set_test->data,set_test->items,101) == -1 &&
        interpolation_fsearch(set_test->data,set_test->items,789) == -1 ) {
        test("interpolation_fsearch","key>max(set)",TRUE);
    } else test("interpolation_fsearch","key>max(set)",FALSE);
    
    if( exponential_fsearch(set_test->data,set_test->items,123) == -1 &&
        exponential_fsearch(set_test->data,set_test->items,456) == -1 &&
        exponential_fsearch(set_test->data,set_test->items,101) == -1 &&
        exponential_fsearch(set_test->data,set_test->items,789) == -1 ) {
        test("exponential_fsearch","key>max(set)",TRUE);
    } else test("exponential_fsearch","key>max(set)",FALSE);
      
  /* make sure all dynamic memory is freed and all fds are closed */
  set_destroy(set_test);

  return (EXIT_SUCCESS);
}


