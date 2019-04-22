#pragma once
#define MAX_BUFF 1024

#include<cstdio>
#include<iostream>
#include<mysql.h>
#include<atltime.h>
#include<Windows.h>
#include"ProofManag.h"

using namespace std;

void Borrow_Book(MYSQL* mysql);		//借书模块
void BorrowRecord_Query(MYSQL* mysql); //借书记录查询
void BorrowBook_Action(MYSQL* mysql);   //借书动作执行模块
void Return_Book(MYSQL* mysql);		//还书模块
void ReturnBook_Action(MYSQL* mysql);  //还书动作执行模块

int BookStore_Query(MYSQL* mysql, char* bno); //查询书号是否存在/库存情况
void Obtain_ReturnTime(MYSQL* mysql, char* rtime, char* bno); //获得对应书号最近的归还时间
void Obtain_CurrentTime(char* time);		//获得系统当前时间 格式为YYYY-MM-DD HH:MM:SS
void UpdateBookStock(MYSQL* mysql, char* bno);  //更新库存

//借书模块
void Borrow_Book(MYSQL* mysql)
{
	int user_choice;
	printf("***********借书模块***********\n");
	printf("1.借书记录查询\n");
	printf("2.借书\n");

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
		printf("服务编号错误！\n");
		printf("请输入正确的服务器编号\n");
		printf("\n");
		break;
	}

}

//借书记录查询模块
void BorrowRecord_Query(MYSQL* mysql)
{
	char cno[40];
	char temp[MAX_BUFF];
	long ResultRows;	//结果集的行数
	int ReturnValue;

	MYSQL_RES* res; //一个结果集结构体
	MYSQL_ROW row;  //char** 二维数组，存放一条条记录

	printf("***********借书记录查询模块***********\n");

	//输入借书证卡号
	printf("请您输入即将查询的借书证卡号：\n");
	cin >> cno;
	getchar();		//处理换行符
	while (strlen(cno) != 7)
	{
		printf("卡号位数错误！请输入正确的卡号(7位)：\n");
		cin >> cno;
		getchar(); //处理换行符
	}

	//先查询卡号是否已登记
	//调用ProofMang中的Query_Manag()函数
	ReturnValue = Query_Manag(mysql, "card", cno);
	if (!ReturnValue)
	{
		cout << "************************************************" << endl;
		cout << "查询的借书证卡号不存在！请先进行登记！" << endl;
		cout << "************************************************" << endl;
		return;
	}

	//卡号存在
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

	//获取结果集
	res = mysql_store_result(mysql);
	ResultRows = (long)mysql_num_rows(res);	//Returns the number of rows in the result set.

	cout << "*******************查询结果*******************" << endl;
	if (!ResultRows)
	{
		cout << "Empty Set!" << endl;

	}
	else
	{
		cout << "书号" << setw(25) << "类别" << setw(30) << "书名" << setw(25) << "借书日期" << setw(30) << "还书日期" << endl;
		//给ROW赋值，判断ROW是否为空，不为空就打印数据。
		while (row = mysql_fetch_row(res))
		{
			cout << row[0] << setw(25) << row[1] << setw(30) << row[2] << setw(30) << row[3] << setw(30) << row[4] << endl;
		}
	}

	//释放结果集
	mysql_free_result(res);

	system("pause");

}

//借书动作执行模块
void BorrowBook_Action(MYSQL* mysql)
{
	char cno[40];
	char bno[40];
	char rtime[40];
	char temp[MAX_BUFF];
	int ReturnValue;
	//int UpdStock;

	printf("***********借书动作执行模块***********\n");
	//输入借书证卡号
	printf("请输入您的借书证卡号：\n");
	cin >> cno;
	getchar();		//处理换行符
	while (strlen(cno) != 7)
	{
		printf("卡号位数错误！请输入正确的卡号(7位)：\n");
		cin >> cno;
		getchar(); //处理换行符
	}

	//先查询卡号是否已登记
	//调用ProofMang中的Query_Manag()函数
	ReturnValue = Query_Manag(mysql, "card", cno);
	if (!ReturnValue)
	{
		cout << "************************************************" << endl;
		cout << "该借书证卡号不存在！请先进行登记！" << endl;
		cout << "************************************************" << endl;
		return;
	}

	//输入书号
	printf("请您输入即将借阅的书号：\n");
	cin >> bno;
	getchar();		//处理换行符
	while (strlen(bno) != 10)
	{
		printf("书号位数错误！请输入正确的书号(10位)：\n");
		cin >> bno;
		getchar(); //处理换行符
	}

	//查询查询书号是否存在/库存情况
	ReturnValue = BookStore_Query(mysql, bno);

	if (ReturnValue == -1)
	{
		cout << "*****************************" << endl;
		cout << "该书号不存在！请确认所输入书号是否在数据库中！" << endl;
		cout << "*****************************" << endl;

	}
	else if (ReturnValue == 0)
	{
		cout << "*****************************" << endl;
		printf("书号为 %s 的图书目前暂无库存！\n", bno);
		//获得该书最近归还的时间
		Obtain_ReturnTime(mysql, rtime, bno);
		//如果该书还未有人归还过
		if (!strcmp(rtime, "0000-00-00 00:00:00")) printf("书号为 %s 的图书未查询到有归还记录！\n", bno);
		else printf("书号为 %s 的图书最近的归还时间为: %s\n", bno, rtime);

	}
	else
	{

		//还有库存，可以成功借书
		Obtain_CurrentTime(rtime);
		//借书时return date 默认设置为 0000-00-00 00:00:00
		sprintf(temp,
			"INSERT INTO borrow VALUES('%s','%s','%s',0)",
			cno, bno, rtime);

		int rt = mysql_query(mysql, temp);
		if (rt)
		{
			printf("Error making query: %s !!!\n", mysql_error(mysql));
			cout << "*************************************" << endl;
			cout << "更新borrow关系时失败！" << endl;
			return;
		}
		//更新库存
		//调用UpdateBookStock函数
		UpdateBookStock(mysql, bno);
		cout << "*************************************" << endl;
		printf("借阅证卡号为 %s 的用户借书成功!\n", cno);
	}


	system("pause");

}

