#pragma once
#define MAX_BUFF 1024

#include<cstdio>
#include<iostream>
#include<iomanip>
#include<string>
#include<mysql.h>

using namespace std;

void Check_Book(MYSQL* mysql);            //ͼ���ѯ
void Check_Book_ByCategory(MYSQL* mysql); //ͼ������ѯ
void Check_Book_ByTitle(MYSQL* mysql);	 //ͼ��������ѯ
void Check_Book_ByPress(MYSQL* mysql);	//ͼ������̲�ѯ
void Check_Book_ByYear(MYSQL* mysql);	//ͼ����������ѯ
void Check_Book_ByAuthor(MYSQL* mysql); //ͼ�����߲�ѯ
void Check_Book_ByPrice(MYSQL* mysql);  //ͼ��۸������ѯ
void Check_Book_Print(MYSQL* mysql, MYSQL_RES* res, const char* tablename); //���ڴ�ӡ��ѯ����ĺ���

//ͼ���ѯģ��
void Check_Book(MYSQL* mysql)
{
	int user_choice;	//�����û�ѡ��
	printf("******ͼ���ѯģ��******\n");
	printf("1.ͼ������ѯ\n");
	printf("2.ͼ��������ѯ\n");
	printf("3.ͼ��������ѯ\n");
	printf("4.ͼ����ݲ�ѯ\n");
	printf("5.ͼ�����߲�ѯ\n");
	printf("6.ͼ��۸��ѯ\n");
	printf("��������Ҫ�ķ�����:\n");

	scanf("%d", &user_choice);

	switch (user_choice)
	{
	case 1:
		//ͼ������ѯ
		Check_Book_ByCategory(mysql);
		break;
	case 2:
		//ͼ��������ѯ
		Check_Book_ByTitle(mysql);
		break;
	case 3:
		//ͼ������̲�ѯ
		Check_Book_ByPress(mysql);
		break;
	case 4:
		//ͼ����������ѯ
		Check_Book_ByYear(mysql);
		break;
	case 5:
		//ͼ�����߲�ѯ
		Check_Book_ByAuthor(mysql);
		break;
	case 6:
		//ͼ��۸��ѯ
		Check_Book_ByPrice(mysql);
		break;
	default:
		printf("�����Ŵ���\n");
		printf("��������ȷ�ķ��������\n");
		printf("\n");
		break;
	}
}
//ͼ���ѯ���ģ��
void Check_Book_ByCategory(MYSQL* mysql)
{
	const char* tablename = "book";
	MYSQL_RES* res; //һ��������ṹ��
	char temp[MAX_BUFF];
	char userinput[MAX_BUFF];
	int rt;

	printf("******ͼ������ѯģ��******\n");
	printf("��������Ҫ��ѯ��ͼ�����:\n");
	getchar();
	cin.getline(userinput, MAX_BUFF);

	sprintf(temp, "select * from %s where category = '%s'", tablename, userinput);
	rt = mysql_query(mysql, temp);
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(mysql));
	}
	else
	{
		printf("%s executed!!!\n", temp);
	}

	//��ȡ�����
	res = mysql_store_result(mysql);
	//���ô�ӡ�������
	Check_Book_Print(mysql, res, tablename);

	//�ͷŽ����
	mysql_free_result(res);

	system("pause");
}
//ͼ���ѯ����ģ��
void Check_Book_ByTitle(MYSQL* mysql)
{

	const char* tablename = "book";
	MYSQL_RES* res; //һ��������ṹ��
	char temp[MAX_BUFF];
	char userinput[MAX_BUFF];
	int rt;

	printf("******ͼ��������ѯģ��******\n");
	printf("��������Ҫ��ѯ��ͼ�����:\n");
	getchar();
	cin.getline(userinput, MAX_BUFF);

	sprintf(temp, "select * from %s where title = '%s'", tablename, userinput);
	rt = mysql_query(mysql, temp);
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(mysql));
	}
	else
	{
		printf("%s executed!!!\n", temp);
	}

	//��ȡ�����
	res = mysql_store_result(mysql);
	//���ô�ӡ�������
	Check_Book_Print(mysql, res, tablename);

	//�ͷŽ����
	mysql_free_result(res);

	system("pause");

}

