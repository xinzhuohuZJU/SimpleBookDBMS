#pragma once
#define MAX_BUFF 1024

#include<cstdio>
#include<iostream>
#include<iomanip>
#include<string>
#include<mysql.h>

using namespace std;

void Proof_Manag(MYSQL* mysql);		//����֤����ģ��
void Add_Manag(MYSQL* mysql);    //����֤�Ǽ�ģ��
void Del_Manag(MYSQL* mysql);	 //����֤ע��ģ��
void Upd_Manag(MYSQL* mysql);	 //����֤����ģ��
void Del_Card(MYSQL* mysql, const char* tablename, char* cno);  //����ɾ��ģ�� 
int Query_Manag(MYSQL* mysql, const char* tablename, char* cno); //���Ų�ѯģ��
int Query_ReturnRecord(MYSQL* mysql, char* cno);		//��ѯ�ÿ��Ƿ���δ�黹���鼮

//����֤����ģ��
void Proof_Manag(MYSQL* mysql)
{
	int user_choice;
	printf("******����֤����ģ��******\n");
	printf("1.����֤�Ǽ�(���)\n");
	printf("2.����֤ע��(ɾ��)\n");
	printf("3.����֤����\n");
	printf("��������Ҫ�ķ�����:\n");

	scanf("%d", &user_choice);

	switch (user_choice)
	{
	case 1:
		Add_Manag(mysql);
		break;
	case 2:
		Del_Manag(mysql);
		break;
	case 3:
		Upd_Manag(mysql);
		break;
	default:
		printf("�����Ŵ���\n");
		printf("��������ȷ�ķ�������ţ�\n");
		printf("\n");
		break;
	}

}

//����֤�Ǽ�ģ��
void Add_Manag(MYSQL* mysql)
{
	char cno[10];     //����֤����
	char name[20];    // ����
	char dept_name[40];//����ѧԺ����
	char type;       //���

	const char* tablename = "card";
	int  ReturnValue;
	char temp[MAX_BUFF];

	printf("******����֤�Ǽ�ģ��******\n");
	printf("�������뼴���ǼǵĽ���֤����:\n");
	cin >> cno;
	getchar();		//�����з�
	while (strlen(cno) != 7)
	{
		printf("����λ��������������ȷ�Ŀ���(7λ)��\n");
		cin >> cno;
		getchar(); //�����з�
	}

	ReturnValue = Query_Manag(mysql, tablename, cno);

	if (ReturnValue)	//���ݿ���������ͬ�Ŀ���
	{
		cout << "����֤�����Ѵ��ڣ������ԣ�" << endl;
		return;
	}
	//��û����ͬ�Ŀ����������ݿ��ڲ�������
	printf("�����������ڵǼǵ�����:\n");
	cin.getline(name, 20);
	printf("�����������ڵǼǵ���������Ժϵ:\n");
	cin.getline(dept_name, 40);
	printf("���������������,T�����ʦ,S����ѧ��:\n");
	cin >> type;
	//check���
	while (type != 'T'&& type != 'S')
	{
		printf("������������������ȷ�����(T�����ʦ,S����ѧ��)��\n");
		cin >> type;
	}

	//�����ݿ��ڲ������֤����
	sprintf(temp, "INSERT INTO %s VALUES('%s','%s','%s','%c')", tablename, cno, name, dept_name, type);
	ReturnValue = mysql_query(mysql, temp);
	if (ReturnValue)
	{
		printf("Error making query: %s !!!\n", mysql_error(mysql));
	}
	else
	{
		printf("%s executed!!!\n", temp);
	}

	printf("**********����Ϊ %s �Ľ���֤�Ǽǳɹ�!**********\n", cno);

	system("pause");

}
//����֤ע��ģ��
void Del_Manag(MYSQL* mysql)
{
	char cno[10];     //����֤����

	const char* tablename = "card";
	int  ReturnValue;
	char temp[MAX_BUFF];

	MYSQL_RES* res; //һ��������ṹ��
	MYSQL_ROW row;  //char** ��ά���飬���һ������¼

	char user_choice;

	printf("******����֤ע��ģ��******\n");
	printf("����������Ҫע���Ľ���֤����:\n");
	cin >> cno;

	//��card���ڲ�ѯ�ÿ��Ƿ����
	ReturnValue = Query_Manag(mysql, tablename, cno);
	if (!ReturnValue)
	{
		printf("����Ϊ %s �Ľ���֤������!������\n", cno);
		system("pause");
		return;
	}

	//��������ڣ���ӡ�ÿ���Ϣ�����û�ȷ���Ƿ������Ҫɾ��
	sprintf(temp, "select * from %s where cno = '%s'", tablename, cno);
	mysql_query(mysql, temp);
	//��ȡ�����
	res = mysql_store_result(mysql);

	cout << "*******************��ѯ���*******************" << endl;
	cout << "����" << setw(20) << "����" << setw(15) << "ѧԺ" << setw(25) << "����" << endl;
	//��ROW��ֵ���ж�ROW�Ƿ�Ϊ�գ���Ϊ�վʹ�ӡ���ݡ�
	while (row = mysql_fetch_row(res))
	{
		cout << row[0] << setw(15) << row[1] << setw(20) << row[2] << setw(20) << row[3] << endl;
	}

	cout << "ȷ��ɾ����������֤��¼��(Y or N)" << endl;
	cin >> user_choice;
	switch (user_choice)
	{
	case 'Y':
		//�����жϸÿ��Ƿ���δ�黹���鼮
		ReturnValue = Query_ReturnRecord(mysql, cno);
		if (ReturnValue>0)
		{
			cout << "*****************************************" << endl;
			cout << "�ý���֤�»���δ�黹���鼮���޷�ɾ�������Ȼ��飡" << endl;
			cout << "*******************************************" << endl;
			break;
		}
		//ɾ���ÿ�
		Del_Card(mysql, tablename, cno);	//����ɾ���ӳ���
		//�ͷŽ����
		mysql_free_result(res);
		break;
	case 'N':
		printf("******�����ô�ɾ���ɹ�!******\n");
		//�ͷŽ����
		mysql_free_result(res);
		break;
	default:
		printf("������Y��ʾȷ��,��N��ʾ���أ�\n");
		//�ͷŽ����
		mysql_free_result(res);
		break;
	}

}

