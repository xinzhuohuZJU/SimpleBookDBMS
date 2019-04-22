#pragma once
#define MAX_BUFF 1024

#include<cstdio>
#include<iostream>
#include<mysql.h>
#include<atltime.h>
#include<Windows.h>
#include"ProofManag.h"

using namespace std;

void Borrow_Book(MYSQL* mysql);		//����ģ��
void BorrowRecord_Query(MYSQL* mysql); //�����¼��ѯ
void BorrowBook_Action(MYSQL* mysql);   //���鶯��ִ��ģ��
void Return_Book(MYSQL* mysql);		//����ģ��
void ReturnBook_Action(MYSQL* mysql);  //���鶯��ִ��ģ��

int BookStore_Query(MYSQL* mysql, char* bno); //��ѯ����Ƿ����/������
void Obtain_ReturnTime(MYSQL* mysql, char* rtime, char* bno); //��ö�Ӧ�������Ĺ黹ʱ��
void Obtain_CurrentTime(char* time);		//���ϵͳ��ǰʱ�� ��ʽΪYYYY-MM-DD HH:MM:SS
void UpdateBookStock(MYSQL* mysql, char* bno);  //���¿��

//����ģ��
void Borrow_Book(MYSQL* mysql)
{
	int user_choice;
	printf("***********����ģ��***********\n");
	printf("1.�����¼��ѯ\n");
	printf("2.����\n");

	scanf("%d", &user_choice);

	switch (user_choice)
	{
	case 1:
		BorrowRecord_Query(mysql);
		break;
	case 2:
		BorrowBook_Action(mysql);
		break;
	default:
		printf("�����Ŵ���\n");
		printf("��������ȷ�ķ��������\n");
		printf("\n");
		break;
	}

}

//�����¼��ѯģ��
void BorrowRecord_Query(MYSQL* mysql)
{
	char cno[40];
	char temp[MAX_BUFF];
	long ResultRows;	//�����������
	int ReturnValue;

	MYSQL_RES* res; //һ��������ṹ��
	MYSQL_ROW row;  //char** ��ά���飬���һ������¼

	printf("***********�����¼��ѯģ��***********\n");

	//�������֤����
	printf("�������뼴����ѯ�Ľ���֤���ţ�\n");
	cin >> cno;
	getchar();		//�����з�
	while (strlen(cno) != 7)
	{
		printf("����λ��������������ȷ�Ŀ���(7λ)��\n");
		cin >> cno;
		getchar(); //�����з�
	}

	//�Ȳ�ѯ�����Ƿ��ѵǼ�
	//����ProofMang�е�Query_Manag()����
	ReturnValue = Query_Manag(mysql, "card", cno);
	if (!ReturnValue)
	{
		cout << "************************************************" << endl;
		cout << "��ѯ�Ľ���֤���Ų����ڣ����Ƚ��еǼǣ�" << endl;
		cout << "************************************************" << endl;
		return;
	}

	//���Ŵ���
	sprintf(temp,
		"select bno,category,title,borrow_date,return_date from book natural join borrow where cno = '%s'",
		cno);
	ReturnValue = mysql_query(mysql, temp);
	if (ReturnValue)
	{
		printf("Error making query: %s !!!\n", mysql_error(mysql));
	}
	/*else
	{
		printf("%s executed!!!\n", temp);
	}*/

	//��ȡ�����
	res = mysql_store_result(mysql);
	ResultRows = (long)mysql_num_rows(res);	//Returns the number of rows in the result set.

	cout << "*******************��ѯ���*******************" << endl;
	if (!ResultRows)
	{
		cout << "Empty Set!" << endl;

	}
	else
	{
		cout << "���" << setw(25) << "���" << setw(30) << "����" << setw(25) << "��������" << setw(30) << "��������" << endl;
		//��ROW��ֵ���ж�ROW�Ƿ�Ϊ�գ���Ϊ�վʹ�ӡ���ݡ�
		while (row = mysql_fetch_row(res))
		{
			cout << row[0] << setw(25) << row[1] << setw(30) << row[2] << setw(30) << row[3] << setw(30) << row[4] << endl;
		}
	}

	//�ͷŽ����
	mysql_free_result(res);

	system("pause");

}