//ͼ���ѯ������ģ��
void Check_Book_ByPress(MYSQL* mysql)
{
	const char* tablename = "book";
	MYSQL_RES* res; //һ��������ṹ��
	char temp[MAX_BUFF];
	char userinput[MAX_BUFF];
	int rt;

	printf("******ͼ������̲�ѯģ��******\n");
	printf("��������Ҫ��ѯ��ͼ��ĳ�����:\n");
	getchar();
	cin.getline(userinput, MAX_BUFF);

	sprintf(temp, "select * from %s where press = '%s'", tablename, userinput);
	rt = mysql_query(mysql, temp);
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(mysql));
	}
	else
	{
		printf("%s executed!!!\n", temp);
	}

	//��ȡ�����
	res = mysql_store_result(mysql);
	//���ô�ӡ�������
	Check_Book_Print(mysql, res, tablename);

	//�ͷŽ����
	mysql_free_result(res);

	system("pause");

}

//ͼ���ѯ�������ģ��
void Check_Book_ByYear(MYSQL* mysql)
{
	const char* tablename = "book";
	MYSQL_RES* res; //һ��������ṹ��
	char temp[MAX_BUFF];
	int rt;

	int LowerBound;  //�������
	int UpperBound;	 //�������
	int tempBound;

	printf("******ͼ����������ѯģ��******\n");
	cout << "��������Ҫ��ѯ���������(0Ϊ������):" << endl;
	cin >> LowerBound;
	cout << "��������Ҫ��ѯ���������(0Ϊ������):" << endl;
	cin >> UpperBound;

	//����������и�������ôת��Ϊ0
	if (LowerBound < 0) LowerBound = 0;
	if (UpperBound < 0) UpperBound = 0;

	//������޴������޲������޲�����0����ô�û�����
	if ((LowerBound > UpperBound) && UpperBound != 0)
	{
		tempBound = LowerBound;
		LowerBound = UpperBound;
		UpperBound = tempBound;
	}

	if (LowerBound != 0 && UpperBound != 0)
	{
		//���߶���Ϊ0
		sprintf(temp, "select * from %s where year>=%d and year<= %d", tablename, LowerBound, UpperBound);
		rt = mysql_query(mysql, temp);
		if (rt)
		{
			printf("Error making query: %s !!!\n", mysql_error(mysql));
		}
		else
		{
			printf("%s executed!!!\n", temp);
		}
	}
	else if (LowerBound == 0 && UpperBound != 0)
	{
		//������
		sprintf(temp, "select * from %s where year<=%d", tablename, UpperBound);
		rt = mysql_query(mysql, temp);
		if (rt)
		{
			printf("Error making query: %s !!!\n", mysql_error(mysql));
		}
		else
		{
			printf("%s executed!!!\n", temp);
		}
	}
	else if (LowerBound != 0 && UpperBound == 0)
	{
		//������
		sprintf(temp, "select * from %s where year>=%d", tablename, LowerBound);
		rt = mysql_query(mysql, temp);
		if (rt)
		{
			printf("Error making query: %s !!!\n", mysql_error(mysql));
		}
		else
		{
			printf("%s executed!!!\n", temp);
		}
	}
	else
	{
		//���߾�Ϊ0
		sprintf(temp, "select * from %s ", tablename);
		rt = mysql_query(mysql, temp);
		if (rt)
		{
			printf("Error making query: %s !!!\n", mysql_error(mysql));
		}
		else
		{
			printf("%s executed!!!\n", temp);
		}
	}

	//��ȡ�����
	res = mysql_store_result(mysql);
	//���ô�ӡ�������
	Check_Book_Print(mysql, res, tablename);

	//�ͷŽ����
	mysql_free_result(res);
	system("pause");
}

