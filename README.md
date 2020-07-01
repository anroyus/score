# zscore normalization of Floating point inputs.

The program is written in GNU C and does not use any STL.
The code has been tested in both Linux as well Windows. 
The program can handle any large size Floating point inputs, (e.g  10 Million+ ) without constraining the in-memory.

This is a command line driven program which can be invoked in 2 steps.

1. How to compile :

In Linux systems execute commands 1.1,1.2 in order : 
1.1. make clean
1.2. make

In Windows testing:

MINGW32 was used for compiler http://www.mingw.org
1.1. make 


How to use the program : 

Step 1: Generate Floating Point Numbers
   Example 1a) ./main g 10  [ this generates 10 random Floating point numbers and writes it in input.txt ]
   Example 1b) ./main g 1000000  [ this generates 1M random Floating point numbers and writes it in input.txt ]

Step 2: Perform z-score normalization on the input and write them in descending order.
  ./main s.  [ This produces 3 output files. 1. merged
  


Example 1a - Sample RUN with 10 Floating Point Numbers as input

Generation Step:

./main g 10


 Generating 10 Random Floating point numbers .. 

 10 Random Floating point numbers have been generated in input.txt

Run Step:

 ./main s


 Performing z-score Normalization on the Floating Point Inputs 

 Step 1: Sorting Input in smaller chunks using Mergesort and writing into disk as temporary files

 Total Time Taken to generate sorted chunks using Merge Sort is 0.000141 secs

 Now Calling HeapSort to Merge sorted chunks into one file

 Sorted Chunks are merged using Heap Sort and writen to output file = merged_output.txt

 Variance of sorted Chunks are writen to output file = zscore_output.txt

 All temporary merge file deleted successfully.

 Total time taken to Heapsort and write merged output file is 0.002633 secs


 ************************************************** 

 The Top 10 Z-Score are 

 0.438399 
 0.348807 
 0.297920 
 0.215797 
 0.192316 
 -0.103344 
 -0.281154 
 -0.282098 
 -0.375552 
 -0.451091 

 The rest of the zscores can be found in zscore_output.txt

 ************************************************** 
sys-prof1@sys-VirtualBox:~/final-zscore/external-sort$ 


 ************************************************** 

sys-prof1@sys-VirtualBox:~/final-zscore/external-sort$ ./main  g 1000000

 Generating 1000000 Random Floating point numbers .. 

 1000000 Random Floating point numbers have been generated in input.txt
pipa@pipa-VirtualBox:~/final-zscore/external-sort$ ./main s


 Performing z-score Normalization on the Floating Point Inputs 

 Sorting Input in smaller chunks using Mergesort and writing into disk as temporary files

 Total Time Taken to generate sorted chunks using Merge Sort is 0.833862 secs

 Now Calling HeapSort to Merge sorted chunks into one file

 Sorted Chunks are merged using Heap Sort and writen to output file = merged_output.txt

 Variance of sorted Chunks are writen to output file = zscore_output.txt

 All temporary merge file deleted successfully.

 Total time taken to Heapsort and write merged output file is 1.045705 secs


 ************************************************** 

 The Top 10 Z-Score are 

 0.001731 
 0.001731 
 0.001731 
 0.001731 
 0.001731 
 0.001731 
 0.001731 
 0.001731 
 0.001731 
 0.001731 

 The rest of the zscores can be found in zscore_output.txt






Note: Alternatively, user can edit the generated Input must be delimited by new-line
Output file has one less number than total input
# score
