#include "fileOperation.h"

int main()
{
	int choice;
	cout << "********************************************************\n";
	cout << "                 ��ӭʹ�ñ��ļ�ϵͳ��                   \n";
	cout << " 1.չʾ�����ļ�                        2.�����ļ�       \n";
	cout << " 3.ɾ���ļ�                            4.�ļ�������     \n";
	cout << " 5.���ļ�                              6.д�ļ�         \n";
	cout << " 7.�鿴�ļ�����                        8.�ر��ļ�       \n";
	cout << " 9.�鿴����                            0.exit           \n";
	cout << "********************************************************\n";
	fcbNode fcb;
	initial(fcb);
	while (true) 
	{
		cout << "����������������룺";
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
			cout << "������Ĳ����������������룡\n";
			break;
		}
	}
	return 0;
}