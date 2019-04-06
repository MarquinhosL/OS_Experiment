图片上传至https://github.com/Vanzque/exp_pic/blob/master/exp_3
如果没有出现请稍作等待


# 《操作系统》实验报告

------------ ---------------------- --
  学 号：      16281015
  姓 名：      王子谦
  专 业：      计算机科学与技术
  学 院：      计算机与信息技术学院
               
               
  提交日期：   2019年4月5日

------------ ---------------------- --

# 《操作系统》实验三

## 实验题目：

**一、同步问题一**

**实验要求**

1、通过fork的方式，产生4个进程P1,P2,P3,P4，每个进程打印输出自己的名字，例如P1输出“I am the process P1”。要求P1最先执行，P2、P3互斥执行，P4最后执行。通过多次测试验证实现是否正确。

**实验思路：**

根据题目所述的四个进程关系可以绘制以下流程图：

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/1.png)

**设计4个信号变量：**

  **信号名**         **信号初始值**   **信号作用**
------------------ ---------------- ------------------------------
  **P1\_finished**   **0**            **当p1完成才进行下一步操作**
  **Mutex**          **1**            **控制p2与p3的互斥**
  **P2\_to\_p4**     **0**            **P2结束信号**
  **P3\_to\_p4**     **0**            **P3结束信号**

**进程代码如下：**

```c
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
```



**主函数代码如下：**

```c
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

```



**实验结果展示：**

**正确执行顺序如下：T1();T2();T3();T4();**

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/2.png)

**执行顺序如下：T1();T2();T4();T3();**

**可以看到没有先执行T3的话，T4永远无法执行。**

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/3.png)

**执行顺序如下：T2();T3();T4();**

**可以看到P1不执行，之后其他都不执行**

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/4.png)

**。**

## 实验题目：

二、进程控制二

**实验要求**

2、火车票余票数ticketCount 初始值为1000，有一个售票线程，一个退票线程，各循环执行多次。添加同步机制，使得结果始终正确。要求多次测试添加同步机制前后的实验效果。

**实验过程与思路：**

1.  该实验关键在于信号量的创建，当两个线程对同一个区域进行修改时，由于不同的各种程序执行结果，将会导致最后结果与实际不符合。通过sem\_open创建一个信号来对共享区域进行同步控制。

**实验结果展示：**

**通过同步进程控制，得到正确的信息如下：**

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/5.png)

**当数值变大同样能得到正确结果**

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/6.png)

> 如果不通过同步控制，所得结果将会与实际不相符合如下（加入强制切换）：
>
> 代码如下：
>
> ```c
> void *sell() {
>     int i;
>     int num;
>     num =rand()%10000;
>     printf("此次买票人数:%d\n",num);
>     for (i = 0; i < num; i++) {
>         sem_wait(mySem);
>         counter--;
>         sem_post(mySem);
>     }
>     return NULL;
> }
> 
> void *refund() {
>     int i;
>     int num;
>     num =rand()%10000;
>     printf("此次退票人数:%d\n",num);
>     for (i = 0; i < num; i++) {
>         sem_wait(mySem);
>         counter++;
>         sem_post(mySem);
>     }
> 
>     return NULL;
> }
> ```
>
> ![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/7.png)
>
> 不加pthread\_yield(),将数值设大，同样能得到一样效果。
>
> 代码如下：
>
> ```c
> void *sell() {
>     int i;
>     int num;
>     int temp;
>     num =rand()%10000;
>     printf("此次买票人数:%d\n",num);
>     for (i = 0; i < num; i++) {
> 	counter--;
>     }
>     return NULL;
> }
> 
> void *refund() {
>     int i;
>     int num;
>     int temp;
>     num =rand()%10000;
>     printf("此次退票人数:%d\n",num);
>     for (i = 0; i < num; i++) {
> 	counter++;
>     }
>     return NULL;
> 
> ```
>
> ![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/8.png)

## 实验题目：

三、同步控制三

**实验要求**

一个生产者一个消费者线程同步。设置一个线程共享的缓冲区， char buf\[10\]。一个线程不断从键盘输入字符到buf,一个线程不断的把buf的内容输出到显示器。要求输出的和输入的字符和顺序完全一致。（在输出线程中，每次输出睡眠一秒钟，然后以不同的速度输入测试输出是否正确）。要求多次测试添加同步机制前后的实验效果。

**实验过程与思路：**

我们需要设计的是一个生产者与消费者模型。根据分析，对于两个不同进程，当输入时不能进行读取，因此需要设计互斥信号，同时对于缓冲区的大小需要限制，当缓冲区没有空间时，输入阻塞。当缓冲区为空时，输出阻塞。从而设计两个信号来控制，初值分别为缓冲区大小和0.

