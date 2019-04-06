#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <time.h>


char buff[10];
int i=0;
int j=0;
sem_t *buff_num1 =NULL;
sem_t *buff_empty1 =NULL;

volatile int counter = 10;
void *input_char() {
    while(1)
    {
	sem_wait(buff_empty1);
        scanf(" %c",&buff[i]);
        i=(i+1)%10;
        sem_post(buff_num1);


    }
    return NULL;
}

void *output_char() {
    while(1)
    {
	
	sem_wait(buff_num1);
        printf("%c\n",buff[j]);
	j=(j+1)%10;
	sleep(1);
        sem_post(buff_empty1);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    buff_num1 = sem_open("buff_num9",O_CREAT,0666,0);
    buff_empty1 = sem_open("buff_empty9",O_CREAT,0666,10);
    pthread_t p1, p2;

    pthread_create(&p1, NULL, input_char, NULL);
    pthread_create(&p2, NULL, output_char, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    sem_close(buff_num1);
    sem_close(buff_empty1);
    sem_unlink("buff_num9");
    sem_unlink("buff_empty9");
    return 0;
}
