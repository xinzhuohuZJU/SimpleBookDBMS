#pragma once
#define MAX_BUFF 1024

#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<iomanip>
#include<string>
#include<vector>
#include<fstream>  
#include<sstream>  
#include<mysql.h>

using namespace std;

void ADD_Book(MYSQL* mysql);	    //ͼ�����
void ADD_Book_Single(MYSQL* mysql); //ͼ�鵥�����
void ADD_Book_Batch(MYSQL* mysql);	//ͼ���������
int  Book_Insertion(MYSQL* mysql, const char* tablename, char* bno,
	char* category, char* title, char* press, int year, char* author, double price); //ͼ�����

//ͼ�����ģ��
void ADD_Book(MYSQL* mysql)
{
	int user_choice;	//�����û�ѡ��
	printf("******ͼ�����ģ��******\n");
	printf("1.�������\n");
	printf("2.�������\n");
	printf("��������Ҫ�ķ�����:\n");

	scanf("%d", &user_choice);

	switch (user_choice)
	{
	case 1:
		//�������ģ��
		ADD_Book_Single(mysql);
		break;
	case 2:
		//�������ģ��
		ADD_Book_Batch(mysql);
		break;

	default:
		printf("�����Ŵ���\n");
		printf("��������ȷ�ķ�������ţ�\n");
		printf("\n");
		break;
	}

}
//ͼ�鵥�����ģ��
void ADD_Book_Single(MYSQL* mysql)
{
	char bno[15];
	char category[20];
	char title[30];
	char press[30];
	int year;
	char author[20];
	double price;

	const char* tablename = "book";
	int rt;			//return value


	printf("******ͼ�鵥�����ģ��******\n");
	printf("�����������:\n");
	cin >> bno;	//�������
	getchar();  //�����з�
	while (strlen(bno) != 10)
	{
		printf("���λ��������������ȷ�����(10λ)��\n");
		cin >> bno;
		getchar(); //�����з�
	}
	printf("��������������:\n");
	//����п����пո�,ʹ��cin.getline
	cin.getline(category, 20);
	printf("����������ı���:\n");
	cin.getline(title, 30);
	printf("����������ĳ�����:\n");
	cin.getline(press, 30);
	printf("��������������ķ������ ���� �۸�:\n");
	cin >> year >> author >> price;

	//����ͼ������ӳ���
	rt = Book_Insertion(mysql, tablename, bno, category, title, press, year, author, price);
	if (rt == 1)
	{
		printf("***********ͼ�����ʧ�ܣ������ԣ�***********\n");
	}

	system("pause");

}

