#pragma OPTIMIZE OFF

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_ITERATION 1000000

int sizeList[] = { 4, 8, 16, 32, 64, 128, 256, 
  512, 1024, 2048, 4096, 
  8192, 16384, 32768, 65536, 
  131072, 262144, 524288
};

int strideList[] = { 4, 16, 32, 64, 128, 256, 1024, 4096, 16384, 65536, 1048576, 4194304, 16777216};

double randomAccessTime(int size, int stride_length)
{
  int *array = (int *)malloc(size*1024);
  int arraysize = (size*1024)/sizeof(array[0]);
  memset(array, 0, arraysize);

  // set values to array to stride through it
  int pos = 0; 
  for(int i = stride_length; i < arraysize; i += stride_length)
  {
    array[pos] = i;
    pos = i;
  }
  array[pos] = 0;
  array[arraysize - 1] = 0; 

  int idx = 0;
  int i = 0;
  int t = 0;
  unsigned cycles_low0, cycles_high0, cycles_low1, cycles_high1;
  unsigned long int start, end;
  unsigned long int overhead = 0;

  asm volatile ("cpuid\n\t"
      "rdtsc\n\t"
      "mov %%edx, %0\n\t"
      "mov %%eax, %1\n\t"
      : "=r" (cycles_high0), "=r" (cycles_low0)
      :: "%rax", "%rbx", "%rcx", "%rdx");

  for (i = 0; i < MAX_ITERATION; i++) {
    idx = t;
  }

  asm volatile ("rdtscp\n\t"
      "mov %%edx, %0\n\t"
      "mov %%eax, %1\n\t"
      "cpuid\n\t"
      : "=r" (cycles_high1), "=r" (cycles_low1)
      :: "%rax", "%rbx", "%rcx", "%rdx");

  start = ( ((unsigned long int)cycles_high0 << 32) | cycles_low0 );
  end = ( ((unsigned long int)cycles_high1 << 32) | cycles_low1 );
  overhead = end-start;

  double avg = 0;

  for(int x = 0; x < 100; x++) {

    asm volatile ("cpuid\n\t"
        "rdtsc\n\t"
        "mov %%edx, %0\n\t"
        "mov %%eax, %1\n\t"
        : "=r" (cycles_high0), "=r" (cycles_low0)
        :: "%rax", "%rbx", "%rcx", "%rdx");

    for (i = 0; i < MAX_ITERATION; i++) {
      t = array[idx];
      idx = t;
    }

    asm volatile ("rdtscp\n\t"
        "mov %%edx, %0\n\t"
        "mov %%eax, %1\n\t"
        "cpuid\n\t"
        : "=r" (cycles_high1), "=r" (cycles_low1)
        :: "%rax", "%rbx", "%rcx", "%rdx");

    start = (((unsigned long int)cycles_high0 << 32) | cycles_low0 );
    end = (((unsigned long int)cycles_high1 << 32) | cycles_low1 );
    avg += (end-start - overhead)/2.3e9;
  }

  free(array);
  return (avg/100);
}

int main(int argc, const char * argv[])
{	
  int size_index = 0;
  int stride_index = 0;
  FILE *file;
  file = fopen("temp-result.txt", "w");
  for (size_index = 0; size_index < sizeof(sizeList)/sizeof(sizeList[0]); size_index++) {
    fprintf(file, "Size: %dKB\n",sizeList[size_index]);
    for (stride_index = 0; stride_index < sizeof(strideList)/sizeof(strideList[0]); stride_index++) {
      double averageTime = randomAccessTime(sizeList[size_index] * 1024, strideList[stride_index]);
      fprintf(file, "%lf\n", averageTime);
      printf("%lf -> size: %dKB, stride: %d \n", averageTime, sizeList[size_index], strideList[stride_index]);
    }
    fprintf(file, "\n");
  }

  fclose(file);   
  return 0;
}


