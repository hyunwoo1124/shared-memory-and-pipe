// Author: Hyun Woo Kim
// Class: CPSC351
// Homework: Process using shared memory
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
//#pragma GCC diagnostic ignored "-Wwrite-strings"  // string to const conversion library used

int main(int argc, char** argv){
  // this will be used to output the ls
  char *const paramList[] = {"/bin/ls", "-l", "/home/student/Desktop/CPSC351/",NULL};
  // /bin/ls-l
  /*char* myargs[3];
  myargs[0] = strdup("./test ls -l");
  myargs[1] = strdup("test.c");
  myargs[2] = NULL;*/
  // struct to hold before and after time
  struct timeval tvalBegin, tvalEnd, timeval_t, elapsedTime;
  // create a process id identifier
  pid_t pid;
  pid = fork();
  char *name = "Lets find time elapsed";

  int shmFD;
  struct timeval * ptr;
  //shared memory file descriptor with creating and read and write and 0666
  shmFD = shm_open(name, O_CREAT | O_RDWR, 0666);

  // configure the size of the shared memory object
  ftruncate(shmFD, sizeof(tvalBegin));

  ptr = (struct timeval *)
    mmap(0, sizeof(timeval_t),PROT_READ | PROT_WRITE, MAP_SHARED,shmFD, 0);

  // if fork is negative value then error occured
  if(pid <0 ){
    perror("fork() error: something went wrong");
  }
  // this is the child process
  else if(pid == 0){
    // child will get the begin time
    gettimeofday(&tvalBegin,NULL);
  //  printf("Time in Child: %ld seconds\n", tvalBegin.tv_usec);
    printf("Time begin: %ld\n", tvalBegin.tv_sec);
    execvp("ls",paramList);
    //execvp(myargs[0],myargs);
    printf("erorr happen before execvp");
  }
  // this is the parent
  // parent will get the end time
  else if(pid > 0){
    wait(NULL);
    gettimeofday(&tvalEnd,NULL);
    printf("Time end: %ld\n", tvalEnd.tv_sec);
/*    printf("Time in microseconds:%ld microseconds\n",((tvalEnd.tv_sec-tvalBegin.tv_sec)*1000000L
      +tvalEnd.tv_usec) - tvalBegin.tv_usec); */
  //  struct timeval_t = tvalEnd.tv_sec - tvalBegin.tv_sec;
  timersub(&tvalEnd, &tvalBegin, &elapsedTime);
    printf("Elapsed time in microseconds: %ld\n",(elapsedTime.tv_sec, elapsedTime.tv_usec));

  }
  return 0;
//  g++ -g time_shm.c -o time -lrt
}
