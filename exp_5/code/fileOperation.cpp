#include "fileOperation.h"
int i;

void initial(fcbNode &fcb)
{
	fcb = (fcbNode)malloc(sizeof(FCB));
	fcb->next = NULL;
	memset(ldisk,false, sizeof(ldisk));
	memset(disk,0x00, sizeof(disk));
	endPosition = 0;
}

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
}

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
			break;
		}
		f1 = f1->next;
	}
	if (flag == 0)
		printf("找不到此文件,无法删除!\n");
}

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
			cout << f1->next->fileName << "\t\t" << f1->next->fileNumber << "\t\t" << f1->next->fileSize << "\t\t" << f1->next->fileKind << "\t\t" << f1->next->diskPosition << "\t\t" << f1->next->modificationTimes << "\t" << f1->next->isOpen << endl;
			flag = 1;
			break;
		}
		f1 = f1->next;
	}
	if (flag == 0)
		printf("找不到此文件,无法查看属性!\n");
}

void readFile(fcbNode& fcb)
{
	cout << "请输入你要读取内容的文件名：\n";
	char input[30];
	cin >> input;
	fcbNode f1;
	f1 = fcb;
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
			for (i = f1->next->diskPosition; i < f1->next->diskPosition + f1->next->fileSize; i++) 
			{
				cout << disk[i];
			}
			cout << endl;
			f1->next->isOpen = 1;
			flag = 1;
			break;
		}
		f1 = f1->next;
	}
	if (flag == 0)
		printf("找不到此文件,无法读取文件!\n");
}

void writeFile(fcbNode& fcb)
{
	cout << "请输入你要写入内容的文件名：\n";
	char input[30];
	cin >> input;
	fcbNode f1;
	f1 = fcb;
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
			if (f1->next->isOpen == 1) 
			{
				cout << "文件已打开，不可写入!" << endl;
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
			for (i = 0; i < int(str.length()); i++)
			{
				disk[i + f1->next->writePosition] = str[i];
			}
			f1->next->writePosition += str.length();
			f1->next->isOpen = 1;
			flag = 1;
			cout << "写入文件成功!\n";
			time_t now_time=time(NULL);  
			//获取本地时间  
			tm*  t_tm = localtime(&now_time);  
			//转换为年月日星期时分秒结果，如图：  

			strncpy(f1->modificationTimes,asctime(t_tm),30);
			break;
		}
		f1 = f1->next;
	}
	if (flag == 0)
		printf("找不到此文件,无法写入文件!\n");
}

void closeFile(fcbNode& fcb)
{
	cout << "请输入你要关闭的文件名：\n";
	char input[30];
	cin >> input;
	fcbNode f1;
	f1 = fcb;
	int flag = 0;
	while (f1->next != NULL)
	{
		if (strcmp(f1->next->fileName, input) == 0)
		{
			if (f1->next->isOpen==0)
			{
				cout << "文件未打开，不用关闭!" << endl;
				return;
			}
			f1->next->isOpen = 0;
			cout <<"文件"<<input<<"已关闭！"<< endl;
			flag = 1;
			break;
		}
		f1 = f1->next;
	}
	if (flag == 0)
		printf("找不到此文件!\n");
}

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