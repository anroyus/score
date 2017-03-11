#define BOOSTFACTOR 90.0


/* The function process does image filtering in two steps.
   In the first step, it applies 2D Gaussian filter on the image. 
 * In the next step, it applies a 2-D symmetric Gaussian with standard deviation S on
 * the image and returns the output of the blurring in (*def).
 * The size of the image is rows x cols and S is the standard deviation for the
 * Gaussian, and the image is in 'image' stored in row-major.*/

void PROCESS(unsigned char *image, int rows, int cols, float S,
         short int *dx, short int *dy)
{
   short int *A;
             

   func1(image, rows, cols, S, &A);

   func3(A, rows, cols, &dx, &dy);

  
   /* free memory block created by func1 */
   free(A);

  
}




/* This function finds the derivatives in the x and y directions
 * after applying a 2D Gaussian filter on the image.
 * The derivative at each point is the diff between the value at
 * the next and the previous positions except at the edge. The 
 * derivative is applied on the output image that is obtained by applying the Gaussian.

 */

void func3(short int *def, int rows, int cols,
        short int **dx, short int **dy)
{
   int r, c, pos;


   for(r=0;r<rows;r++){
      pos = r * cols;
      (*dx)[pos] = def[pos+1] - def[pos];
      pos++;
      for(c=1;c<(cols-1);c++,pos++){
         (*dx)[pos] = def[pos+1] - def[pos-1];
      }
      (*dx)[pos] = def[pos] - def[pos-1];
   }


   for(c=0;c<cols;c++){
      pos = c;
      (*dy)[pos] = def[pos+cols] - def[pos];
      pos += cols;
      for(r=1;r<(rows-1);r++,pos+=cols){
         (*dy)[pos] = def[pos+cols] - def[pos-cols];
      }
      (*dy)[pos] = def[pos] - def[pos-cols];
   }
}


/* This function applies a 2-D symmetric Gaussian with standard deviation S on
 * the image and return the output of the blurring in (*def).
 * The size of the image is rows x cols and S is the standard deviation for the
 * Gaussian, and the image is in 'image' stored in row-major.
 */
void func1(unsigned char *image, int rows, int cols, float S,
        short int **def)
{
   int r, c, rr, cc,     
      WS,        
      C;            
   float *tempim,        
         *K,        
         dot,            
         sum;            

   magic(S, &K, &WS);
   C = WS / 2;

   /* Allocate space equal to the size of the image for intermdiate processing */
   if((tempim = (float *) calloc(rows*cols, sizeof(float))) == NULL){
      exit(1);
   }
   if(((*def) = (short int *) calloc(rows*cols,
         sizeof(short int))) == NULL){
      exit(1);
   }

   /* Apply the 1-D gaussian for each row; the image is store in row major */
   for(r=0;r<rows;r++){
      for(c=0;c<cols;c++){
         dot = 0.0;
         sum = 0.0;
         for(cc=(-C);cc<=C;cc++){
            if(((c+cc) >= 0) && ((c+cc) < cols)){
               dot += (float)image[r*cols+(c+cc)] * K[C+cc]; /* When the filter is applied the image is wrapped around */
               sum += K[C+cc];
            }
         }
         tempim[r*cols+c] = dot/sum; /* normalize the result */
      }
   }

   /* Apply the 1-D gaussian for each column on the intermidiate the image; the 
    * has the affect of applying a 2-D symmetric Gaussian on the image. The output
    * of the convolution is scaled by BOOSTFACTOR to store it in 16-bits.
    */
   for(c=0;c<cols;c++){
      for(r=0;r<rows;r++){
         sum = 0.0;
         dot = 0.0;
         for(rr=(-C);rr<=C;rr++){
            if(((r+rr) >= 0) && ((r+rr) < rows)){
               dot += tempim[(r+rr)*cols+c] * K[C+rr];
               sum += K[C+rr];
            }
         }
         (*def)[r*cols+c] = (short int)(dot*BOOSTFACTOR/sum + 0.5);
      }
   }

   free(tempim); /* Free the space for the intermidiate processing */
   free(K); /* Free the space of tge Guassian */
}

/* Generate the 1-D gaussian with standard deviation S. 
 * The filter is stored in (*K) amd *WS is the width of the
 * filter .
 */
void magic(float S, float **K, int *WS)
{
   int i, C;
   float x, fx, sum=0.0;

   /* Compute the width of the filter from S;  S is the standard deviation and C will be mean of the distribution */
   *WS = (int)(1 + 2 * ceil(2.5 * S));
   C = (*WS) / 2;

   if((*K = (float *) calloc((*WS), sizeof(float))) == NULL){
      exit(1);
   }

   /* Compute coefficients for a gaussian */
   for(i=0;i<(*WS);i++){
      x = (float)(i - C);
      fx = (float)(pow(2.71828, -0.5*x*x/(S*S)) / (S * sqrt(6.2831853))); /* fx = e^(-x^2/(2*S^2))/(S * sqrt(2*pi)) */
      (*K)[i] = fx;
      sum += fx;
   }

   /* Normalize the gaussian filter and the normalized filter is store in array pointed to by (*K) */
   for(i=0;i<(*WS);i++) (*K)[i] /= sum;

}
