#pragma once
#define MAX_BUFF 1024

#include<cstdio>
#include<iostream>
#include<iomanip>
#include<string>
#include<mysql.h>

using namespace std;

void Proof_Manag(MYSQL* mysql);		//借阅证管理模块
void Add_Manag(MYSQL* mysql);    //借阅证登记模块
void Del_Manag(MYSQL* mysql);	 //借阅证注销模块
void Upd_Manag(MYSQL* mysql);	 //借阅证更新模块
void Del_Card(MYSQL* mysql, const char* tablename, char* cno);  //卡号删除模块 
int Query_Manag(MYSQL* mysql, const char* tablename, char* cno); //卡号查询模块
int Query_ReturnRecord(MYSQL* mysql, char* cno);		//查询该卡是否还有未归还的书籍

//借阅证管理模块
void Proof_Manag(MYSQL* mysql)
{
	int user_choice;
	printf("******借阅证管理模块******\n");
	printf("1.借阅证登记(添加)\n");
	printf("2.借阅证注销(删除)\n");
	printf("3.借阅证更新\n");
	printf("请输入需要的服务编号:\n");

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
		printf("服务编号错误！\n");
		printf("请输入正确的服务器编号：\n");
		printf("\n");
		break;
	}

}

//借阅证登记模块
void Add_Manag(MYSQL* mysql)
{
	char cno[10];     //借书证卡号
	char name[20];    // 姓名
	char dept_name[40];//所属学院名称
	char type;       //类别

	const char* tablename = "card";
	int  ReturnValue;
	char temp[MAX_BUFF];

	printf("******借阅证登记模块******\n");
	printf("请您输入即将登记的借阅证卡号:\n");
	cin >> cno;
	getchar();		//处理换行符
	while (strlen(cno) != 7)
	{
		printf("卡号位数错误！请输入正确的卡号(7位)：\n");
		cin >> cno;
		getchar(); //处理换行符
	}

	ReturnValue = Query_Manag(mysql, tablename, cno);

	if (ReturnValue)	//数据库内已有相同的卡号
	{
		cout << "借书证卡号已存在！请重试！" << endl;
		return;
	}
	//若没有相同的卡号则向数据库内插入数据
	printf("请您输入用于登记的姓名:\n");
	cin.getline(name, 20);
	printf("请您输入用于登记的您的所属院系:\n");
	cin.getline(dept_name, 40);
	printf("请您输入您的类别,T代表教师,S代表学生:\n");
	cin >> type;
	//check类别
	while (type != 'T'&& type != 'S')
	{
		printf("类别输入错误！请输入正确的类别(T代表教师,S代表学生)：\n");
		cin >> type;
	}

	//向数据库内插入借阅证卡号
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

	printf("**********卡号为 %s 的借书证登记成功!**********\n", cno);

	system("pause");

}
//借阅证注销模块
void Del_Manag(MYSQL* mysql)
{
	char cno[10];     //借书证卡号

	const char* tablename = "card";
	int  ReturnValue;
	char temp[MAX_BUFF];

	MYSQL_RES* res; //一个结果集结构体
	MYSQL_ROW row;  //char** 二维数组，存放一条条记录

	char user_choice;

	printf("******借阅证注销模块******\n");
	printf("请您输入想要注销的借阅证卡号:\n");
	cin >> cno;

	//在card表内查询该卡是否存在
	ReturnValue = Query_Manag(mysql, tablename, cno);
	if (!ReturnValue)
	{
		printf("卡号为 %s 的借书证不存在!请重试\n", cno);
		system("pause");
		return;
	}

	//如果卡存在，打印该卡信息并向用户确认是否真的想要删除
	sprintf(temp, "select * from %s where cno = '%s'", tablename, cno);
	mysql_query(mysql, temp);
	//获取结果集
	res = mysql_store_result(mysql);

	cout << "*******************查询结果*******************" << endl;
	cout << "卡号" << setw(20) << "姓名" << setw(15) << "学院" << setw(25) << "类型" << endl;
	//给ROW赋值，判断ROW是否为空，不为空就打印数据。
	while (row = mysql_fetch_row(res))
	{
		cout << row[0] << setw(15) << row[1] << setw(20) << row[2] << setw(20) << row[3] << endl;
	}

	cout << "确认删除该条借书证记录吗(Y or N)" << endl;
	cin >> user_choice;
	switch (user_choice)
	{
	case 'Y':
		//首先判断该卡是否还有未归还的书籍
		ReturnValue = Query_ReturnRecord(mysql, cno);
		if (ReturnValue>0)
		{
			cout << "*****************************************" << endl;
			cout << "该借阅证下还有未归还的书籍！无法删除！请先还书！" << endl;
			cout << "*******************************************" << endl;
			break;
		}
		//删除该卡
		Del_Card(mysql, tablename, cno);	//调用删除子程序
		//释放结果集
		mysql_free_result(res);
		break;
	case 'N':
		printf("******撤销该次删除成功!******\n");
		//释放结果集
		mysql_free_result(res);
		break;
	default:
		printf("请输入Y表示确认,或N表示返回！\n");
		//释放结果集
		mysql_free_result(res);
		break;
	}

}

