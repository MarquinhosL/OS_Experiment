#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>


int main()
{
printf("��ǰ����(p1)IDΪ%d\n",getpid());
pid_t fpid2;

fpid2 = fork();
if(fpid2 == 0){
	
	printf("��ǰ����(p2)IDΪ%d,������IDΪ%d\n",getpid(),getppid());
	pid_t fpid4,fpid5;
	fpid4 = fork();
	if(fpid4 == 0)
	{	
		printf("��ǰ����(p4)IDΪ%d,������IDΪ%d\n",getpid(),getppid());
	}
	else
	{
	fpid5 = fork();
	if(fpid5 == 0)
	{
		printf("��ǰ����(p5)IDΪ%d,������IDΪ%d\n",getpid(),getppid());
	}
	}
	}
else{

pid_t fpid3 = fork();
if(fpid3 > 0){
	printf("��ǰ����(p3)IDΪ%d,������IDΪ%d\n",fpid3,getpid());
	
}
}

return 0;
}