实验代码如下：

```c

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
```

**实验结果展示：**

测试结果：

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/9.png)

对此加入sleep(1);输出时间有所延迟，其他正常运行。效果如下：

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/10.png)



## 实验题目：

四、进程操作四

**实验要求**

进程通信问题。阅读并运行共享内存、管道、消息队列三种机制的代码

实验测试

a）通过实验测试，验证共享内存的代码中，receiver能否正确读出sender发送的字符串？如果把其中互斥的代码删除，观察实验结果有何不同？如果在发送和接收进程中打印输出共享内存地址，他们是否相同，为什么？

b)有名管道和无名管道通信系统调用是否已经实现了同步机制？通过实验验证，发送者和接收者如何同步的。比如，在什么情况下，发送者会阻塞，什么情况下，接收者会阻塞？

c）消息通信系统调用是否已经实现了同步机制？通过实验验证，发送者和接收者如何同步的。比如，在什么情况下，发送者会阻塞，什么情况下，接收者会阻塞？

**实验过程与思路：**

**a)源代码已给出。**

> **实验步骤：**通过测试可以看出，receive和send能正常接受字符串
>
> ```c
> Value=semctl(sem_id, 0, GETVAL)
> ```
>
> 通过以上代码获取当前状态
>
> ```c
> semop(sem_id, &sem_b, 1)
> ```
>
> 通过以上代码来进行P或V操作
>
> Send:
>
> ```c
> //6. Operation procedure
>     struct sembuf sem_b;
>     sem_b.sem_num = 0;      //first sem(index=0)
>     sem_b.sem_flg = SEM_UNDO;
>     sem_b.sem_op = 1;           //Increase 1,make sem=1
> 
> ```
>
> Receive:
>
> ```c
> //6. Operation procedure
>     struct sembuf sem_b;
>     sem_b.sem_num = 0;      //first sem(index=0)
>     sem_b.sem_flg = SEM_UNDO;
> sem_b.sem_op = -1;           //Increase 1,make sem=1
> 
> ```
>
> 

根据我们所给的结构体可以看到：sem\_op&lt;0时，执行V操作，sem\_op&gt;0执行P操作。

> **Send：**

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/11.png)

Recieve：

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/12.png)

可以看到当send输入多个字符串的时候，receive会依次输出，没有出现问题。Send中多出现了两次提醒输入。这是由于提前输入了字符串，每次receive之后sem\_op=0,send进程进行操作，由于已经输入，因此出现了多的两次提醒。

**将互斥代码删除：**

**Send代码如下：（receive类似）**

```c
/*
 * Filename: Receiver.c
 * Description: 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char *argv[])
{
    key_t  key;
    int shm_id;
    int sem_id;
    int value = 0;

    //1.Product the key
    key = ftok(".", 0xFF);
    
    //2. Creat semaphore for visit the shared memory
    sem_id = semget(key, 1, IPC_CREAT|0644);
    if(-1 == sem_id)
    {
        perror("semget");
        exit(EXIT_FAILURE);
    }
    /*
    //3. init the semaphore, sem=0
    
    if(-1 == (semctl(sem_id, 0, SETVAL, value)))
    {
        perror("semctl");
        exit(EXIT_FAILURE);
    }
    
    //4. Creat the shared memory(1K bytes)
    shm_id = shmget(key, 1024, IPC_CREAT|0644);
    if(-1 == shm_id)
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    */
    //5. attach the shm_id to this process
    char *shm_ptr;
    shm_ptr = shmat(shm_id, NULL, 0);
    if(NULL == shm_ptr)
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    //6. Operation procedure
    struct sembuf sem_b;
    sem_b.sem_num = 0;      //first sem(index=0)
    sem_b.sem_flg = SEM_UNDO;
    sem_b.sem_op = -1;           //Increase 1,make sem=1
    
    while(1)
    {
        //if(1 == (value = semctl(sem_id, 0, GETVAL)))
        //{
            printf("the sharing address is %p\n",shm_ptr);
            printf("\nNow, receive message process running:\n");
            printf("\tThe message is : %s\n", shm_ptr);
            /*
            if(-1 == semop(sem_id, &sem_b, 1))
            {
                perror("semop");
                exit(EXIT_FAILURE);
            }
            */
            
        //}

        //if enter "end", then end the process
        if(0 == (strcmp(shm_ptr ,"end")))
        {
            printf("\nExit the receiver process now!\n");
            break;
        }
    }

    shmdt(shm_ptr);
    //7. delete the shared memory
    if(-1 == shmctl(shm_id, IPC_RMID, NULL))
    {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }

    //8. delete the semaphore
    if(-1 == semctl(sem_id, 0, IPC_RMID))
    {
        perror("semctl");
        exit(EXIT_FAILURE);
    }

    return 0;
}

```

