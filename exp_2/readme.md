# 《操作系统》实验报告

------

  学 号：      16281015
  姓 名：      王子谦
  专 业：      计算机科学与技术
  学 院：      计算机与信息技术学院
               
               
  提交日期：   2019年3月18日

------

# 《操作系统》实验二

## 实验题目：

**一、进程控制一**

**实验要求**

1、打开一个vi进程。通过ps命令以及选择合适的参数，只显示名字为vi的进程。寻找vi进程的父进程，直到init进程为止。记录过程中所有进程的ID和父进程ID。将得到的进程树和由pstree命令的得到的进程树进行比较。

**实验思路：**

**当我们打开一个vi进行编写时，该过程就是一个进程。通过已知进程的关键字，利用ps –e|grep 查找对应进程号，再通过ps –ef|grep 进程号可以进行父进程的追溯。再通过pstree指令进行比较进程树是否一致。**

**实验结果展示：**

**使用vi编辑文件：**

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_2/1.png)

**以下是ps –ef|grep追溯结果**

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_2/2.png)

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_2/3.png)

![image text](https://github.com/Vanzque/exp_pic/blob/master/exp_2/4.png)

![image text](https://github.com/Vanzque/exp_pic/blob/master/exp_2/5.png)

**以下是pstree指令追溯结果：**

![image text](https://github.com/Vanzque/exp_pic/blob/master/exp_2/6.png)

![image text](https://github.com/Vanzque/exp_pic/blob/master/exp_2/7.png)

![image text](https://github.com/Vanzque/exp_pic/blob/master/exp_2/8.png)

![image text](https://github.com/Vanzque/exp_pic/blob/master/exp_2/9.png)

**可以看出来二者追溯得到的进程树一致。**

## 实验题目：

二、进程控制二

**实验要求**

2、编写程序，首先使用fork系统调用，创建子进程。在父进程中继续执行空循环操作；在子进程中调用exec打开vi编辑器。然后在另外一个终端中，通过ps –Al命令、ps aux或者top等命令，查看vi进程及其父进程的运行状态，理解每个参数所表达的意义。选择合适的命令参数，对所有进程按照cpu占用率排序。

**实验过程与思路：**

1. 该实验关键在于对fork()函数的理解，fork（）函数通过系统调用创建一个与原来进程几乎完全相同的进程，也就是两个进程可以做完全相同的事。返回值有两个，返回值为0时，表示为子进程。返回值&gt;0时，为父进程，且返回值为子进程Id.该实验对fork返回值进行条件分支比较。

> **源代码：**
>
> ```c
> #include <unistd.h>
> 
> #include <stdio.h>
> 
> int main()
> 
> {
> 
> int count=0;
> 
> pid_t fpid;
> 
> fpid = fork();
> 
> if(fpid==0){
> 
> int ret;
> 
> ret = execl ("/usr/bin/vi", "vi","/home/vanzque/new.txt", NULL);
> 
> if (ret == -1)
> 
> printf("GG");
> 
> }
> 
> else if(fpid&gt;0)
> 
> {
> 
> while(1)
> 
> {count++;count--;}
> 
> }
> 
> return 0;
> 
> }
> ```
>
> 

**实验结果展示：**

**通过pstree查看其进程树：**

![image text](https://github.com/Vanzque/exp_pic/blob/master/exp_2/10.png)

**通过 ps aux指令查看进程状态信息，在下图中，由于之前操作另一个进程未关闭，其状态为R+，表示在后台正在运行，另一个R表示正在运行。**

> ![image text](https://github.com/Vanzque/exp_pic/blob/master/exp_2/11.png)
>
> 再通过ps aux –sort=-%cpu 按照cpu占用由大到小排序。如下图所示。
>
> ![image text](https://github.com/Vanzque/exp_pic/blob/master/exp_2/12.png)

## 实验题目：

三、进程控制三

**实验要求**

使用fork系统调用，创建如下进程树，并使每个进程输出自己的ID和父进程的ID。观察进程的执行顺序和运行状态的变化。

![image text](https://github.com/Vanzque/exp_pic/blob/master/exp_2/13.png)

**实验过程与思路：**

**个人思路流程图如下：**
![image text](https://github.com/Vanzque/exp_pic/blob/master/exp_2/14.png)

由以上流程图可设计条件语句进行实现：

**有问题的源代码：**

```c
#include <unistd.h>

#include <stdio.h>

#include <sys/types.h>

#include <stdlib.h>

/*

疑惑。。。如果不加sleep(1),所有else的父进程都是1615。

*/

int main()

{

printf("当前进程(p1)ID为%d\\n",getpid());

pid_t fpid2;

//sleep(1);

fpid2 = fork();

if(fpid2 == 0){

printf("当前进程(p2)ID为%d,父进程ID为%d\\n",getpid(),getppid());

pid_t fpid4,fpid5;

sleep(1);

fpid4 = fork();

if(fpid4 == 0)

{ 

printf("当前进程(p4)ID为%d,父进程ID为%d\\n",getpid(),getppid());

}

else

{

sleep(1);

fpid5 = fork();

if(fpid5 == 0)

{

printf("当前进程(p5)ID为%d,父进程ID为%d\\n",getpid(),getppid());

}

}

}

else{

sleep(1);

pid_t fpid3 = fork();

if(fpid3 == 0){

printf("当前进程(p3)ID为%d,父进程ID为%d\\n",getpid(),getppid());

}

}

return 0;

}
```



**没问题的源代码：**

```c
#include <unistd.h>

#include <stdio.h>

#include <sys/types.h>

#include <stdlib.h>

int main()

{

printf("当前进程(p1)ID为%d\\n",getpid());

pid_t fpid2;

fpid2 = fork();

if(fpid2 == 0){

printf("当前进程(p2)ID为%d,父进程ID为%d\\n",getpid(),getppid());

pid_t fpid4,fpid5;

fpid4 = fork();

if(fpid4 == 0)

{ 

printf("当前进程(p4)ID为%d,父进程ID为%d\\n",getpid(),getppid());

}

else

{

fpid5 = fork();

if(fpid5 == 0)

{

printf("当前进程(p5)ID为%d,父进程ID为%d\\n",getpid(),getppid());

}

}

}

else{

pid_t fpid3 = fork();

if(fpid3 &gt; 0){

printf("当前进程(p3)ID为%d,父进程ID为%d\\n",fpid3,getpid());

}

}

return 0;

}
```



**实验结果展示：**

![image text](https://github.com/Vanzque/exp_pic/blob/master/exp_2/15.png)

## 实验题目：

四、进程操作四

**实验要求**

修改上述进程树中的进程，使得所有进程都循环输出自己的ID和父进程的ID。然后终止p2进程(分别采用kill -9 、自己正常退出exit()、段错误退出)，观察p1、p3、p4、p5进程的运行状态和其他相关参数有何改变。

**实验过程与思路：**

**源代码：**

```c
#include &lt;unistd.h&gt;

#include &lt;stdio.h&gt;

#include &lt;sys/types.h&gt;

#include &lt;stdlib.h&gt;

int main()

{

printf("当前进程(p1)ID为%d\\n",getpid());

pid_t fpid2;

fpid2 = fork();

if(fpid2 == 0){

printf("当前进程(p2)ID为%d,父进程ID为%d\\n",getpid(),getppid());

pid_t fpid4,fpid5;

fpid4 = fork();

if(fpid4 == 0)

{ 

printf("当前进程(p4)ID为%d,父进程ID为%d\\n",getpid(),getppid());

}

else

{

fpid5 = fork();

if(fpid5 == 0)

{

printf("当前进程(p5)ID为%d,父进程ID为%d\\n",getpid(),getppid());

}

}

}

else{

pid_t fpid3 = fork();

if(fpid3 &gt; 0){

printf("当前进程(p3)ID为%d,父进程ID为%d\\n",fpid3,getpid());

}

}

while(1)

{


printf("当前进程ID为%d,父进程ID为%d\\n",getpid(),getppid());

}

return 0;

}
```



**实验结果展示：**

**输出结果如下：**

![image text](https://github.com/Vanzque/exp_pic/blob/master/exp_2/16.png)

**进程树如下：**

![image text](https://github.com/Vanzque/exp_pic/blob/master/exp_2/17.png)

**使用kill -9后：p2结束，并且P4,P5的父进程变成了1615。（此处代码循环结构进行了修改）**

**\

**![image text](https://github.com/Vanzque/exp_pic/blob/master/exp_2/81.png)

**各个进程状态如下：**

> **P2进程变成了Z+表示该进程处于"Zombie"状态**

![image text](https://github.com/Vanzque/exp_pic/blob/master/exp_2/19.png)

> 使用exit(0):
>
> 代码需修改处：
>
> ![image text](https://github.com/Vanzque/exp_pic/blob/master/exp_2/20.png)
>
> Z+表示该进程处于"Zombie"状态
>
> ![image text](https://github.com/Vanzque/exp_pic/blob/master/exp_2/21.png)

**（实验疑惑）此处发现P4,P5的父节点并没有改变，说明P2并没有正常结束。但是其状态已经是zombie了。**

> **纠正：**
>
> exit(0)应加在while循环中。以下截图P1也zombie是由于没有写循环了。
>
> ![image text](https://github.com/Vanzque/exp_pic/blob/master/exp_2/22.png)
>
> ![image text](https://github.com/Vanzque/exp_pic/blob/master/exp_2/23.png)

**段错误退出：**

**此处对指针进行错误操作。函数取自百科段错误例子**

![image text](https://github.com/Vanzque/exp_pic/blob/master/exp_2/24.png)

![image text](https://github.com/Vanzque/exp_pic/blob/master/exp_2/25.png)

**P4,P5父进程号变成1615**

**实验总结和心得**

> 通过此次实验，对于如何通过fork()创建线程有了了解，学习了父进程与子进程的创建关系与顺序。对于整体进程控制的使用操作有了更进一步的学习。