//����֤����ģ��
void Upd_Manag(MYSQL* mysql)
{
	char cno[10];     //����֤����
	char name[20];    // ����
	char dept_name[40];//����ѧԺ����
	char type;       //���

	const char* tablename = "card";
	int  ReturnValue;
	char temp[MAX_BUFF];

	MYSQL_RES* res; //һ��������ṹ��
	MYSQL_ROW row;  //char** ��ά���飬���һ������¼

	printf("******����֤����ģ��******\n");
	printf("�������뼴�����µĽ���֤����:\n");
	cin >> cno;
	getchar();		//�����з�
	while (strlen(cno) != 7)
	{
		printf("����λ��������������ȷ�����(7λ)��\n");
		cin >> cno;
		getchar(); //�����з�
	}
	//��ѯ���ݿ����Ƿ���ڸý���֤
	ReturnValue = Query_Manag(mysql, tablename, cno);

	if (!ReturnValue)	//���ݿ���û�ж�Ӧ�Ŀ���
	{
		cout << "*****************************************" << endl;
		cout << "����֤���Ų����ڣ������ȵǼǣ�" << endl;
		cout << "*****************************************" << endl;
		return;
	}

	//��û����ͬ�Ŀ����������ݿ��ڲ�������
	printf("���������½���֤������:\n");
	cin.getline(name, 20);
	printf("���������½���֤�û�������Ժϵ:\n");
	cin.getline(dept_name, 40);
	printf("���������½���֤�û������,T�����ʦ,S����ѧ��:\n");
	cin >> type;
	//check���
	while (type != 'T'&& type != 'S')
	{
		printf("������������������ȷ�����(T�����ʦ,S����ѧ��)��\n");
		cin >> type;
	}

	//�����ݿ��ڸ��¸ý���֤
	sprintf(temp, "select * from %s where cno = '%s'", tablename, cno);
	mysql_query(mysql, temp);
	//��ȡ�����
	res = mysql_store_result(mysql);
	//����֤����ֻ��һ��
	row = mysql_fetch_row(res);

	//���������
	if (strcmp(row[1], name) != 0)
	{
		//��������
		sprintf(temp, "UPDATE %s SET name = '%s' where cno = '%s'", tablename, name, cno);
		ReturnValue = mysql_query(mysql, temp);
		if (ReturnValue)
		{
			printf("Error making query: %s !!!\n", mysql_error(mysql));
			printf("*******************************************\n");
			printf("����Ϊ %s ����������ʧ��!\n", cno);
		}
		else
		{
			printf("%s executed!!!\n", temp);
			printf("*******************************************\n");
			printf("����Ϊ %s ���������³ɹ�!\n", cno);
		}

	}

	//ѧԺ���Ʋ����
	if (strcmp(row[2], dept_name) != 0)
	{
		//��������
		sprintf(temp, "UPDATE %s SET department = '%s' where cno = '%s'", tablename, dept_name, cno);
		ReturnValue = mysql_query(mysql, temp);
		if (ReturnValue)
		{
			printf("Error making query: %s !!!\n", mysql_error(mysql));
			printf("*******************************************\n");
			printf("����Ϊ %s ��ѧԺ����ʧ��!\n", cno);
		}
		else
		{
			printf("%s executed!!!\n", temp);
			printf("*******************************************\n");
			printf("����Ϊ %s ��ѧԺ���³ɹ�!\n", cno);
		}

	}

	//������
	if (type != row[3][0])
	{
		//��������
		sprintf(temp, "UPDATE %s SET type = '%c' where cno = '%s'", tablename, type, cno);
		ReturnValue = mysql_query(mysql, temp);
		if (ReturnValue)
		{
			printf("Error making query: %s !!!\n", mysql_error(mysql));
			printf("*******************************************\n");
			printf("����Ϊ %s ��������ʧ��!\n", cno);
		}
		else
		{
			printf("%s executed!!!\n", temp);
			printf("*******************************************\n");
			printf("����Ϊ %s �������³ɹ�!\n", cno);
		}

	}

	//�ͷŽ����
	mysql_free_result(res);

	system("pause");
}

