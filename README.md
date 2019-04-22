# SimpleBookDBMS
A Simple Book Management Program
# 编译环境
本程序运行的环境为： Visual Studio 2017 x64 Debug模式

使用的数据库系统为：MySQL 8.0

编译运行的流程：


1.  使用VS打开.sln项目文件
2.  右键单击项目名称，选择属性，项目名->属性->c/c+±>常规->附加包含目录 添加路径：C:\\Program Files\\MySQL\\MySQL Server 8.0\\include （这个是默认路径，选择自己安装的路径即可）
3.  链接器->常规->附加库目录 添加路径：C:\\Program Files\\MySQL\\MySQL Server 8.0\\lib
（这个是默认路径，选择自己安装的路径即可）
4.  链接器->输入->附加依赖项 添加：libmysql.lib 然后将C:\\Program Files\\MySQL\\MySQL Server 8.0\\lib下的libmysql.lib拷贝至项目x64目录下的debug目录

# 功能模块
1. 图书入库
2. 图书查询
3. 借书
4. 还书
5. 借书证管理