//���鶯��ִ��ģ��
void BorrowBook_Action(MYSQL* mysql)
{
	char cno[40];
	char bno[40];
	char rtime[40];
	char temp[MAX_BUFF];
	int ReturnValue;
	//int UpdStock;

	printf("***********���鶯��ִ��ģ��***********\n");
	//�������֤����
	printf("���������Ľ���֤���ţ�\n");
	cin >> cno;
	getchar();		//�����з�
	while (strlen(cno) != 7)
	{
		printf("����λ��������������ȷ�Ŀ���(7λ)��\n");
		cin >> cno;
		getchar(); //�����з�
	}

	//�Ȳ�ѯ�����Ƿ��ѵǼ�
	//����ProofMang�е�Query_Manag()����
	ReturnValue = Query_Manag(mysql, "card", cno);
	if (!ReturnValue)
	{
		cout << "************************************************" << endl;
		cout << "�ý���֤���Ų����ڣ����Ƚ��еǼǣ�" << endl;
		cout << "************************************************" << endl;
		return;
	}

	//�������
	printf("�������뼴�����ĵ���ţ�\n");
	cin >> bno;
	getchar();		//�����з�
	while (strlen(bno) != 10)
	{
		printf("���λ��������������ȷ�����(10λ)��\n");
		cin >> bno;
		getchar(); //�����з�
	}

	//��ѯ��ѯ����Ƿ����/������
	ReturnValue = BookStore_Query(mysql, bno);

	if (ReturnValue == -1)
	{
		cout << "*****************************" << endl;
		cout << "����Ų����ڣ���ȷ������������Ƿ������ݿ��У�" << endl;
		cout << "*****************************" << endl;

	}
	else if (ReturnValue == 0)
	{
		cout << "*****************************" << endl;
		printf("���Ϊ %s ��ͼ��Ŀǰ���޿�棡\n", bno);
		//��ø�������黹��ʱ��
		Obtain_ReturnTime(mysql, rtime, bno);
		//������黹δ���˹黹��
		if (!strcmp(rtime, "0000-00-00 00:00:00")) printf("���Ϊ %s ��ͼ��δ��ѯ���й黹��¼��\n", bno);
		else printf("���Ϊ %s ��ͼ������Ĺ黹ʱ��Ϊ: %s\n", bno, rtime);

	}
	else
	{

		//���п�棬���Գɹ�����
		Obtain_CurrentTime(rtime);
		//����ʱreturn date Ĭ������Ϊ 0000-00-00 00:00:00
		sprintf(temp,
			"INSERT INTO borrow VALUES('%s','%s','%s',0)",
			cno, bno, rtime);

		int rt = mysql_query(mysql, temp);
		if (rt)
		{
			printf("Error making query: %s !!!\n", mysql_error(mysql));
			cout << "*************************************" << endl;
			cout << "����borrow��ϵʱʧ�ܣ�" << endl;
			return;
		}
		//���¿��
		//����UpdateBookStock����
		UpdateBookStock(mysql, bno);
		cout << "*************************************" << endl;
		printf("����֤����Ϊ %s ���û�����ɹ�!\n", cno);
	}


	system("pause");

}

