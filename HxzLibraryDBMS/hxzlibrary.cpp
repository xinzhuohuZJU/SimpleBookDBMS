#define _CRT_SECURE_NO_WARNINGS
#define MAX_BUFF 1024

#include<cstdio>
#include<iostream>
#include<mysql.h>
#include"AddBook.h"
#include"CheckBook.h"
#include"ProofManag.h"
#include"BorrowReturn.h"

using namespace std;

int main()
{
	MYSQL mysql;    //һ�����ݿ�ṹ��
	MYSQL_RES* res; //һ��������ṹ��
	MYSQL_ROW row;  //char** ��ά���飬���һ������¼

	//���ݿ�������Ϣ
	const char* HOSTNAME = "localhost";
	const char* USER = "root";
	const char* PASSWORD = "970119";
	const char* DATABASENAME = "hxzlibrary";
	int MyPort = 3306;

	int user_choice;	//����û�ѡ��

	printf("*******************************************************************\n");
	printf("\t\t\tͼ�����ϵͳ\n");
	printf("*******************************************************************\n");

	//��ʼ�����ݿ�
	mysql_init(&mysql);
	//���ñ��뷽ʽ
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
	//�������ݿ�
	//�ж��������ʧ�ܾ��������ʧ�ܡ�
	if (mysql_real_connect(&mysql, HOSTNAME, USER, PASSWORD, DATABASENAME, MyPort, NULL, 0) == NULL)
		printf("����ʧ�ܣ�\n");

	while (1)
	{
		printf("1.ͼ�����\n");
		printf("2.ͼ���ѯ\n");
		printf("3.����\n");
		printf("4.����\n");
		printf("5.����֤����\n");
		printf("0.�˳���ϵͳ\n");
		printf("��������Ҫ�ķ�����:\n");

		scanf("%d", &user_choice);

		switch (user_choice)
		{
		case 0:
			//�Ͽ����ӣ��˳�������
			//�ر����ݿ�
			mysql_close(&mysql);
			printf("***************�������˳���ϵͳ��***************\n");
			system("pause");
			exit(0);

		case 1:
			//ͼ�����
			ADD_Book(&mysql);
			break;

		case 2:
			//ͼ���ѯ
			Check_Book(&mysql);
			break;
		case 3:
			//����
			Borrow_Book(&mysql);
			break;
		case 4:
			//����
			Return_Book(&mysql);
			break;
		case 5:
			//����֤����
			Proof_Manag(&mysql);
			break;

		default:
			printf("�����Ŵ���\n");
			printf("��������ȷ�ķ�������ţ�\n");
			printf("\n");
			break;
		}
	}
		
	return 0;
}

