#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "genFloat.h"
#include "heapsort.h"

#define MAX_FLOATS_READ_IN_HEAP 250000
#define MAX_ALLOWED_SIZE 10000000
#define TOP_SCORE 10

float mean = 0.0;
float standard_deviation = 0.0;
float *floatList = NULL, *zscoreList=NULL;
heapNode *Node_list = NULL;


/* Merges two halves of the list of floating point numbers 
 * by using a temporary array called tmp */

void merge(float * lst, int a, int b, int s ) 
{
    float *tmp = NULL;
    tmp = (float *)malloc(sizeof(float)*s);

    int ti = a, ai = a, bi = b;
    
     while ((ai < b) || (bi < s)) 
     {   
        if (bi == s)
            tmp[ti++] = lst[ai++];
        else if (ai == b)  
            tmp[ti++] = lst[bi++];
        else if ((lst[ai]) > lst[bi])
            tmp[ti++] = lst[ai++];
        else 
            tmp[ti++] = lst[bi++];
      }   
    
     for (ti = a; ti < s; ti++)
          lst[ti] = tmp[ti];

     if(tmp != NULL) 
     {
        free(tmp);
        tmp = NULL;
     }
}





/* Mergesort divides the entire list of inputs into two halves recursively
 * and merges them again into a sorted list during the merge function */

void mergesort(float * lst, int a, int b)
{
     if (b-a < 2)
          return;

     mergesort(lst, a, a + (b-a)/2);
     mergesort(lst, a + (b-a)/2, b);
     merge(lst, a, a + (b-a)/2, b);
}



/* Creates a heap which takes one input from each sorted chunk and runs 
 * MAX_HEAPSORT on that heap. In a sorted Max Heap the largest number is always
 * on the end of the heap and that is written to the output.
 * The next number from the chunk is then read and a new heap is again generated
 * until all the numbers are written in sorted order 
 *
 * In addition we variance and standard deviation of the entire set of inputs and
 * call the zscore_calculator function*/


void create_sorted_heap_output (heapNode *A, int total_chunks, int max_floats)
{
    float variance=0.0, sum_of_variance=0.0;
    float *heap_max = NULL;
     
    FILE *fp_out = NULL;
    FILE *zscore_out= NULL;

    fp_out = openFile("merged_output.txt","w");
    zscore_out = openFile("variance_output.txt","wb");
    
    heap_max = (float *)malloc(sizeof(float)*total_chunks);

    for (int i=0;i<total_chunks;++i)
        *(heap_max+i) = *((A+i)->root_element);

    int j = total_chunks-1;         
    
    for (int i=0;i<max_floats;i++)
    {
        max_heapsort(heap_max, j+1);

        variance = *(heap_max+j) - mean ;
        sum_of_variance = sum_of_variance + (variance*variance);
        
        fprintf(fp_out,"%f\n",*(heap_max+j));
        fprintf(zscore_out,"%f,",variance);

        
        /* Reading the next floating point number to replace the max number written */
        
        for (int k = 0; k < total_chunks ; k++)
        { 
            if ( ((A+k)->root_element) != NULL )
            {
                if( (heap_max[j] == *((A+k)->root_element)) )
                {
                    if( ( !feof( (A+k)->root_index) ) )
                    {
                        fscanf((A+k)->root_index,"%f,",(A+k)->root_element);
                        heap_max[j] = *((A+k)->root_element);
                    }
                    else
                    {
                        j--;
                        
                        if ( ((A+k)->root_index) != NULL )
                        {
                            fclose( (A+k)->root_index );
                            (A+k)->root_index = NULL;
                        }

                        if ( ((A+k)->root_element) != NULL )
                        {
                            free((A+k)->root_element);
                            (A+k)->root_element = NULL;
                        }
                    }
                 break;
                    
                }
            }
        } 
    }

    /* Calculating standard deviation*/

    standard_deviation = sqrt(sum_of_variance);
    
    
    /* Setting pointers to NULL after freeing them */

    if (heap_max !=NULL) 
    { 
        free (heap_max);
        heap_max = NULL;

    }
    
    if (zscore_out != NULL)
    {
        fclose(zscore_out);
        zscore_out=NULL; 
    }
    
    if (fp_out != NULL)
    {
        fclose(fp_out);
        fp_out=NULL; 
    }
}


/* Calculates the zscore using the formula
 * zscore = variance / standard_deviation 
 * and write the output to file*/