//ͼ��۸������ѯ
void Check_Book_ByPrice(MYSQL* mysql)
{
	const char* tablename = "book";
	MYSQL_RES* res; //һ��������ṹ��
	char temp[MAX_BUFF];
	int rt;

	double LowerBound;  //�۸�����
	double UpperBound;	 //�۸�����
	double tempBound;

	printf("******ͼ��۸������ѯģ��******\n");
	cout << "��������Ҫ��ѯ�ļ۸�����(0Ϊ������):" << endl;
	cin >> LowerBound;
	cout << "��������Ҫ��ѯ�ļ۸�����(0Ϊ������):" << endl;
	cin >> UpperBound;

	//����������и�������ôת��Ϊ0
	if (LowerBound < 0) LowerBound = 0;
	if (UpperBound < 0) UpperBound = 0;

	//������޴������޲������޲�����0����ô�û�����
	if ((LowerBound > UpperBound) && UpperBound != 0)
	{
		tempBound = LowerBound;
		LowerBound = UpperBound;
		UpperBound = tempBound;
	}

	if (LowerBound != 0 && UpperBound != 0)
	{
		//���߶���Ϊ0
		sprintf(temp, "select * from %s where price>=%lf and price<= %lf", tablename, LowerBound, UpperBound);
		rt = mysql_query(mysql, temp);
		if (rt)
		{
			printf("Error making query: %s !!!\n", mysql_error(mysql));
		}
		else
		{
			printf("%s executed!!!\n", temp);
		}
	}
	else if (LowerBound == 0 && UpperBound != 0)
	{
		//������
		sprintf(temp, "select * from %s where price<=%lf", tablename, UpperBound);
		rt = mysql_query(mysql, temp);
		if (rt)
		{
			printf("Error making query: %s !!!\n", mysql_error(mysql));
		}
		else
		{
			printf("%s executed!!!\n", temp);
		}
	}
	else if (LowerBound != 0 && UpperBound == 0)
	{
		//������
		sprintf(temp, "select * from %s where price>=%lf", tablename, LowerBound);
		rt = mysql_query(mysql, temp);
		if (rt)
		{
			printf("Error making query: %s !!!\n", mysql_error(mysql));
		}
		else
		{
			printf("%s executed!!!\n", temp);
		}
	}
	else
	{
		//���߾�Ϊ0
		sprintf(temp, "select * from %s ", tablename);
		rt = mysql_query(mysql, temp);
		if (rt)
		{
			printf("Error making query: %s !!!\n", mysql_error(mysql));
		}
		else
		{
			printf("%s executed!!!\n", temp);
		}
	}

	//��ȡ�����
	res = mysql_store_result(mysql);
	//���ô�ӡ�������
	Check_Book_Print(mysql, res, tablename);

	//�ͷŽ����
	mysql_free_result(res);
	system("pause");

}

//ͼ�����߲�ѯģ��
void Check_Book_ByAuthor(MYSQL* mysql)
{
	const char* tablename = "book";
	MYSQL_RES* res; //һ��������ṹ��
	char temp[MAX_BUFF];
	char userinput[MAX_BUFF];
	int rt;

	printf("******ͼ�����߲�ѯģ��******\n");
	printf("��������Ҫ��ѯ��ͼ�������:\n");
	getchar();
	cin.getline(userinput, MAX_BUFF);

	sprintf(temp, "select * from %s where author = '%s'", tablename, userinput);
	rt = mysql_query(mysql, temp);
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(mysql));
	}
	else
	{
		printf("%s executed!!!\n", temp);
	}

	//��ȡ�����
	res = mysql_store_result(mysql);
	//���ô�ӡ�������
	Check_Book_Print(mysql, res, tablename);

	//�ͷŽ����
	mysql_free_result(res);

	system("pause");
}

//��ӡ��ѯ���ģ��(�Ҷ���)
void Check_Book_Print(MYSQL* mysql, MYSQL_RES* res, const char* tablename)
{
	MYSQL_ROW row;  //char** ��ά���飬���һ������¼
	long ResultRows;


	cout << "*******************��ѯ���*******************" << endl;
	ResultRows = (long)mysql_num_rows(res);	//Returns the number of rows in the result set.
	if (ResultRows == 0)
	{
		cout << "Empty Set!" << endl;
	}
	else
	{
		//��ʾ����
		//cout << setiosflags(ios::left);
		cout << "���" << setw(25) << "���" << setw(30) << "����" <<
			setw(20) << "������" << setw(15) << "���" << setw(15) << "����" << setw(15) << "�۸�" <<
			setw(15) << "����" << setw(15) << "���" << endl;
		//��ROW��ֵ���ж�ROW�Ƿ�Ϊ�գ���Ϊ�վʹ�ӡ����
		//���д�ӡ����
		while (row = mysql_fetch_row(res))
		{

			//cout << setiosflags(ios::left);
			cout << row[0] << setw(20) << row[1] << setw(30) << row[2] <<
				setw(20) << row[3] << setw(15) << row[4] << setw(15) << row[5] << setw(15) << row[6] <<
				setw(15) << row[7] << setw(15) << row[8] << endl;

		}

	}

}