//查询书号是否存在/库存情况
int BookStore_Query(MYSQL* mysql, char* bno)
{
	int ReturnCode;
	char temp[MAX_BUFF];

	MYSQL_RES* res; //一个结果集结构体
	MYSQL_ROW row;  //char** 二维数组，存放一条条记录
	long ResultRows; //MySQL查询的行数

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

	//获取结果集
	res = mysql_store_result(mysql);
	ResultRows = (long)mysql_num_rows(res);	//Returns the number of rows in the result set.

	if (!ResultRows) //结果为空
	{
		ReturnCode = -1;

	}
	else
	{
		//获取stock
		row = mysql_fetch_row(res);
		int stock = atoi(row[0]);
		ReturnCode = stock;
	}

	//释放结果集
	mysql_free_result(res);
	return ReturnCode;
}

//获得对应书号最近的归还时间
void Obtain_ReturnTime(MYSQL* mysql, char* rtime, char* bno)
{
	char temp[MAX_BUFF];

	MYSQL_RES* res; //一个结果集结构体
	MYSQL_ROW row;  //char** 二维数组，存放一条条记录
	int rt;         //return value

	//降序排列当前书号的return_date
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

	//获取结果集
	res = mysql_store_result(mysql);
	row = mysql_fetch_row(res);

	//将获取到的结果赋给rtime
	strcpy(rtime, row[0]);

	//释放结果集
	mysql_free_result(res);
	return;
}

//获得系统当前的时间
void Obtain_CurrentTime(char* time)
{
	char temp[30];
	CTime curTime = CTime::GetCurrentTime();
	int nYear = curTime.GetYear();  // 获取当前年份   
	int nMonth = curTime.GetMonth(); // 获取当前月份   
	int nDay = curTime.GetDay();   // 获取当前日期   
	int nHour = curTime.GetHour();  // 获取当前小时时间   
	int nMin = curTime.GetMinute(); // 获取当前分钟时间   
	int nSec = curTime.GetSecond(); // 获取当前秒时间   

	sprintf(temp, "%d-%02d-%02d %02d:%02d:%02d", nYear, nMonth, nDay, nHour, nMin, nSec);
	//赋值给time
	strcpy(time, temp);
}
//借书成功时更新对应书号库存，对应书号库存-1
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
		printf("书号为 %s 的图书的库存已经更新!\n", bno);
	}

}
//还书模块
void Return_Book(MYSQL* mysql)
{
	int user_choice;
	printf("***********还书模块***********\n");
	printf("1.借书记录查询\n");
	printf("2.还书\n");

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
		printf("服务编号错误！\n");
		printf("请输入正确的服务器编号\n");
		printf("\n");
		break;
	}

}
//还书动作执行模块
void ReturnBook_Action(MYSQL* mysql)
{

	char cno[40];
	char bno[40];
	char temptime[40];
	char rtime[40];
	char temp[MAX_BUFF];
	int ReturnValue;
	int UpdStock;

	MYSQL_RES* res; //一个结果集结构体
	MYSQL_ROW row;  //char** 二维数组，存放一条条记录
	long ResultRows;

	printf("***********还书动作执行模块***********\n");
	//输入借书证卡号
	printf("请输入您的借书证卡号：\n");
	cin >> cno;
	getchar();		//处理换行符
	while (strlen(cno) != 7)
	{
		printf("卡号位数错误！请输入正确的卡号(7位)：\n");
		cin >> cno;
		getchar(); //处理换行符
	}

	//先查询卡号是否已登记
	//调用ProofMang中的Query_Manag()函数
	ReturnValue = Query_Manag(mysql, "card", cno);
	if (!ReturnValue)
	{
		cout << "************************************************" << endl;
		cout << "该借书证卡号不存在！请先进行登记！" << endl;
		cout << "************************************************" << endl;
		return;
	}

	//输入书号
	printf("请您输入即将归还的书号：\n");
	cin >> bno;
	getchar();		//处理换行符
	while (strlen(bno) != 10)
	{
		printf("书号位数错误！请输入正确的书号(10位)：\n");
		cin >> bno;
		getchar(); //处理换行符
	}

	//在已借书籍列表中查询借书关系是否存在
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

	//获取结果集
	res = mysql_store_result(mysql);
	ResultRows = (long)mysql_num_rows(res);	//Returns the number of rows in the result set.

	if (!ResultRows)
	{
		cout << "***************************************************" << endl;
		printf("卡号为 %s 和 书号为 %s 的借书关系目前不存在，请确认！\n", cno, bno);
	}
	else
	{
		//当同一个用户一次借多本同一书号的书时，归还其借最早的一本书
		row = mysql_fetch_row(res);	//已降序排列，所以处理第一行
		strcpy(temptime, row[2]);

		//先更新该借书记录的还书时间
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
			printf("卡号为 %s 和 书号为 %s 的还书日期已经更新，请确认！\n", cno, bno);
		}

		//再更新该书库存
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
			printf("书号为 %s 的库存已经更新，请确认！\n", bno);
		}

		cout << "**********************************" << endl;
		cout << "还书成功!" << endl;
	}

	//释放结果集
	mysql_free_result(res);
	system("pause");

}