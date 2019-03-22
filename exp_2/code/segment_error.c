#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

dummy_function (void)
{
unsigned char *ptr = 0x00;
*ptr = 0x00;
}

int main()
{

pid_t fpid2;

fpid2 = fork();
if(fpid2 == 0){
	
	printf("当前进程(p2)ID为%d,父进程ID为%d\n",getpid(),getppid());
	pid_t fpid4,fpid5;
	fpid4 = fork();
	if(fpid4 == 0)
	{	while(1){
		printf("当前进程(p4)ID为%d,父进程ID为%d\n",getpid(),getppid());}
	}
	else
	{
	fpid5 = fork();
	if(fpid5 == 0)
	{	while(1){
		printf("当前进程(p5)ID为%d,父进程ID为%d\n",getpid(),getppid());}
	}
	}
	
	while(1){
	printf("当前进程(p2)ID为%d,父进程ID为%d\n",getpid(),getppid());
	//设置数组越界
	dummy_function ();	
	}
	
	}
else{

pid_t fpid3 = fork();
if(fpid3 > 0){
	while(1){
	printf("当前进程(p3)ID为%d,父进程ID为%d\n",fpid3,getpid());}
	
}
}
while(1){
printf("当前进程(p1)ID为%d\n",getpid());}
return 0;
}