//ͼ���������ģ�飬��ȡcsv�ļ�
void ADD_Book_Batch(MYSQL* mysql)
{
	//����һ���ṹ��book���ڴ���鱾��Ϣ
	struct Book
	{
		char bno[15];
		char category[20];
		char title[30];
		char press[30];
		int year;
		char author[20];
		double price;
	};

	string csvpath; //���ڴ��csv�ļ�·��

	const char* tablename = "book";
	int rt;    //return value
	int bookcount;	//csv�ļ��е�ͼ����Ŀ
	//�ṹ��ָ��
	Book* mybook = new Book;


	printf("******ͼ���������ģ��******\n");
	printf("��������Ҫ��ȡ��csv�ļ���(ȷ����cpp�ļ���ͬһĿ¼��)\n");
	cin >> csvpath;

	ifstream inFile(csvpath, ios::in);
	string line;
	if (!inFile)
	{
		printf("��ǰĿ¼��û�и��ļ�,�����ԣ�\n");
		return;
	}
	else
	{
		printf("******�ļ��򿪳ɹ�******\n");
		bookcount = 1;
		while (getline(inFile, line))
		{
			cout << "���������ͼ��" << bookcount << ": " << line << endl;
			//��ÿһ��ͼ��Ķ�Ӧ��Ϣ��������

			//��ά��ṹ
			stringstream ss(line);
			string str;
			vector<string> lineArray;
			//�����ŷָ�
			while (getline(ss, str, ','))
				lineArray.push_back(str);

			//����ָ��õ���ÿһ��string
			strcpy(mybook->bno, lineArray[0].c_str()); //���ת��
			strcpy(mybook->category, lineArray[1].c_str()); //���ת��
			strcpy(mybook->title, lineArray[2].c_str()); //����ת��
			strcpy(mybook->press, lineArray[3].c_str()); //������ת��
			mybook->year = atoi(lineArray[4].c_str());   //���ת��
			strcpy(mybook->author, lineArray[5].c_str()); //����ת��
			mybook->price = atof(lineArray[6].c_str());  //�۸�ת��

			//�ȶ�primary key��λ����һ���ж�
			while (strlen(mybook->bno) != 10)
			{
				printf("���Ϊ%sλ��������������ȷ�����(10λ)��\n", mybook->bno);
				cin >> mybook->bno;
				getchar(); //�����з�
			}

			//����ͼ������ӳ���
			rt = Book_Insertion(mysql, tablename, mybook->bno, mybook->category, mybook->title,
				mybook->press, mybook->year, mybook->author, mybook->price);
			if (rt == 1)
			{
				printf("***********���Ϊ%s��ͼ�����ʧ��! ***********\n", mybook->bno);
			}

			bookcount++;
		}
	}
	system("pause");

}

