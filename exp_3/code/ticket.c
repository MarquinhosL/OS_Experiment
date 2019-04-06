#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

sem_t *mySem =NULL;
volatile int counter = 10000;
void *sell() {
    int i;
    int num;
    num =rand()%10000;
    printf("此次买票人数:%d\n",num);
    for (i = 0; i < num; i++) {
        sem_wait(mySem);
        counter--;
        sem_post(mySem);
    }
    return NULL;
}

void *refund() {
    int i;
    int num;
    num =rand()%10000;
    printf("此次退票人数:%d\n",num);
    for (i = 0; i < num; i++) {
        sem_wait(mySem);
        counter++;
        sem_post(mySem);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    mySem = sem_open("mySemName",O_CREAT,0666,1);

    pthread_t p1, p2;
    printf("Initial value : %d\n", counter);
    pthread_create(&p1, NULL, sell, NULL);
    pthread_create(&p2, NULL, refund, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    sem_close(mySem);
    printf("Final value : %d\n", counter);
    sem_unlink("mySemName");
    return 0;
}