void zscore_calculator (int total_zscores)
{
    FILE *zscore_input = NULL, *zscore_output = NULL;
    int i;

    zscore_input = openFile("variance_output.txt","rb");
    zscore_output = openFile("zscore_output.txt","w");

    zscoreList = (float *)malloc(sizeof(float));
    
    printf("\n ************************************************** \n");
    printf("\n The Top %d Z-Score are \n",TOP_SCORE);
    for(i=0;i<total_zscores;++i)
    {
        fscanf(zscore_input,"%f,",zscoreList);
        *zscoreList = *zscoreList / standard_deviation ;
        fprintf(zscore_output,"%f\n",*zscoreList);
        if(i<TOP_SCORE)
            printf("\n %f ",*zscoreList );
    }
    printf("\n\n The rest of the zscores can be found in zscore_output.txt\n");
    printf("\n ************************************************** \n");

    if (zscore_input !=NULL)
        fclose(zscore_input);
    
    if (zscore_output !=NULL)
        fclose(zscore_output);
} 




/* Creates a structure called Node_list. The elements of Node_list are root_index
 * and root_element
 *
 * root_index will contain a File pointer to a sorted chunk, while 
 * root_element will point to the floating point number in that chunk
 *
 * Node_list is then sent to create_sorted_heap_output function
 *  */


int call_heapsort (int total_chunks, int max_floats_read)
{
    char *filename_chunk = NULL;
    FILE *fp_list=NULL;
    int i, status;


    Node_list = (heapNode*)malloc(sizeof(heapNode)*total_chunks);
    Node_list->root_element = NULL;
    Node_list->root_index = NULL;

    filename_chunk = (char *)malloc(sizeof(char*));

    for(i=0;i<total_chunks;++i)
    {       
        Node_list->root_index = (FILE *)malloc(sizeof(FILE));
        Node_list->root_element = (float *)malloc(sizeof(float));
        
        sprintf(filename_chunk,"~tmp%d",i);
        fp_list = openFile(filename_chunk,"rb");
    
        Node_list->root_index = fp_list;

        fscanf(fp_list,"%f,",Node_list->root_element);
        if ((Node_list->root_element) == NULL)
        {
            perror("Root element is null");
            return -1;
        }
        fp_list++;
        Node_list++;
    }

    Node_list = Node_list - total_chunks;

    create_sorted_heap_output (Node_list, total_chunks, max_floats_read);

    printf ("\n Sorted Chunks are merged using Heap Sort and writen to output file = merged_output.txt\n");
    printf ("\n Variance of sorted Chunks are writen to output file = zscore_output.txt\n");
    
    
    /* Delete Temporary Files */
    
    for(i=0;i<total_chunks;++i)
    {
        sprintf(filename_chunk,"~tmp%d",i);
        status = remove(filename_chunk);
        if(status != 0)
        {
            printf("Unable to delete %s",filename_chunk);
            perror("Error");
        }
    }

    printf("\n All temporary merge file deleted successfully.\n");
       

   
    if (filename_chunk!=NULL)
    {
        free(filename_chunk);
        filename_chunk=NULL;
    }

    if(Node_list->root_element != NULL)
    {
        free(Node_list->root_element);
        Node_list->root_element = NULL;
    }

    if(Node_list->root_index != NULL)
    {
        fclose(Node_list->root_index);
        Node_list->root_index = NULL;
    }
    
    return 1;

}


/* Banner function to explain usage of the external-sort program */

void printBanner()
{
    printf("\n Usage Sequence 1: main g <total numbers to be generated> \n");
    printf(" e.g Usage Sequence 1: main g 100 or main g 1000000 \n");
    printf(" Usage Sequence 2.: main s\n");

    printf("g [ this option generates Floating point Numbers in file input.txt ]\n");
    printf("s [ this option sorts in descending order Floating point Numbers in small chunks using Merge Sort\n");
    printf("     Each sorted chunk is then persisted in files. In the next step, the algorithm performs Heap Sort to merge the externally sorted files]\n\n"); 

}





/* Main function which reads the inputs and creates sorted chunks and handles the case where number
 * of input is smaller than buffer size */

