#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "rdtsc.h"

#define DATA 32
#define ITERATIONS 10
#define LOOPS 1000

int OVERHEAD = 0;

int main(int argc , char *argv[]) {      
  if(argc != 3) {
    perror("USAGE: ./CLIENT-EXE <IP-ADDRESS> <PORT-NO>");
    return -1;
  }
  
  int port_num = atoi(argv[2]);
  unsigned long int start, end;
  unsigned cycles_low0, cycles_high0, cycles_low1, cycles_high1;
  unsigned long long total;
  unsigned long long final_avg;

  char temp_msg[DATA];
  memset(temp_msg, 'x', sizeof(temp_msg));

  // calculate the overhead clock cycles
  for (int j = 0; j < LOOPS; j++) {

      asm volatile ("cpuid\n\t"
        "rdtsc\n\t"
        "mov %%edx, %0\n\t"
        "mov %%eax, %1\n\t"
        : "=r" (cycles_high0), "=r" (cycles_low0)
        :: "%rax", "%rbx", "%rcx", "%rdx");

      asm volatile ("rdtscp\n\t"
        "mov %%edx, %0\n\t"
        "mov %%eax, %1\n\t"
        "cpuid\n\t"
        : "=r" (cycles_high1), "=r" (cycles_low1)
        :: "%rax", "%rbx", "%rcx", "%rdx");

      start = ( ((unsigned long int)cycles_high0 << 32) | cycles_low0 );
      end = ( ((unsigned long int)cycles_high1 << 32) | cycles_low1 );

      OVERHEAD += (end - start);
  }

  OVERHEAD = OVERHEAD/LOOPS;

  // calculate the RTT
  for(int idx=0; idx < ITERATIONS; idx++) {
    sleep(1);

    struct sockaddr_in server;
    server.sin_addr.s_addr =inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(port_num);
    int sockfd;

    sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sockfd == -1) {
      perror("ERROR: Socket create failed!\n");
      return -1;
    }

    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
      perror("ERROR: Socket connect failed!");
      return -1;
    }

    start = end = total = 0;

    for (int j = 0; j < LOOPS; j++) {

      asm volatile ("cpuid\n\t"
        "rdtsc\n\t"
        "mov %%edx, %0\n\t"
        "mov %%eax, %1\n\t"
        : "=r" (cycles_high0), "=r" (cycles_low0)
        :: "%rax", "%rbx", "%rcx", "%rdx");

      send(sockfd, &temp_msg, DATA, 0);
      recv(sockfd, &temp_msg, DATA, 0);

      asm volatile ("rdtscp\n\t"
        "mov %%edx, %0\n\t"
        "mov %%eax, %1\n\t"
        "cpuid\n\t"
        : "=r" (cycles_high1), "=r" (cycles_low1)
        :: "%rax", "%rbx", "%rcx", "%rdx");

      start = ( ((unsigned long int)cycles_high0 << 32) | cycles_low0 );
      end = ( ((unsigned long int)cycles_high1 << 32) | cycles_low1 );

      total += (end - start - OVERHEAD);
    }

    printf ("%d => RTT Cycles: %f ms\n", idx+1, total/(LOOPS*2.3e6));
    final_avg += total/LOOPS;
  }

  printf("Final Average: %f ms\n", final_avg/(ITERATIONS*2.3e6));
  return 0;
}
