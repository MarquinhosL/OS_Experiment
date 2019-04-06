#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <time.h>
sem_t *p1_finished,*mutex,*p2_to_p4,*p3_to_p4;
void T1()
{
    pid_t p1 = fork();
    if(p1==0){printf("I'm the process P1\n");}
    else{
        sleep(5);exit(0);}
    
    sem_post(p1_finished);
}
void T2(){
    sem_wait(p1_finished);
    sem_wait(mutex);
    pid_t p2 = fork();
    if(p2==0){printf("I'm the process P2\n");}
    else{
        sleep(5);exit(0);}
    
    sem_post(mutex);
    sem_post(p2_to_p4);
}
void T3(){
    sem_wait(p1_finished);
    sem_wait(mutex);
    pid_t p3 = fork();
    if(p3==0){printf("I'm the process P3\n");}
    else{
        sleep(5);exit(0);}
    
    sem_post(mutex);
    sem_post(p3_to_p4);
}
void T4(){
    sem_wait(p2_to_p4);
    sem_wait(p3_to_p4);
    pid_t p4 = fork();
    if(p4==0){printf("I'm the process P4\n");}
    else{
        sleep(5);exit(0);}
    
}
int main(){
    
    p1_finished=sem_open("p1_finished",O_CREAT,0666,0);
    mutex=sem_open("mutex",O_CREAT,0666,1);
    p2_to_p4=sem_open("p2_to_p4",O_CREAT,0666,0);
    p3_to_p4=sem_open("p3_to_p4",O_CREAT,0666,0);

    T1();T2();T3();T4();

    sem_close(p1_finished);
    sem_close(p2_to_p4);
    sem_close(p3_to_p4);
    sem_close(mutex);
    sem_unlink("p1_finished");
    sem_unlink("p2_to_p4");
    sem_unlink("p3_to_p4");
    sem_unlink("mutex");
    
	return 0;
}
