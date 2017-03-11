#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "genFloat.h"

/* Generate Floating point number */
 
void genFloat(char* inputfile, int MAX_FLOATS)
{
    FILE *fp_write = NULL;
    fp_write = openFile(inputfile, "w");
    srand((unsigned int)time(NULL));
    
    float a = 5;
    char delimiter[] = ",";

    for (int i = 0; i < MAX_FLOATS; ++i)
    {    
        float r = (1+(float)rand()/(float)RAND_MAX) * a; 
        fprintf(fp_write, "%f", r);
        if (i < (MAX_FLOATS-1) )
            fprintf(fp_write, "%s", delimiter);
    }

    if(fp_write != NULL)
    {
        fclose(fp_write);
        fp_write = NULL;
    }
}

