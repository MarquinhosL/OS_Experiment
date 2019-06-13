
# 《操作系统》实验报告

------

  学 号：      16281015
  姓 名：      王子谦
  专 业：      计算机科学与技术
  学 院：      计算机与信息技术学院
               
               
  提交日期：   2019年6月13日

------

# 《操作系统》实验五

## 实验题目：

**一文件系统实验**

## 实验要求

本实验要求在模拟的I/O系统之上开发一个简单的文件系统。用户通过create, open, read等命令与文件系统交互。文件系统把磁盘视为顺序编号的逻辑块序列，逻辑块的编号为0至L − 1。I/O系统利用内存中的数组模拟磁盘。

## 实验思路：

本实验通过使用数组模拟磁盘，针对文件系统的几个操作对数组进行操作进行模拟实验。

## 数据结构

本实验中主要涉及到的数据结构有如下几种：

1. 磁盘数组disk。我们可以利用数组disk\[C\]\[H\]\[B\]构建磁盘模型，其中CHB分别表示柱面号，磁头号和扇区号。每个扇区大小为512字节。

2. 空闲文件描述符ldisk\[64\]，

3. FCB结构，为了便于对文件进行控制和管理，在文件系统内部，给每个文件惟一地设置一个文件控制块。

   ```c++
   typedef struct FCB
   {
   	struct FCB *next;
   	char fileName[30]; //文件名
   	int fileSize;  //文件长度
   	char fileKind[5];  //文件种类
   	int fileNumber; //文件描述符序号
   	int diskPosition; //磁盘位置
   	int writePosition;//写指针位置
   	char modificationTimes[30];  //修改时间
   	int isOpen; //是否打开,1表示打开状态，0表示未打开状态
   	int status; //状态位
   }*fcbNode;
   ```

   

4. FBB结构，为方便管理我自定义的缓冲区文件表，用于文件打开后，对文件数据的缓冲读写。

   ```c++
   typedef struct FBB
   {
   	struct FBB *next;
   	int fileSize;  //文件长度
   	int diskPosition; //磁盘位置
   	char fileName[30]; //文件名
   	char buffer[5120];//文件缓冲区内容
   	int writePosition;//写指针位置
   	char fileKind[5];  //文件种类
   }*fbbNode;
   ```

   

5. 初始化文件控制块函数 ，对文件控制块进行初始化操作。

   ```c++
   void initial(fcbNode &fcb,fbbNode &fbb)
   {
   	fcb = (fcbNode)malloc(sizeof(FCB));
   	fcb->next = NULL;
   	fbb = (fbbNode)malloc(sizeof(FBB));
   	fbb->next = NULL;
   	memset(ldisk,false, sizeof(ldisk));
   	memset(disk,0x00, sizeof(disk));
   	endPosition = 0;
   }
   ```

   

6. 文件创建函数。通过输入文件名，文件大小和读写权限进行文件创建。

   ```
   void fileCreate(fcbNode &fcb)
   {
   	fcbNode f1, f2;
   	f1 = fcb;
   	cout << "请输入创建文件的名称、大小和权限(r、w、rw),以空格分开!\n";
   	char fname[30],fkind[5];
   	int fsize;
   	cin >> fname >> fsize >> fkind;
   	while (f1)
   	{
   		if (strcmp(f1->fileName,fname)==0)
   		{
   			cout << "文件名重复，创建文件失败!\n";
   			return ;
   		}
   		if (!(f1->next))
   			break;
   		else
   			f1 = f1->next;
   	}
   	if (endPosition + fsize > MaxDisk)
   	{
   		cout << "磁盘剩余空间不足，创建文件失败!\n";
   		return;
   	}
   	f2 = (fcbNode)malloc(sizeof(FCB));
   	strcpy(f2->fileName, fname);
   	f2->fileSize = fsize;
   	strcpy(f2->fileKind, fkind);
   	for (i = 0; i < 64; i++)
   	{
   		if (ldisk[i] == false)
   			break;
   	}
   	f2->fileNumber = i+1;
   	ldisk[i] = true;
   	f2->diskPosition = endPosition;	
   	endPosition += fsize;
   	f2->writePosition = f2->diskPosition;
   	time_t now_time=time(NULL);  
   	//获取本地时间  
   	tm*  t_tm = localtime(&now_time);  
   	//转换为年月日星期时分秒结果，如图：  
   
   	strncpy(f2->modificationTimes,asctime(t_tm),30);
   	f2->isOpen = 0;
   	f2->status = 1;
   	f1->next = f2;
   	f1->next->next = NULL;
   	f1 = f1->next->next;
   	cout << "创建文件成功!\n";
   	cout<<"------------------------------------------------"<<endl;
   	showFiles(fcb);
   }
   ```

   

