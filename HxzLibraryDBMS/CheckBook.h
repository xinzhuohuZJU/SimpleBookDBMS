#pragma once
#define MAX_BUFF 1024

#include<cstdio>
#include<iostream>
#include<iomanip>
#include<string>
#include<mysql.h>

using namespace std;

void Check_Book(MYSQL* mysql);            //图书查询
void Check_Book_ByCategory(MYSQL* mysql); //图书类别查询
void Check_Book_ByTitle(MYSQL* mysql);	 //图书书名查询
void Check_Book_ByPress(MYSQL* mysql);	//图书出版商查询
void Check_Book_ByYear(MYSQL* mysql);	//图书年份区间查询
void Check_Book_ByAuthor(MYSQL* mysql); //图书作者查询
void Check_Book_ByPrice(MYSQL* mysql);  //图书价格区间查询
void Check_Book_Print(MYSQL* mysql, MYSQL_RES* res, const char* tablename); //用于打印查询结果的函数

//图书查询模块
void Check_Book(MYSQL* mysql)
{
	int user_choice;	//保存用户选项
	printf("******图书查询模块******\n");
	printf("1.图书类别查询\n");
	printf("2.图书书名查询\n");
	printf("3.图书出版社查询\n");
	printf("4.图书年份查询\n");
	printf("5.图书作者查询\n");
	printf("6.图书价格查询\n");
	printf("请输入需要的服务编号:\n");

	scanf("%d", &user_choice);

	switch (user_choice)
	{
	case 1:
		//图书类别查询
		Check_Book_ByCategory(mysql);
		break;
	case 2:
		//图书书名查询
		Check_Book_ByTitle(mysql);
		break;
	case 3:
		//图书出版商查询
		Check_Book_ByPress(mysql);
		break;
	case 4:
		//图书年份区间查询
		Check_Book_ByYear(mysql);
		break;
	case 5:
		//图书作者查询
		Check_Book_ByAuthor(mysql);
		break;
	case 6:
		//图书价格查询
		Check_Book_ByPrice(mysql);
		break;
	default:
		printf("服务编号错误！\n");
		printf("请输入正确的服务器编号\n");
		printf("\n");
		break;
	}
}
//图书查询类别模块
void Check_Book_ByCategory(MYSQL* mysql)
{
	const char* tablename = "book";
	MYSQL_RES* res; //一个结果集结构体
	char temp[MAX_BUFF];
	char userinput[MAX_BUFF];
	int rt;

	printf("******图书类别查询模块******\n");
	printf("请您输入要查询的图书类别:\n");
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

	//获取结果集
	res = mysql_store_result(mysql);
	//调用打印结果程序
	Check_Book_Print(mysql, res, tablename);

	//释放结果集
	mysql_free_result(res);

	system("pause");
}
//图书查询标题模块
void Check_Book_ByTitle(MYSQL* mysql)
{

	const char* tablename = "book";
	MYSQL_RES* res; //一个结果集结构体
	char temp[MAX_BUFF];
	char userinput[MAX_BUFF];
	int rt;

	printf("******图书书名查询模块******\n");
	printf("请您输入要查询的图书标题:\n");
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

	//获取结果集
	res = mysql_store_result(mysql);
	//调用打印结果程序
	Check_Book_Print(mysql, res, tablename);

	//释放结果集
	mysql_free_result(res);

	system("pause");

}

//图书查询出版商模块
void Check_Book_ByPress(MYSQL* mysql)
{
	const char* tablename = "book";
	MYSQL_RES* res; //一个结果集结构体
	char temp[MAX_BUFF];
	char userinput[MAX_BUFF];
	int rt;

	printf("******图书出版商查询模块******\n");
	printf("请您输入要查询的图书的出版商:\n");
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

	//获取结果集
	res = mysql_store_result(mysql);
	//调用打印结果程序
	Check_Book_Print(mysql, res, tablename);

	//释放结果集
	mysql_free_result(res);

	system("pause");

}