//��ѯ����Ƿ����/������
int BookStore_Query(MYSQL* mysql, char* bno)
{
	int ReturnCode;
	char temp[MAX_BUFF];

	MYSQL_RES* res; //һ��������ṹ��
	MYSQL_ROW row;  //char** ��ά���飬���һ������¼
	long ResultRows; //MySQL��ѯ������

	sprintf(temp, "select stock from book where bno = '%s'", bno);
	ReturnCode = mysql_query(mysql, temp);
	if (ReturnCode)
	{
		printf("Error making query: %s !!!\n", mysql_error(mysql));
	}
	/*else
	{
		printf("%s executed!!!\n", temp);
	}*/

	//��ȡ�����
	res = mysql_store_result(mysql);
	ResultRows = (long)mysql_num_rows(res);	//Returns the number of rows in the result set.

	if (!ResultRows) //���Ϊ��
	{
		ReturnCode = -1;

	}
	else
	{
		//��ȡstock
		row = mysql_fetch_row(res);
		int stock = atoi(row[0]);
		ReturnCode = stock;
	}

	//�ͷŽ����
	mysql_free_result(res);
	return ReturnCode;
}

//��ö�Ӧ�������Ĺ黹ʱ��
void Obtain_ReturnTime(MYSQL* mysql, char* rtime, char* bno)
{
	char temp[MAX_BUFF];

	MYSQL_RES* res; //һ��������ṹ��
	MYSQL_ROW row;  //char** ��ά���飬���һ������¼
	int rt;         //return value

	//�������е�ǰ��ŵ�return_date
	sprintf(temp,
		"select return_date from borrow where bno = '%s' order by return_date desc",
		bno);

	rt = mysql_query(mysql, temp);
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(mysql));
	}
	/*else
	{
		printf("%s executed!!!\n", temp);
	}*/

	//��ȡ�����
	res = mysql_store_result(mysql);
	row = mysql_fetch_row(res);

	//����ȡ���Ľ������rtime
	strcpy(rtime, row[0]);

	//�ͷŽ����
	mysql_free_result(res);
	return;
}