7. 文件删除函数。通过输入文件名，在磁盘中查找文件，将其磁盘信息删除，同时删除文件控制块中的相关信息。

   ```c++
   void fileDelete(fcbNode &fcb)
   {
   	cout << "请输入你要删除的文件名：\n";
   	string input;
   	cin >> input;
   	fcbNode f1,f2;
   	f1 = fcb;
   	int flag = 0;
   	while (f1->next != NULL)
   	{
   		if (f1->next->fileName == input)
   		{
   
   			if (f1->next->isOpen==1)
   			{
   				cout << "文件处在打开状态，无法删除!\n";
   				return;
   			}
   			int temp = endPosition;
   			endPosition -= f1->diskPosition;
   			for (i = f1->diskPosition; i < endPosition; i++)
   			{
   				disk[i] = disk[i + f1->fileSize];
   			}
   			for (i = endPosition; i < temp; i++)
   			{
   				disk[i] = 0x00;
   			}
   			ldisk[f1->fileNumber] = false;
   			f2 = f1->next;
   			f1->next = f2->next;
   			free(f2);
   			flag = 1;
   			cout << "删除文件成功!\n";
   			cout<<"------------------------------------------------"<<endl;
   			showFiles(fcb);
   			break;
   		}
   		f1 = f1->next;
   	}
   	if (flag == 0)
   		printf("找不到此文件,无法删除!\n");
   }
   
   ```

   

8. 展示所有文件信息函数，将存在于磁盘中，文件控制块中的文件遍历输出。

   ```c++
   void showFiles(fcbNode &fcb)
   {
   	fcbNode f1;
   	f1 = fcb->next;
   	while (f1)
   	{
   		cout << "文件名：" << f1->fileName << "\t\t文件大小：" << f1->fileSize << " B\t\t修改时间：" << f1->modificationTimes << endl;
   		if (f1->next != NULL)
   			f1 = f1->next;
   		else
   			break;
   	}
   }
   ```

   

9. 重命名函数，对已有文件进行重命名。

   ```c++
   void renameFile(fcbNode& fcb)
   {
   	cout << "请输入你要重命名的文件名：\n";
   	char input[30];
   	cin >> input;
   	fcbNode f1,f2;
   	f1 = fcb;
   	f2 = fcb;
   	int flag = 0;
   	while (f1->next != NULL)
   	{
   		if (strcmp(f1->next->fileName, input) == 0)
   		{
   
   			if (f1->next->isOpen == 1)
   			{
   				cout << "文件处在打开状态，无法重命名!\n";
   				return;
   			}
   			cout << "请输入新命名：\n";
   			cin >> input;
   			while (f2)
   			{
   				if (strcmp(f2->fileName, input ) == 0)
   				{
   					cout << "文件名重复，重命名文件失败!\n";
   					return;
   				}
   				if (!(f2->next))
   					break;
   				else
   					f2 = f2->next;
   			}
   			strcpy(f1->next->fileName, input);
   			flag = 1;
   			cout << "重命名文件成功!\n";
   			break;
   		}
   		f1 = f1->next;
   	}
   	if (flag == 0)
   		printf("找不到此文件,无法重命名!\n");
   }
   ```

   

10. 文件属性函数，查看指定文件的状态属性。

    ```c++
    void fileProperties(fcbNode& fcb)
    {
    	cout << "请输入你要查看属性的文件名：\n";
    	char input[30];
    	cin >> input;
    	fcbNode f1;
    	f1 = fcb;
    	int flag = 0;
    	while (f1->next != NULL)
    	{
    		if (strcmp(f1->next->fileName, input) == 0)
    		{
    			cout << "文件名：" << "\t文件描述符：" << "\t文件大小：" << "\t文件类型：" << "\t文件磁盘位置：" << "\t修改时间：" << "\t\t是否打开：" << endl;
    			cout << f1->next->fileName << "\t\t" << f1->next->fileNumber << "\t\t" << f1->next->fileSize << "\t\t" << f1->next->fileKind << "\t\t" << f1->next->diskPosition << "\t\t" << f1->next->modificationTimes << " " << f1->next->isOpen << endl;
    			flag = 1;
    			break;
    		}
    		f1 = f1->next;
    	}
    	if (flag == 0)
    		printf("找不到此文件,无法查看属性!\n");
    }
    ```

    

