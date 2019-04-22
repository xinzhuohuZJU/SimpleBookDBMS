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
	MYSQL mysql;    //一个数据库结构体
	MYSQL_RES* res; //一个结果集结构体
	MYSQL_ROW row;  //char** 二维数组，存放一条条记录

	//数据库连接信息
	const char* HOSTNAME = "localhost";
	const char* USER = "root";
	const char* PASSWORD = "970119";
	const char* DATABASENAME = "hxzlibrary";
	int MyPort = 3306;

	int user_choice;	//存放用户选项

	printf("*******************************************************************\n");
	printf("\t\t\t图书管理系统\n");
	printf("*******************************************************************\n");

	//初始化数据库
	mysql_init(&mysql);
	//设置编码方式
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
	//连接数据库
	//判断如果连接失败就输出连接失败。
	if (mysql_real_connect(&mysql, HOSTNAME, USER, PASSWORD, DATABASENAME, MyPort, NULL, 0) == NULL)
		printf("连接失败！\n");

	while (1)
	{
		printf("1.图书入库\n");
		printf("2.图书查询\n");
		printf("3.借书\n");
		printf("4.还书\n");
		printf("5.借阅证管理\n");
		printf("0.退出本系统\n");
		printf("请输入需要的服务编号:\n");

		scanf("%d", &user_choice);

		switch (user_choice)
		{
		case 0:
			//断开连接，退出本程序：
			//关闭数据库
			mysql_close(&mysql);
			printf("***************您即将退出本系统！***************\n");
			system("pause");
			exit(0);

		case 1:
			//图书入库
			ADD_Book(&mysql);
			break;

		case 2:
			//图书查询
			Check_Book(&mysql);
			break;
		case 3:
			//借书
			Borrow_Book(&mysql);
			break;
		case 4:
			//还书
			Return_Book(&mysql);
			break;
		case 5:
			//借阅证管理
			Proof_Manag(&mysql);
			break;

		default:
			printf("服务编号错误！\n");
			printf("请输入正确的服务器编号：\n");
			printf("\n");
			break;
		}
	}
		
	return 0;
}