//���ϵͳ��ǰ��ʱ��
void Obtain_CurrentTime(char* time)
{
	char temp[30];
	CTime curTime = CTime::GetCurrentTime();
	int nYear = curTime.GetYear();  // ��ȡ��ǰ���   
	int nMonth = curTime.GetMonth(); // ��ȡ��ǰ�·�   
	int nDay = curTime.GetDay();   // ��ȡ��ǰ����   
	int nHour = curTime.GetHour();  // ��ȡ��ǰСʱʱ��   
	int nMin = curTime.GetMinute(); // ��ȡ��ǰ����ʱ��   
	int nSec = curTime.GetSecond(); // ��ȡ��ǰ��ʱ��   

	sprintf(temp, "%d-%02d-%02d %02d:%02d:%02d", nYear, nMonth, nDay, nHour, nMin, nSec);
	//��ֵ��time
	strcpy(time, temp);
}
//����ɹ�ʱ���¶�Ӧ��ſ�棬��Ӧ��ſ��-1
void UpdateBookStock(MYSQL* mysql, char* bno)
{
	char temp[MAX_BUFF];
	int rt;		//return value

	sprintf(temp,
		"UPDATE book SET stock = stock - 1 where bno = '%s'",
		bno);

	rt = mysql_query(mysql, temp);
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(mysql));
	}
	else
	{
		printf("%s executed!!!\n", temp);
		cout << "*************************************" << endl;
		printf("���Ϊ %s ��ͼ��Ŀ���Ѿ�����!\n", bno);
	}

}
//����ģ��
void Return_Book(MYSQL* mysql)
{
	int user_choice;
	printf("***********����ģ��***********\n");
	printf("1.�����¼��ѯ\n");
	printf("2.����\n");

	scanf("%d", &user_choice);

	switch (user_choice)
	{
	case 1:
		BorrowRecord_Query(mysql);
		break;
	case 2:
		ReturnBook_Action(mysql);
		break;
	default:
		printf("�����Ŵ���\n");
		printf("��������ȷ�ķ��������\n");
		printf("\n");
		break;
	}

}
//���鶯��ִ��ģ��
void ReturnBook_Action(MYSQL* mysql)
{

	char cno[40];
	char bno[40];
	char temptime[40];
	char rtime[40];
	char temp[MAX_BUFF];
	int ReturnValue;
	int UpdStock;

	MYSQL_RES* res; //һ��������ṹ��
	MYSQL_ROW row;  //char** ��ά���飬���һ������¼
	long ResultRows;

	printf("***********���鶯��ִ��ģ��***********\n");
	//�������֤����
	printf("���������Ľ���֤���ţ�\n");
	cin >> cno;
	getchar();		//�����з�
	while (strlen(cno) != 7)
	{
		printf("����λ��������������ȷ�Ŀ���(7λ)��\n");
		cin >> cno;
		getchar(); //�����з�
	}

	//�Ȳ�ѯ�����Ƿ��ѵǼ�
	//����ProofMang�е�Query_Manag()����
	ReturnValue = Query_Manag(mysql, "card", cno);
	if (!ReturnValue)
	{
		cout << "************************************************" << endl;
		cout << "�ý���֤���Ų����ڣ����Ƚ��еǼǣ�" << endl;
		cout << "************************************************" << endl;
		return;
	}

	//�������
	printf("�������뼴���黹����ţ�\n");
	cin >> bno;
	getchar();		//�����з�
	while (strlen(bno) != 10)
	{
		printf("���λ��������������ȷ�����(10λ)��\n");
		cin >> bno;
		getchar(); //�����з�
	}

	//���ѽ��鼮�б��в�ѯ�����ϵ�Ƿ����
	sprintf(temp,
		"select * from borrow where cno= '%s' and bno = '%s' and return_date = 0 order by borrow_date",
		cno, bno);
	ReturnValue = mysql_query(mysql, temp);
	if (ReturnValue)
	{
		printf("Error making query: %s !!!\n", mysql_error(mysql));
	}
	/*else
	{
		printf("%s executed!!!\n", temp);
	}*/

	//��ȡ�����
	res = mysql_store_result(mysql);
	ResultRows = (long)mysql_num_rows(res);	//Returns the number of rows in the result set.

	if (!ResultRows)
	{
		cout << "***************************************************" << endl;
		printf("����Ϊ %s �� ���Ϊ %s �Ľ����ϵĿǰ�����ڣ���ȷ�ϣ�\n", cno, bno);
	}
	else
	{
		//��ͬһ���û�һ�ν�౾ͬһ��ŵ���ʱ���黹��������һ����
		row = mysql_fetch_row(res);	//�ѽ������У����Դ����һ��
		strcpy(temptime, row[2]);

		//�ȸ��¸ý����¼�Ļ���ʱ��
		Obtain_CurrentTime(rtime);
		sprintf(temp,
			"UPDATE borrow SET return_date = '%s' WHERE cno = '%s' and bno = '%s' and borrow_date = '%s'",
			rtime, cno, bno, temptime);
		ReturnValue = mysql_query(mysql, temp);
		if (ReturnValue)
		{
			printf("Error making query: %s !!!\n", mysql_error(mysql));
		}
		else
		{
			printf("%s executed!!!\n", temp);
			cout << "**********************************" << endl;
			printf("����Ϊ %s �� ���Ϊ %s �Ļ��������Ѿ����£���ȷ�ϣ�\n", cno, bno);
		}

		//�ٸ��¸�����
		sprintf(temp,
			"UPDATE book SET stock = stock + 1 WHERE bno = '%s'", bno);
		ReturnValue = mysql_query(mysql, temp);
		if (ReturnValue)
		{
			printf("Error making query: %s !!!\n", mysql_error(mysql));
		}
		else
		{
			printf("%s executed!!!\n", temp);
			cout << "**********************************" << endl;
			printf("���Ϊ %s �Ŀ���Ѿ����£���ȷ�ϣ�\n", bno);
		}

		cout << "**********************************" << endl;
		cout << "����ɹ�!" << endl;
	}

	//�ͷŽ����
	mysql_free_result(res);
	system("pause");

}