11. 打开文件操作函数，打开文件。该函数可用于后续的read, write,或close操作，在此函数中，对FBB缓冲区进行初始化，读取磁盘中数据，将数据存入到缓冲区。之后的操作便只在缓冲区中进行，直到文件关闭函数，写回磁盘。

    ```c++
    void openFile(fcbNode& fcb,fbbNode &fbb)
    {
    	cout << "请输入你要打开的文件名：\n";
    	char input[30];
    	cin >> input;
    	fcbNode f1;
    	f1 = fcb;
    	int flag = 0;
    	while (f1->next != NULL)
    	{
    		if (strcmp(f1->next->fileName, input) == 0)
    		{
    
    			if (f1->next->isOpen==1)
    			{
    				cout << "文件已经处于打开状态！" << endl;
    				return;
    			}
    			f1->next->isOpen = 1;
    			cout<<"ok"<<endl;
    			fbbNode fb;
    			fb = fbb;
    			while (fb->next)
    			{
    				fb = fb->next;
    			}
    			fbbNode fb1;
    			fb1 = (fbbNode)malloc(sizeof(FBB));
    			cout<<"ok"<<endl;
    			strcpy(fb1->fileName, f1->next->fileName);
    			fb1->writePosition=0;
    			fb1->fileSize = f1->next->fileSize;
    			cout<<fb1->fileSize<<endl;
    			fb1->diskPosition =f1->next->diskPosition;
    			strcmp(fb1->fileKind, f1->next->fileKind);
    			memset(fb1->buffer,0x00, sizeof(fb1->buffer));
    			for (i = 0; i < int(f1->next->fileSize); i++)
    				{
    					if(disk[i + f1->next->diskPosition])
    					{
    						fb1->buffer[i]=disk[i + f1->next->diskPosition];
    					}
    				}
    			fb->next = fb1;
    			cout <<"文件"<<input<<"已打开！"<< endl;
    			flag = 1;
    			//fb1 = fb->next;
    			fb->next->next=NULL;
    			break;
    		}
    	}
    	if (flag == 0)
    		printf("找不到此文件!\n");
    }
    ```

    

12. 读文件操作，此处读文件操作只使用从头开始读取。将已存放在缓冲区的数据读取出来。

    ```c++
    void readFile(fbbNode& fbb)
    {
    	cout << "请输入你要读取内容的文件名：\n";
    	char input[30];
    	cin >> input;
    	fbbNode f1;
    	f1 = fbb;
    	int flag = 0;
    	while (f1->next != NULL)
    	{
    		if (strcmp(f1->next->fileName, input) == 0)
    		{
    			if (strcmp(f1->next->fileKind, "w") == 0)
    			{
    				cout << "文件不可读!"<<endl;
    				return;
    			}
    
    			cout << "文件内容如下：\n";
    			for (i = 0; i < f1->next->fileSize; i++) 
    			{
    				if(f1->next->buffer[i])
    					cout << f1->next->buffer[i];
    			}
    			cout << endl;
    			flag = 1;
    			break;
    			
    		}
    		f1 = f1->next;
    	}
    	if (flag == 0)
    		printf("文件未打开或找不到此文件,无法读取文件!\n");
    }
    ```

    

13. 写文件操作，从指定的内存位置开始的数据写入指定文件。写操作从文件的读写指针指示的位置开始。此处采用数据覆盖，如果内存之前有数据，将被新写入的数据所覆盖。

    ```c++
    void writeFile(fbbNode& fbb)
    {
    	cout << "请输入你要写入内容的文件名和插入位置(用空格隔开)：\n";
    	char input[30];
    	int index;
    	cin >> input>>index;
    	fbbNode f1;
    	f1 = fbb;
    	int flag = 0;
    	while (f1->next != NULL)
    	{
    		if (strcmp(f1->next->fileName, input) == 0)
    		{
    			if (strcmp(f1->next->fileKind, "r") == 0)
    			{
    				cout << "文件不可写!" << endl;
    				return;
    			}
    			
    				cout << "请输入文件内容：" << endl;
    				string str;
    				cin >> str;
    				if (int(str.length()) > f1->next->fileSize)
    				{
    					cout << "写入内容超过文件大小，写入失败!\n";
    					return;
    				}
    				for (i = index; i < int(str.length()); i++)
    				{
    					f1->buffer[i] = str[i];
    				}
    				flag = 1;
    				cout << "写入文件成功!\n";
    				break;
    		}
    		f1 = f1->next;
    	}
    	if (flag == 0)
    		printf("文件未打开或找不到此文件,无法写入文件!\n");
    }
    ```

    