//���Ų�ѯģ�飬����card����ָ�������Ƿ����
int Query_Manag(MYSQL* mysql, const char* tablename, char* cno)
{
	MYSQL_RES* res; //һ��������ṹ��
	MYSQL_ROW row;  //char** ��ά���飬���һ������¼
	long ResultRows;
	char temp[MAX_BUFF];
	int rt;      //return value;

	//�жϿ����Ƿ��Ѿ������ݿ���
	sprintf(temp, "select * from %s where cno = '%s'", tablename, cno);
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
	ResultRows = (long)mysql_num_rows(res);	//Returns the number of rows in the result set.

	//�ͷŽ����
	mysql_free_result(res);

	return (int)ResultRows;
}
//ʵ�ʵĿ���ɾ��ģ��
void Del_Card(MYSQL* mysql, const char* tablename, char* cno)
{
	char temp[MAX_BUFF];
	int rt;
	sprintf(temp, "DELETE from %s where cno = '%s'", tablename, cno);

	rt = mysql_query(mysql, temp);
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(mysql));
	}
	else
	{
		printf("%s executed!!!\n", temp);
		printf("����Ϊ %s �Ľ���֤�Ѿ��ɹ�ɾ��!\n", cno);
	}

}

//��ѯ�ÿ����Ƿ���δ�黹���鼮��
int Query_ReturnRecord(MYSQL* mysql, char* cno)
{
	char temp[MAX_BUFF];
	MYSQL_RES* res; //һ��������ṹ��
	long ResultRows;
	int rt;      //return value;

	//
	sprintf(temp,
		"select * from borrow where cno = '%s' and return_date = 0",
		cno);
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
	ResultRows = (long)mysql_num_rows(res);	//Returns the number of rows in the result set.


	//�ͷŽ����
	mysql_free_result(res);

	return (int)ResultRows;
}