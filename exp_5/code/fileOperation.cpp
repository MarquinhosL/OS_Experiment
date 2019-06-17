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
	cout << "�����봴���ļ������ơ���С��Ȩ��(r��w��rw),�Կո�ֿ�!\n";
	char fname[30],fkind[5];
	int fsize;
	cin >> fname >> fsize >> fkind;
	while (f1)
	{
		if (strcmp(f1->fileName,fname)==0)
		{
			cout << "�ļ����ظ��������ļ�ʧ��!\n";
			return ;
		}
		if (!(f1->next))
			break;
		else
			f1 = f1->next;
	}
	if (endPosition + fsize > MaxDisk)
	{
		cout << "����ʣ��ռ䲻�㣬�����ļ�ʧ��!\n";
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
	//��ȡ����ʱ��  
	tm*  t_tm = localtime(&now_time);  
	//ת��Ϊ����������ʱ����������ͼ��  

	strncpy(f2->modificationTimes,asctime(t_tm),30);
	f2->isOpen = 0;
	f2->status = 1;
	f1->next = f2;
	f1->next->next = NULL;
	f1 = f1->next->next;
	cout << "�����ļ��ɹ�!\n";
}

void fileDelete(fcbNode &fcb)
{
	cout << "��������Ҫɾ�����ļ�����\n";
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
				cout << "�ļ����ڴ�״̬���޷�ɾ��!\n";
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
			cout << "ɾ���ļ��ɹ�!\n";
			break;
		}
		f1 = f1->next;
	}
	if (flag == 0)
		printf("�Ҳ������ļ�,�޷�ɾ��!\n");
}

void showFiles(fcbNode &fcb)
{
	fcbNode f1;
	f1 = fcb->next;
	while (f1)
	{
		cout << "�ļ�����" << f1->fileName << "\t\t�ļ���С��" << f1->fileSize << " B\t\t�޸�ʱ�䣺" << f1->modificationTimes << endl;
		if (f1->next != NULL)
			f1 = f1->next;
		else
			break;
	}
}

void renameFile(fcbNode& fcb)
{
	cout << "��������Ҫ���������ļ�����\n";
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
				cout << "�ļ����ڴ�״̬���޷�������!\n";
				return;
			}
			cout << "��������������\n";
			cin >> input;
			while (f2)
			{
				if (strcmp(f2->fileName, input ) == 0)
				{
					cout << "�ļ����ظ����������ļ�ʧ��!\n";
					return;
				}
				if (!(f2->next))
					break;
				else
					f2 = f2->next;
			}
			strcpy(f1->next->fileName, input);
			flag = 1;
			cout << "�������ļ��ɹ�!\n";
			break;
		}
		f1 = f1->next;
	}
	if (flag == 0)
		printf("�Ҳ������ļ�,�޷�������!\n");
}

void fileProperties(fcbNode& fcb)
{
	cout << "��������Ҫ�鿴���Ե��ļ�����\n";
	char input[30];
	cin >> input;
	fcbNode f1;
	f1 = fcb;
	int flag = 0;
	while (f1->next != NULL)
	{
		if (strcmp(f1->next->fileName, input) == 0)
		{
			cout << "�ļ�����" << "\t�ļ���������" << "\t�ļ���С��" << "\t�ļ����ͣ�" << "\t�ļ�����λ�ã�" << "\t�޸�ʱ�䣺" << "\t\t�Ƿ�򿪣�" << endl;
			cout << f1->next->fileName << "\t\t" << f1->next->fileNumber << "\t\t" << f1->next->fileSize << "\t\t" << f1->next->fileKind << "\t\t" << f1->next->diskPosition << "\t\t" << f1->next->modificationTimes << "\t" << f1->next->isOpen << endl;
			flag = 1;
			break;
		}
		f1 = f1->next;
	}
	if (flag == 0)
		printf("�Ҳ������ļ�,�޷��鿴����!\n");
}

void readFile(fcbNode& fcb)
{
	cout << "��������Ҫ��ȡ���ݵ��ļ�����\n";
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
				cout << "�ļ����ɶ�!"<<endl;
				return;
			}
			cout << "�ļ��������£�\n";
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
		printf("�Ҳ������ļ�,�޷���ȡ�ļ�!\n");
}

void writeFile(fcbNode& fcb)
{
	cout << "��������Ҫд�����ݵ��ļ�����\n";
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
				cout << "�ļ�����д!" << endl;
				return;
			}
			if (f1->next->isOpen == 1) 
			{
				cout << "�ļ��Ѵ򿪣�����д��!" << endl;
				return;
			}
			cout << "�������ļ����ݣ�" << endl;
			string str;
			cin >> str;
			if (int(str.length()) > f1->next->fileSize)
			{
				cout << "д�����ݳ����ļ���С��д��ʧ��!\n";
				return;
			}
			for (i = 0; i < int(str.length()); i++)
			{
				disk[i + f1->next->writePosition] = str[i];
			}
			f1->next->writePosition += str.length();
			f1->next->isOpen = 1;
			flag = 1;
			cout << "д���ļ��ɹ�!\n";
			time_t now_time=time(NULL);  
			//��ȡ����ʱ��  
			tm*  t_tm = localtime(&now_time);  
			//ת��Ϊ����������ʱ����������ͼ��  

			strncpy(f1->modificationTimes,asctime(t_tm),30);
			break;
		}
		f1 = f1->next;
	}
	if (flag == 0)
		printf("�Ҳ������ļ�,�޷�д���ļ�!\n");
}

void closeFile(fcbNode& fcb)
{
	cout << "��������Ҫ�رյ��ļ�����\n";
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
				cout << "�ļ�δ�򿪣����ùر�!" << endl;
				return;
			}
			f1->next->isOpen = 0;
			cout <<"�ļ�"<<input<<"�ѹرգ�"<< endl;
			flag = 1;
			break;
		}
		f1 = f1->next;
	}
	if (flag == 0)
		printf("�Ҳ������ļ�!\n");
}

void showDisk(fcbNode& fcb)
{
	fcbNode f1;
	f1 = fcb->next;
	i = 1;
	cout << "�����ܴ�СΪ" << MaxDisk << "bytes,��ʹ��" << endPosition << "bytes,��ʣ��" << MaxDisk - endPosition << "bytess!" << endl;
	cout << "�̿��" << "\t��ʼ��ַ" << "\t������С" << "\t�Ƿ�ʹ��\n";
	cout << "0" << "\t" << endPosition << "\t\t" << MaxDisk - endPosition << "bytes\t" << "��"<<endl;
	while (f1)
	{
		cout << i << "\t" << f1->diskPosition << "\t\t" << f1->fileSize << "bytes\t��" << endl;
		i++;
		if (f1->next != NULL)
			f1 = f1->next;
		else
			break;
	}
	
}