将所有的setval,getval以及sem\_op操作全部注释。实验结果如下：

Send：

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/13.png)

Receive：

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/14.png)

可以看到由于没有互斥信号限制，receive端执行一次后不会停止，而是持续输出。

打印共享内存地址：

```c
printf("the sharing address is %p\n",shm_ptr);
```

输出结果显示相同：

Receive：

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/15.png)

Send：

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/16.png)

```c
#include <sys/ipc.h>
#include <sys/shm.h>
	
// 得到一个共享内存标识符或创建一个共享内存对象并返回共享内存标识符
int shmget(key_t key, size_t size, int shmflg)
// 成功：返回共享内存的标识符; 出错：-1，错误原因存于error中。
//根据代码中的参数可以看到：
shmget(key, 1024, IPC_CREAT|0644)

```

IPC\_CREAT：如果内核中不存在键值与key相等的共享内存，则新建一个共享内存；如果存在这样的共享内存，返回此共享内存的标识符

0644,它表示允许一个进程创建的共享内存被内存创建者所拥有的进程向共享内存读取和写入数据，同时其他用户创建的进程只能读取共享内存。

b\) 源代码已给出。

有名管道是当以只读方式打开管道时会一直阻塞到有其他地方以写打开的时候。利用这个特性便可以实现进程同步。如果一个进程读一个空管道，read函数会阻塞，直到管道中有可用数据。如果一个进程想写一个已满管道，write函数会阻塞，直到管道中有足够的数据被读出来让write函数完成写操作。

产生文件如下：

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/17.png)

当写（读）端存在时，读（写）端执行时立即返回。如下：

Send：（wirte）

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/18.png)

recieve：（read）

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/19.png)

当写（读）端不存在时，读（写）端一直阻塞。如下：

Receive（read）

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/20.png)

Send（write）:

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/21.png)



c)初始源代码已给出。发送者和接收者之间能够实现同步。实验结果如下：

client:

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/22.png)

Server:

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/23.png)

两进程之间是通过消息队列来实现同步接收发送消息。

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/24.png)

消息队列是内核地址空间中的内部链表，通过linux内核在各个进程之间传递内容，消息顺序地发送到消息队列中，并且以几种不同的方式从队列中获取，每个消息队列可以用IPC标识符唯一的进行标识，内核中的消息队列是通过IPC的标识符来区别的，不同的消息队列之间是相互独立的，每个消息队列中的消息又构成一个独立的链表。

msgsnd(msgId, &sndBuf, strlen(buf)+1, 0)

根据函数结构与参数可以看到，我们设置的队列大小为128字节。发送端和s输出端超过队列大小就会造成阻塞。当我们一次键入了过多的字符后，可以发现阻塞。

Client:

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/25.png)

server：

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/26.png)

## 实验题目：

**阅读PINTOS源码**

**实验要求**

阅读Pintos操作系统，找到并阅读进程上下文切换的代码，说明实现的保存和恢复的上下文内容以及进程切换的工作流程。

从src中找到thread.c文件，对于一个新进程的创建可以看到进程机制的流程。从创建开始查看整个工作流程。

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/27.png)

thread\_create()函数创建一个新的线程。

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/28.png)

其中init\_thread (t, name, priority)对线程进行初始化，从参数可以看出初始化线程变量名以及优先级。

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/29.png)

list\_push\_back (&all\_list, &t-&gt;allelem);将线程加入all\_list队列。

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/30.png)

将当前创建线程添加至当前运行线程之后。

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/31.png)

给当前线程解除阻塞状态。

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/32.png)

将当前线程依据优先级添加至ready\_list就绪队列之中。

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/33.png)

![input txt](https://github.com/Vanzque/exp_pic/blob/master/exp_3/34.png)

当前正在执行的线程中断并将其依据优先级添加到就绪队列中。

通过以上一个创建新线程的过程就可看到进程上下文切换的流程。在Pintos中线程通过队列方式进行存储。线程依据其之间的优先级顺序执行，当前线程在执行的时候，一旦有线程创建或解阻塞。比较当前线程与其之间的优先级，若后者高，则将当前线程从执行状态转换成就绪状态存入就绪队列中，反之，则将新线程添加至就绪队列依据优先级进行等待执行。
