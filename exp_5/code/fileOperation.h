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
static bool ldisk[64]; //空闲文件描述符
static int endPosition;

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