int main(int argc, char **argv)
{
    FILE *fp_read=NULL,*fp_write=NULL;                                          
    int num_of_floats_read,actual_nums_read,run_size,more_input,status;
    int i,j;
    char temp_output[15];                                   
    char *output_file;                                      
    char *input_file = "input.txt";                         
    clock_t t,t1;                                           
    double cpu_time_used;                                    
    
    
    if (argc > 3) 
    {
        printBanner();
        exit (1); 
    } else if ((argc == 3 ) && (( !strcmp(argv[1], "g"))))
    {
        if (!atoi(argv[2])) 
        {
            printf("\n Input Error : Received %s instead of Integer as input ", argv[2]);
            exit(1);
        }else if (atoi(argv[2]) > MAX_ALLOWED_SIZE )
        {
            printf("\n Input Error : Size out of range. Max supported range is 10 Million ");
            exit(1);
        } else 
        { 
            printf("\n Generating %d Random Floating point numbers .. \n", atoi(argv[2]));    
            genFloat(input_file, atoi(argv[2]));
            printf("\n %d Random Floating point numbers have been generated in %s\n", atoi(argv[2]), input_file);    
        }
    }
    else if ((argc == 2 ) && (( !strcmp(argv[1], "s") )))
    {
        fp_read = openFile(input_file,"r");
        if ( fp_read == NULL)
        {
            printf("\nUnable to open input file");
            printf("\nTry running with option -g first before selecting option s");
            exit(1);
        }
        else
        {
            printf("\n +--------------------------------------------------------------");
            printf("\n Performing z-score Normalization on the Floating Point Inputs "); 
            printf("\n +--------------------------------------------------------------");
            
            floatList = (float *)malloc(sizeof(float)*MAX_FLOATS_READ_IN_HEAP );
 
            num_of_floats_read = 0, run_size = 0, actual_nums_read = 0 ; 
    
    
            t = clock(); // Initial Time
            t1 = t;

            printf("\n Sorting Input in smaller chunks using Mergesort and writing into disk as temporary files\n");
            
            more_input = 1;

            while( more_input)
            {
                for (num_of_floats_read=0;num_of_floats_read<MAX_FLOATS_READ_IN_HEAP;num_of_floats_read++)
                {
                    if( fscanf(fp_read,"%f,",floatList+num_of_floats_read) == 0 )
                    {
                        printf("\n Invalid input received. Try again. \n");
                        exit(1);
                    }
                    mean += *(floatList+num_of_floats_read);
                    if( feof(fp_read) )
                    {
                        more_input = 0;
                        num_of_floats_read++;
                        break;
                    }
                }

                mergesort(floatList,0,num_of_floats_read);
                
                sprintf(temp_output,"~tmp%d",run_size);
                fp_write = openFile(temp_output,"wb");

                for (int j=0; j<num_of_floats_read; j++)
                    fprintf(fp_write,"%f,",*(floatList+j));

                #ifdef TIMER
                    t1 = clock() - t1; 
                    cpu_time_used = ((double)t1)/CLOCKS_PER_SEC; // Time in seconds
                    printf("\n Completed. Mergesort for chunk %d complete. Time taken was  %lf \n",run_size,cpu_time_used);
                #endif // TIMER 

                actual_nums_read += num_of_floats_read;
                run_size++;
                fclose(fp_write);
            }

            fclose(fp_read);

            t = clock() - t;    // Final Time
            cpu_time_used = ((double)t)/CLOCKS_PER_SEC; // Time in seconds

            printf("\n Total Time Taken to generate sorted chunks using Merge Sort is %lf secs\n",cpu_time_used);

            mean = mean/actual_nums_read;

            printf ("\n Now Calling HeapSort to Merge sorted chunks into one file\n");
            
            t = clock(); // Initial Time
            
            #ifdef DEBUG_ENABLED
                printf("run_size = %d actual_nums_read=%d", run_size, actual_nums_read);
            #endif // DEBUG_ENABLED
            
            
            if(run_size>1)
                call_heapsort (run_size, actual_nums_read);
            else
            {
                float variance=0.0, sum_of_variance=0.0;

                FILE *zscore_out = NULL;
                fp_write = openFile("merged_output.txt","w");
                zscore_out = openFile("variance_output.txt","wb");

                sprintf(temp_output,"~tmp%d",run_size-1);
                fp_read = openFile(temp_output,"r");

                for(i=0;i<actual_nums_read;++i)
                {
                    variance = *(floatList+i) - mean;
                    sum_of_variance = sum_of_variance + (variance*variance);
        
                    fprintf(fp_write,"%f\n",*(floatList+i));
                    fprintf(zscore_out,"%f,",variance);
                }

                standard_deviation = sqrt(sum_of_variance);

                if (fp_write != NULL) 
                {
                    fclose(fp_write);
                    fp_write = NULL;
                }
                if (zscore_out != NULL)
                {
                    fclose(zscore_out);
                    zscore_out = NULL;
                }
                
                printf ("\n Sorted Chunks are merged using Heap Sort and writen to output file = merged_output.txt\n");
                printf ("\n Variance of sorted Chunks are writen to output file = zscore_output.txt\n");
    
                status = remove(temp_output);
                
                if(status == 0)
                    printf("\n All temporary merge file deleted successfully.\n");
                else
                {
                    printf("Unable to delete %s",temp_output);
                    perror("Error");
                }
                
                if (fp_read != NULL )
                {
                    fclose(fp_read);
                    fp_read;
                }

            }
                    
            
            
            t = clock() - t;  // Final Time
            cpu_time_used = ((double)t)/CLOCKS_PER_SEC; // Time in seconds
            printf("\n Total time taken to Heapsort and write merged output file is %lf secs\n\n",cpu_time_used);

            zscore_calculator(actual_nums_read);
   
        } 

    } else 
    {
        printf ("\n**Input Error: Improper usage option\n\n");
        printBanner();

    }

    return 0;
}
