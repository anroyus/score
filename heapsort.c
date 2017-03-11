#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "heapsort.h"

//#define DEBUG_ENABLED



/* This function opens a file in the specified mode and returns error if file cannot be opened */

FILE* openFile(char* fileName, char* mode)
  {
      FILE* fp = fopen(fileName, mode);
      if (fp == NULL)
      {
          printf("Error while opening %s in mode =%s \n", fileName, mode);
          exit(EXIT_FAILURE);
      }
      return fp;
  }



/* Various functions for heap sorting */

int parent (int i)
{
    return (i/2);
}

int left ( int i)
{
    return (2*i + 1);
}
int right ( int i)
{
    return (2*i+2);    
}    



/* Swaps two floating point numbers */

void swap ( float *px, float *py)
{
    float tmp;

    tmp = *px;
    *px = *py;
    *py = tmp;
}




/* Displays the current heap */

void display_heap(float *A, int len)
{
    printf ("\n Displaying current heap and its elements \n");
    
    for (int i = 0; i < len; i++)
        printf("Inside A[%d] - value is  = %f\n",i, *(A+i));
    
}



/* Finds the maximum element in a heap by checking whether each node in the heap
 * is the largest of its two child nodes. If one of the child nodes is larger it swaps
 * the  */

void max_heapify (float *A, int index, int len)
{
    int i = index;
    int l, r, largest_index ;

    l = left(i);
    r = right(i);
    largest_index = i;
    
    if (l < len )
    { 
        if (*(A+l) > *(A+i))
            largest_index = l;    
        else
          largest_index = i;
    }

    if (r< len)
    { 
            if (*(A+r) > *(A+largest_index))
                largest_index= r;
    }
    
    if (largest_index != i )
    {
        swap ( (A+i), (A+largest_index));
        max_heapify (A, largest_index, len);
    } 

}


/* Function to create a max heap by continually calling the max_heapify function */

void build_max_heap(float *A, int len)
{
    for ( int i = (len)/2-1; i>=0; i--)        
        max_heapify (A, i, len);
}



/* Creates a sorted max heap with the largest value at the end of the heap */

void max_heapsort (float *A, int len)    

{
    build_max_heap(A, len);
    for ( int i = len-1; i>=0; i--)
    { 
        swap( A, (A+i));
        len = len -1;
        max_heapify (A, 0, len);
    }
}