int  Book_Insertion(MYSQL* mysql, const char* tablename, char* bno,
	char* category, char* title, char* press, int year, char* author, double price)
{
	int ERROR_CODE = 0;
	char temp[MAX_BUFF];
	int rt;			//return value
	MYSQL_RES* res; //һ��������ṹ��
	MYSQL_ROW row;  //char** ��ά���飬���һ������¼
	int total;		//���
	int stock;		//����

	//����ǰ�Ȳ�ѯ���ݿ��Ƿ����м�¼
	sprintf(temp, "select * from %s where bno = '%s'", tablename, bno);
	rt = mysql_query(mysql, temp);
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(mysql));
		ERROR_CODE = 1;
		//����������ͷ���
		return ERROR_CODE;

	}
	/*else
	{
		printf("%s executed!!!\n", temp);
	}*/

	//��ȡ�����
	res = mysql_store_result(mysql);
	row = mysql_fetch_row(res);
	if (!row)	//book��û����ͬ����ţ����������
	{
		total = 1;
		stock = 1;
		//Insert���
		sprintf(temp, "INSERT INTO %s VALUES ('%s','%s','%s','%s',%d,'%s',%.2f,%d,%d)",
			tablename, bno, category, title, press, year, author, price, total, stock);
		rt = mysql_query(mysql, temp);
		if (rt)
		{
			printf("Error making query: %s !!!\n", mysql_error(mysql));
			ERROR_CODE = 1;
			return ERROR_CODE;
		}
		else
		{
			printf("%s executed!!!\n", temp);
		}
		printf("*******************************************\n");
		printf("���Ϊ %s �����Ѿ��ɹ����!\n", bno);

	}
	else  //book��������ͬ����ţ��������ݿ�������������Ϳ��
	{
		int flag = 0;
		//�Ƚ����
		if (strcmp(row[1], category) != 0)
		{
			flag = 1;
			//��������
			sprintf(temp, "UPDATE %s SET category = '%s' where bno = '%s'", tablename, category, bno);
			rt = mysql_query(mysql, temp);
			if (rt)
			{
				printf("Error making query: %s !!!\n", mysql_error(mysql));
				ERROR_CODE = 1;
				return ERROR_CODE;
			}
			else
			{
				printf("%s executed!!!\n", temp);
			}
			printf("*******************************************\n");
			printf("���Ϊ %s ����������³ɹ�!\n", bno);
		}

		//�Ƚ�����
		if (strcmp(row[2], title) != 0)
		{
			flag = 1;
			//��������
			sprintf(temp, "UPDATE %s SET title = '%s' where bno = '%s'", tablename, title, bno);
			rt = mysql_query(mysql, temp);
			if (rt)
			{
				printf("Error making query: %s !!!\n", mysql_error(mysql));
				ERROR_CODE = 1;
				return ERROR_CODE;
			}
			else
			{
				printf("%s executed!!!\n", temp);
			}
			printf("*******************************************\n");
			printf("���Ϊ %s ���������������!\n", bno);

		}

		//�Ƚϳ�����
		if (strcmp(row[3], press) != 0)
		{
			flag = 1;
			//��������
			sprintf(temp, "UPDATE %s SET press = '%s' where bno = '%s'", tablename, press, bno);
			rt = mysql_query(mysql, temp);
			if (rt)
			{
				printf("Error making query: %s !!!\n", mysql_error(mysql));
				ERROR_CODE = 1;
				return ERROR_CODE;
			}
			else
			{
				printf("%s executed!!!\n", temp);
			}
			printf("*******************************************\n");
			printf("���Ϊ %s ����ĳ����̸�����!\n", bno);

		}

		//�Ƚ����
		//�Ƚ����ת��Ϊint����
		int oldyear = atoi(row[4]);
		if (year != oldyear)
		{
			flag = 1;
			//��������
			sprintf(temp, "UPDATE %s SET year = %d where bno = '%s'", tablename, year, bno);
			rt = mysql_query(mysql, temp);
			if (rt)
			{
				printf("Error making query: %s !!!\n", mysql_error(mysql));
				ERROR_CODE = 1;
				return ERROR_CODE;
			}
			else
			{
				printf("%s executed!!!\n", temp);
			}
			printf("*******************************************\n");
			printf("���Ϊ %s ����ķ�����ݸ�����!\n", bno);
		}

		//�Ƚ�����
		if (strcmp(row[5], author) != 0)
		{
			flag = 1;
			//��������
			sprintf(temp, "UPDATE %s SET author = '%s' where bno = '%s'", tablename, author, bno);
			rt = mysql_query(mysql, temp);
			if (rt)
			{
				printf("Error making query: %s !!!\n", mysql_error(mysql));
				ERROR_CODE = 1;
				return ERROR_CODE;
			}
			else
			{
				printf("%s executed!!!\n", temp);
			}
			printf("*******************************************\n");
			printf("���Ϊ %s ��������߸�����!\n", bno);
		}

		//�Ƚϼ۸�
		//�Ƚ��۸�ת��Ϊdouble����
		double oldprice = atof(row[6]);
		if (price != oldprice)
		{
			flag = 1;
			//��������
			sprintf(temp, "UPDATE %s SET price = %.2f where bno = '%s'", tablename, price, bno);
			rt = mysql_query(mysql, temp);
			if (rt)
			{
				printf("Error making query: %s !!!\n", mysql_error(mysql));
				ERROR_CODE = 1;
				return ERROR_CODE;
			}
			else
			{
				printf("%s executed!!!\n", temp);
			}
			printf("*******************************************\n");
			printf("���Ϊ %s ����ļ۸������!\n", bno);
		}

		//������������Զ�һ������ô�����ӿ����ܲ�
		if (flag == 0)
		{
			sprintf(temp, "UPDATE %s SET total=total+1,stock=stock+1 where bno = '%s'", tablename, bno);
			rt = mysql_query(mysql, temp);
			if (rt)
			{
				printf("Error making query: %s !!!\n", mysql_error(mysql));
				ERROR_CODE = 1;
				return ERROR_CODE;
			}
			else
			{
				printf("%s executed!!!\n", temp);
			}

			printf("*******************************************\n");
			printf("���Ϊ %s ����Ŀ�������������!\n", bno);
		}


	}

	//�ͷŽ����
	mysql_free_result(res);

	return ERROR_CODE;

}