//图书查询年份区间模块
void Check_Book_ByYear(MYSQL* mysql)
{
	const char* tablename = "book";
	MYSQL_RES* res; //一个结果集结构体
	char temp[MAX_BUFF];
	int rt;

	int LowerBound;  //年份下限
	int UpperBound;	 //年份上限
	int tempBound;

	printf("******图书年份区间查询模块******\n");
	cout << "请您输入要查询的年份下限(0为无下限):" << endl;
	cin >> LowerBound;
	cout << "请您输入要查询的年份上限(0为无上限):" << endl;
	cin >> UpperBound;

	//如果上下限有负数，那么转换为0
	if (LowerBound < 0) LowerBound = 0;
	if (UpperBound < 0) UpperBound = 0;

	//如果下限大于上限并且上限不等于0，那么置换二者
	if ((LowerBound > UpperBound) && UpperBound != 0)
	{
		tempBound = LowerBound;
		LowerBound = UpperBound;
		UpperBound = tempBound;
	}

	if (LowerBound != 0 && UpperBound != 0)
	{
		//二者都不为0
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
		//无下限
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
		//无上限
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
		//二者均为0
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

	//获取结果集
	res = mysql_store_result(mysql);
	//调用打印结果程序
	Check_Book_Print(mysql, res, tablename);

	//释放结果集
	mysql_free_result(res);
	system("pause");
}

//图书价格区间查询
void Check_Book_ByPrice(MYSQL* mysql)
{
	const char* tablename = "book";
	MYSQL_RES* res; //一个结果集结构体
	char temp[MAX_BUFF];
	int rt;

	double LowerBound;  //价格下限
	double UpperBound;	 //价格上限
	double tempBound;

	printf("******图书价格区间查询模块******\n");
	cout << "请您输入要查询的价格下限(0为无下限):" << endl;
	cin >> LowerBound;
	cout << "请您输入要查询的价格上限(0为无上限):" << endl;
	cin >> UpperBound;

	//如果上下限有负数，那么转换为0
	if (LowerBound < 0) LowerBound = 0;
	if (UpperBound < 0) UpperBound = 0;

	//如果下限大于上限并且上限不等于0，那么置换二者
	if ((LowerBound > UpperBound) && UpperBound != 0)
	{
		tempBound = LowerBound;
		LowerBound = UpperBound;
		UpperBound = tempBound;
	}

	if (LowerBound != 0 && UpperBound != 0)
	{
		//二者都不为0
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
		//无下限
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
		//无上限
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
		//二者均为0
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

	//获取结果集
	res = mysql_store_result(mysql);
	//调用打印结果程序
	Check_Book_Print(mysql, res, tablename);

	//释放结果集
	mysql_free_result(res);
	system("pause");

}

//图书作者查询模块
void Check_Book_ByAuthor(MYSQL* mysql)
{
	const char* tablename = "book";
	MYSQL_RES* res; //一个结果集结构体
	char temp[MAX_BUFF];
	char userinput[MAX_BUFF];
	int rt;

	printf("******图书作者查询模块******\n");
	printf("请您输入要查询的图书的作者:\n");
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

	//获取结果集
	res = mysql_store_result(mysql);
	//调用打印结果程序
	Check_Book_Print(mysql, res, tablename);

	//释放结果集
	mysql_free_result(res);

	system("pause");
}

//打印查询结果模块(右对齐)
void Check_Book_Print(MYSQL* mysql, MYSQL_RES* res, const char* tablename)
{
	MYSQL_ROW row;  //char** 二维数组，存放一条条记录
	long ResultRows;


	cout << "*******************查询结果*******************" << endl;
	ResultRows = (long)mysql_num_rows(res);	//Returns the number of rows in the result set.
	if (ResultRows == 0)
	{
		cout << "Empty Set!" << endl;
	}
	else
	{
		//显示数据
		//cout << setiosflags(ios::left);
		cout << "书号" << setw(25) << "类别" << setw(30) << "书名" <<
			setw(20) << "出版商" << setw(15) << "年份" << setw(15) << "作者" << setw(15) << "价格" <<
			setw(15) << "总量" << setw(15) << "库存" << endl;
		//给ROW赋值，判断ROW是否为空，不为空就打印数据
		//逐行打印数据
		while (row = mysql_fetch_row(res))
		{

			//cout << setiosflags(ios::left);
			cout << row[0] << setw(20) << row[1] << setw(30) << row[2] <<
				setw(20) << row[3] << setw(15) << row[4] << setw(15) << row[5] << setw(15) << row[6] <<
				setw(15) << row[7] << setw(15) << row[8] << endl;

		}

	}

}