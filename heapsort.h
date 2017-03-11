#ifndef HEAPSORT_H_INCLUDED
#define HEAPSORT_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct heapNode{
        FILE* root_index;              //position of the floating point number in the file
        float *root_element;           //value
} heapNode ;
 

FILE* openFile(char* fileName, char* mode);

int parent (int i);

int left ( int i);

int right ( int i);

void swap ( float *px, float *py );

void display_heap(float *A, int len);

void max_heapify (float *A, int index, int len);

void build_max_heap(float *A, int len);

void max_heapsort (float *A, int len);

#endif
