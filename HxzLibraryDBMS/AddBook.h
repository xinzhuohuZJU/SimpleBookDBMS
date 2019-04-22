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

void ADD_Book(MYSQL* mysql);	    //图书入库
void ADD_Book_Single(MYSQL* mysql); //图书单本入库
void ADD_Book_Batch(MYSQL* mysql);	//图书批量入库
int  Book_Insertion(MYSQL* mysql, const char* tablename, char* bno,
	char* category, char* title, char* press, int year, char* author, double price); //图书插入

//图书入库模块
void ADD_Book(MYSQL* mysql)
{
	int user_choice;	//保存用户选项
	printf("******图书入库模块******\n");
	printf("1.单本入库\n");
	printf("2.批量入库\n");
	printf("请输入需要的服务编号:\n");

	scanf("%d", &user_choice);

	switch (user_choice)
	{
	case 1:
		//单本入库模块
		ADD_Book_Single(mysql);
		break;
	case 2:
		//批量入库模块
		ADD_Book_Batch(mysql);
		break;

	default:
		printf("服务编号错误！\n");
		printf("请输入正确的服务器编号：\n");
		printf("\n");
		break;
	}

}
//图书单本入库模块
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


	printf("******图书单本入库模块******\n");
	printf("请您输入书号:\n");
	cin >> bno;	//输入书号
	getchar();  //处理换行符
	while (strlen(bno) != 10)
	{
		printf("书号位数错误！请输入正确的书号(10位)：\n");
		cin >> bno;
		getchar(); //处理换行符
	}
	printf("请您输入书的类别:\n");
	//类别中可能有空格,使用cin.getline
	cin.getline(category, 20);
	printf("请您输入书的标题:\n");
	cin.getline(title, 30);
	printf("请您输入书的出版商:\n");
	cin.getline(press, 30);
	printf("请您依次输入书的发行年份 作者 价格:\n");
	cin >> year >> author >> price;

	//调用图书插入子程序
	rt = Book_Insertion(mysql, tablename, bno, category, title, press, year, author, price);
	if (rt == 1)
	{
		printf("***********图书入库失败！请重试！***********\n");
	}

	system("pause");

}

//图书批量入库模块，读取csv文件
void ADD_Book_Batch(MYSQL* mysql)
{
	//创建一个结构体book用于存放书本信息
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

	string csvpath; //用于存放csv文件路径

	const char* tablename = "book";
	int rt;    //return value
	int bookcount;	//csv文件中的图书数目
	//结构体指针
	Book* mybook = new Book;


	printf("******图书批量入库模块******\n");
	printf("请您输入要读取的csv文件名(确保与cpp文件在同一目录下)\n");
	cin >> csvpath;

	ifstream inFile(csvpath, ios::in);
	string line;
	if (!inFile)
	{
		printf("当前目录下没有该文件,请重试！\n");
		return;
	}
	else
	{
		printf("******文件打开成功******\n");
		bookcount = 1;
		while (getline(inFile, line))
		{
			cout << "即将处理的图书" << bookcount << ": " << line << endl;
			//将每一行图书的对应信息保存下来

			//二维表结构
			stringstream ss(line);
			string str;
			vector<string> lineArray;
			//按逗号分隔
			while (getline(ss, str, ','))
				lineArray.push_back(str);

			//处理分隔得到的每一个string
			strcpy(mybook->bno, lineArray[0].c_str()); //书号转换
			strcpy(mybook->category, lineArray[1].c_str()); //类别转换
			strcpy(mybook->title, lineArray[2].c_str()); //书名转换
			strcpy(mybook->press, lineArray[3].c_str()); //出版商转换
			mybook->year = atoi(lineArray[4].c_str());   //年份转换
			strcpy(mybook->author, lineArray[5].c_str()); //作者转换
			mybook->price = atof(lineArray[6].c_str());  //价格转换

			//先对primary key的位数做一个判断
			while (strlen(mybook->bno) != 10)
			{
				printf("书号为%s位数错误！请输入正确的书号(10位)：\n", mybook->bno);
				cin >> mybook->bno;
				getchar(); //处理换行符
			}

			//调用图书插入子程序
			rt = Book_Insertion(mysql, tablename, mybook->bno, mybook->category, mybook->title,
				mybook->press, mybook->year, mybook->author, mybook->price);
			if (rt == 1)
			{
				printf("***********书号为%s的图书入库失败! ***********\n", mybook->bno);
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
	MYSQL_RES* res; //一个结果集结构体
	MYSQL_ROW row;  //char** 二维数组，存放一条条记录
	int total;		//库存
	int stock;		//总量

	//插入前先查询数据库是否已有记录
	sprintf(temp, "select * from %s where bno = '%s'", tablename, bno);
	rt = mysql_query(mysql, temp);
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(mysql));
		ERROR_CODE = 1;
		//插入语句出错就返回
		return ERROR_CODE;

	}
	/*else
	{
		printf("%s executed!!!\n", temp);
	}*/

	//获取结果集
	res = mysql_store_result(mysql);
	row = mysql_fetch_row(res);
	if (!row)	//book中没有相同的书号，则添加新书
	{
		total = 1;
		stock = 1;
		//Insert语句
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
		printf("书号为 %s 的书已经成功入库!\n", bno);

	}
	else  //book中已有相同的书号，更新数据库或者增加总量和库存
	{
		int flag = 0;
		//比较类别
		if (strcmp(row[1], category) != 0)
		{
			flag = 1;
			//更新数据
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
			printf("书号为 %s 的书的类别更新成功!\n", bno);
		}

		//比较书名
		if (strcmp(row[2], title) != 0)
		{
			flag = 1;
			//更新数据
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
			printf("书号为 %s 的书的书名更新了!\n", bno);

		}

		//比较出版商
		if (strcmp(row[3], press) != 0)
		{
			flag = 1;
			//更新数据
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
			printf("书号为 %s 的书的出版商更新了!\n", bno);

		}

		//比较年份
		//先将年份转换为int类型
		int oldyear = atoi(row[4]);
		if (year != oldyear)
		{
			flag = 1;
			//更新数据
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
			printf("书号为 %s 的书的发行年份更新了!\n", bno);
		}

		//比较作者
		if (strcmp(row[5], author) != 0)
		{
			flag = 1;
			//更新数据
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
			printf("书号为 %s 的书的作者更新了!\n", bno);
		}

		//比较价格
		//先将价格转换为double类型
		double oldprice = atof(row[6]);
		if (price != oldprice)
		{
			flag = 1;
			//更新数据
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
			printf("书号为 %s 的书的价格更新了!\n", bno);
		}

		//如果上述都属性都一样，那么就增加库存和总藏
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
			printf("书号为 %s 的书的库存和总量增加了!\n", bno);
		}


	}

	//释放结果集
	mysql_free_result(res);

	return ERROR_CODE;

}