//借阅证更新模块
void Upd_Manag(MYSQL* mysql)
{
	char cno[10];     //借书证卡号
	char name[20];    // 姓名
	char dept_name[40];//所属学院名称
	char type;       //类别

	const char* tablename = "card";
	int  ReturnValue;
	char temp[MAX_BUFF];

	MYSQL_RES* res; //一个结果集结构体
	MYSQL_ROW row;  //char** 二维数组，存放一条条记录

	printf("******借阅证更新模块******\n");
	printf("请您输入即将更新的借阅证卡号:\n");
	cin >> cno;
	getchar();		//处理换行符
	while (strlen(cno) != 7)
	{
		printf("卡号位数错误！请输入正确的书号(7位)：\n");
		cin >> cno;
		getchar(); //处理换行符
	}
	//查询数据库内是否存在该借阅证
	ReturnValue = Query_Manag(mysql, tablename, cno);

	if (!ReturnValue)	//数据库内没有对应的卡号
	{
		cout << "*****************************************" << endl;
		cout << "借书证卡号不存在！请首先登记！" << endl;
		cout << "*****************************************" << endl;
		return;
	}

	//若没有相同的卡号则向数据库内插入数据
	printf("请您输入新借书证的姓名:\n");
	cin.getline(name, 20);
	printf("请您输入新借书证用户的所属院系:\n");
	cin.getline(dept_name, 40);
	printf("请您输入新借书证用户的类别,T代表教师,S代表学生:\n");
	cin >> type;
	//check类别
	while (type != 'T'&& type != 'S')
	{
		printf("类别输入错误！请输入正确的类别(T代表教师,S代表学生)：\n");
		cin >> type;
	}

	//向数据库内更新该借阅证
	sprintf(temp, "select * from %s where cno = '%s'", tablename, cno);
	mysql_query(mysql, temp);
	//获取结果集
	res = mysql_store_result(mysql);
	//借阅证内容只有一行
	row = mysql_fetch_row(res);

	//姓名不相等
	if (strcmp(row[1], name) != 0)
	{
		//更新数据
		sprintf(temp, "UPDATE %s SET name = '%s' where cno = '%s'", tablename, name, cno);
		ReturnValue = mysql_query(mysql, temp);
		if (ReturnValue)
		{
			printf("Error making query: %s !!!\n", mysql_error(mysql));
			printf("*******************************************\n");
			printf("卡号为 %s 的姓名更新失败!\n", cno);
		}
		else
		{
			printf("%s executed!!!\n", temp);
			printf("*******************************************\n");
			printf("卡号为 %s 的姓名更新成功!\n", cno);
		}

	}

	//学院名称不相等
	if (strcmp(row[2], dept_name) != 0)
	{
		//更新数据
		sprintf(temp, "UPDATE %s SET department = '%s' where cno = '%s'", tablename, dept_name, cno);
		ReturnValue = mysql_query(mysql, temp);
		if (ReturnValue)
		{
			printf("Error making query: %s !!!\n", mysql_error(mysql));
			printf("*******************************************\n");
			printf("卡号为 %s 的学院更新失败!\n", cno);
		}
		else
		{
			printf("%s executed!!!\n", temp);
			printf("*******************************************\n");
			printf("卡号为 %s 的学院更新成功!\n", cno);
		}

	}

	//类别更新
	if (type != row[3][0])
	{
		//更新数据
		sprintf(temp, "UPDATE %s SET type = '%c' where cno = '%s'", tablename, type, cno);
		ReturnValue = mysql_query(mysql, temp);
		if (ReturnValue)
		{
			printf("Error making query: %s !!!\n", mysql_error(mysql));
			printf("*******************************************\n");
			printf("卡号为 %s 的类别更新失败!\n", cno);
		}
		else
		{
			printf("%s executed!!!\n", temp);
			printf("*******************************************\n");
			printf("卡号为 %s 的类别更新成功!\n", cno);
		}

	}

	//释放结果集
	mysql_free_result(res);

	system("pause");
}

//卡号查询模块，返回card表内指定卡号是否存在
int Query_Manag(MYSQL* mysql, const char* tablename, char* cno)
{
	MYSQL_RES* res; //一个结果集结构体
	MYSQL_ROW row;  //char** 二维数组，存放一条条记录
	long ResultRows;
	char temp[MAX_BUFF];
	int rt;      //return value;

	//判断卡号是否已经在数据库内
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

	//获取结果集
	res = mysql_store_result(mysql);
	ResultRows = (long)mysql_num_rows(res);	//Returns the number of rows in the result set.

	//释放结果集
	mysql_free_result(res);

	return (int)ResultRows;
}
//实际的卡号删除模块
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
		printf("卡号为 %s 的借书证已经成功删除!\n", cno);
	}

}

//查询该卡号是否还有未归还的书籍：
int Query_ReturnRecord(MYSQL* mysql, char* cno)
{
	char temp[MAX_BUFF];
	MYSQL_RES* res; //一个结果集结构体
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

	//获取结果集
	res = mysql_store_result(mysql);
	ResultRows = (long)mysql_num_rows(res);	//Returns the number of rows in the result set.


	//释放结果集
	mysql_free_result(res);

	return (int)ResultRows;
}