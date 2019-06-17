#pragma once
#include <stdio.h>
#include <time.h>
#include <iostream>
//#include <xutility>
#include <string>
#include <sstream>
#include <cstring>
//#include <atltime.h>

using namespace std;

#define MaxDisk 512*1024
static char disk[MaxDisk];
static bool ldisk[64]; //�����ļ�������
static int endPosition;

typedef struct FCB
{
	struct FCB *next;
	char fileName[30]; //�ļ���
	int fileSize;  //�ļ�����
	char fileKind[5];  //�ļ�����
	int fileNumber; //�ļ����������
	int diskPosition; //����λ��
	int writePosition;//дָ��λ��
	char modificationTimes[30];  //�޸�ʱ��
	int isOpen; //�Ƿ��,1��ʾ��״̬��0��ʾδ��״̬
	int status; //״̬λ
}*fcbNode;

void initial(fcbNode &fcb);
void fileCreate(fcbNode &fcb);
void fileDelete(fcbNode &fcb);
void showFiles(fcbNode &fcb);
void readFile(fcbNode &fcb);
void writeFile(fcbNode &fcb);
void renameFile(fcbNode &fcb);
void fileProperties(fcbNode &fcb);
void closeFile(fcbNode& fcb);
void showDisk(fcbNode& fcb);



