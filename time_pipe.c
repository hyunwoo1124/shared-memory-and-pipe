// Author: Hyun Woo Kim
// Class: CPSC351
// Homework: Process using pipe
// Due: 2/11/2019

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>   // to use time library function such as timeval
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/time.h>
struct timeval tvalBegin, tvalEnd, timeval_t, elapsedTime;

#define READ_END 0
#define WRITE_END 1

int main(int argc, char* argv[]){

  char *const paramList[] = {"/bin/ls", "-l", "/home/student/Desktop/CPSC351",NULL};
//  struct timeval tvalBegin, tvalEnd, timeval_t;
  // read and write 0 and 1 consecutively
  int fd[2];
  // Process id
  pid_t pid;

  // create the pipe
  if(pipe(fd) == -1){
    fprintf(stderr,"Pipe Failed\n");
    return 1;
  }
  // fork the child Process
  pid = fork();

  if(pid < 0){
    fprintf(stderr,"Fork Failed\n");
    return 1;
  }
  // parent process
  if(pid >0){
    wait(NULL);   //omit this if wrong
    close(fd[WRITE_END]);
    gettimeofday(&tvalEnd,NULL);
    read(fd[READ_END], &tvalEnd, sizeof(tvalEnd));
    timersub(&tvalEnd, &tvalBegin, &elapsedTime);
    printf("Time in microseconds using pipe:%ld microseconds\n",(elapsedTime.tv_sec));

    close(fd[READ_END]);

  }
  // child process
  else{
    close(fd[WRITE_END]);
    gettimeofday(&tvalBegin,NULL);
    write(fd[WRITE_END], &tvalBegin,sizeof(tvalBegin));
    //timersub(&tvalEnd, &tvalBegin, &elapsedTime);
    execvp("ls",paramList);

    close(fd[WRITE_END]);
    //printf("Time in microseconds using pipe:%ld microseconds\n",(elapsedTime.tv_sec));
  }
  // gcc -g time_pipe.c -o Pipe
  return 0;
}
