#include "fileOperation.h"

int main()
{
	int choice;
	cout << "********************************************************\n";
	cout << "                 欢迎使用本文件系统！                   \n";
	cout << " 1.展示所有文件                        2.创建文件       \n";
	cout << " 3.删除文件                            4.文件重命名     \n";
	cout << " 5.读文件                              6.写文件         \n";
	cout << " 7.查看文件属性                        8.关闭文件       \n";
	cout << " 9.查看磁盘                            0.exit           \n";
	cout << "********************************************************\n";
	fcbNode fcb;
	initial(fcb);
	while (true) 
	{
		cout << "请输入你想操作号码：";
		cin >> choice;
		switch (choice)
		{
		case 1:
			showFiles(fcb);
			break;
		case 2:
			fileCreate(fcb);
			break;
		case 3:
			fileDelete(fcb);
			break;
		case 4:
			renameFile(fcb);
			break;
		case 5:
			readFile(fcb);
			break;
		case 6:
			writeFile(fcb);
			break;
		case 7:
			fileProperties(fcb);
			break;
		case 8:
			closeFile(fcb);
			break;
		case 9:
			showDisk(fcb);
			break;
		case 0:
			exit(0);
		default:
			cout << "您输入的操作有误，请重新输入！\n";
			break;
		}
	}
	return 0;
}