14. 关闭文件操作函数，此时文件将被修改为关闭操作，同时将缓冲区数据写回到原磁盘中。同时将删除FBB结构中对应数据。

    ```c++
    void closeFile(fcbNode& fcb,fbbNode &fbb)
    {
    	cout << "请输入你要关闭的文件名：\n";
    	char input[30];
    	cin >> input;
    	fbbNode f1,f2;
    	f1 = fbb;
    	int flag = 0;
    	while (f1->next != NULL)
    	{
    		if (strcmp(f1->next->fileName, input) == 0)
    		{
    			for (i = 0; i < int(f1->next->fileSize); i++)
    				{
    					disk[i + f1->next->diskPosition]=f1->buffer[i];
    				}
    			f2 =f1->next->next;
    			f1->next = f2;
    			free(f2);
    			cout <<"文件"<<input<<"已关闭！"<< endl;
    			flag = 1;
    			fcbNode fc1;
    			fc1 = fcb;
    			while (fc1->next != NULL)
    			{
    				if (strcmp(fc1->next->fileName, input) == 0)
    				{
    					cout<<"fiine"<<endl;
    					time_t now_time=time(NULL);  
    					//获取本地时间  
    					tm*  t_tm = localtime(&now_time);  
    					//转换为年月日星期时分秒结果，如图：  
    
    					strncpy(fc1->next->modificationTimes,asctime(t_tm),30);
    					fc1->next->isOpen=0;
    					break;
    				}
    				fc1=fc1->next;
    			}
    			break;
    		}
    		f1 = f1->next;
    		
    		
    	}
    	if (flag == 0)
    		printf("文件未打开或找不到此文件!\n");
    }
    ```

    

15. 显示磁盘分配情况函数。

    ```c++
    void showDisk(fcbNode& fcb)
    {
    	fcbNode f1;
    	f1 = fcb->next;
    	i = 1;
    	cout << "磁盘总大小为" << MaxDisk << "bytes,已使用" << endPosition << "bytes,还剩下" << MaxDisk - endPosition << "bytess!" << endl;
    	cout << "盘块号" << "\t起始地址" << "\t容量大小" << "\t是否使用\n";
    	cout << "0" << "\t" << endPosition << "\t\t" << MaxDisk - endPosition << "bytes\t" << "否"<<endl;
    	while (f1)
    	{
    		cout << i << "\t" << f1->diskPosition << "\t\t" << f1->fileSize << "bytes\t是" << endl;
    		i++;
    		if (f1->next != NULL)
    			f1 = f1->next;
    		else
    			break;
    	}
    	
    }
    ```

## 文件系统模拟结果

### 菜单展示效果如下：

通过输入相关序号选择相关功能进行模拟操作。
![](https://github.com/Vanzque/exp_pic/blob/master/lab5/1.png)


### 创建文件功能：

通过输入文件名称、大小和权限进行创建。
![](https://github.com/Vanzque/exp_pic/blob/master/lab5/2.png)


### 删除文件功能：
![](https://github.com/Vanzque/exp_pic/blob/master/lab5/3.png)


### 文件重命名功能：

![](https://github.com/Vanzque/exp_pic/blob/master/lab5/4.png)

### 打开文件功能

![](https://github.com/Vanzque/exp_pic/blob/master/lab5/5.png)

### 查看磁盘功能：

![](https://github.com/Vanzque/exp_pic/blob/master/lab5/6.png)

### 读取文件功能：

![](https://github.com/Vanzque/exp_pic/blob/master/lab5/7.png)

### 写文件功能

![](https://github.com/Vanzque/exp_pic/blob/master/lab5/8.png)

![](https://github.com/Vanzque/exp_pic/blob/master/lab5/9.png)


### 查看文件属性

![](https://github.com/Vanzque/exp_pic/blob/master/lab5/10.png)

### 关闭文件功能

![](https://github.com/Vanzque/exp_pic/blob/master/lab5/11.png)
