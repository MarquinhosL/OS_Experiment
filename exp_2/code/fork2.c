#include <unistd.h>
#include <stdio.h>

int main()
{
	int count=0;
 	pid_t fpid;
	fpid = fork();
	if(fpid==0){
		int ret;
		ret = execl ("/usr/bin/vi", "vi","/home/vanzque/new.txt", NULL);
		if (ret == -1)
			printf("GG");

	}
	else if(fpid>0)
		{
			while(1)
			{count++;count--;}
		}
return 0;
}

