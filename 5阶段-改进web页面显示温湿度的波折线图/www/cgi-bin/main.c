#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <mosquitto.h>//这头文件里就是咱们libmosquito 这C库提供函数接口
#include "sqlite3.h"

int my_sqlite3_callback(void* parg ,int columns, char**column_value, char**column_name)
{
    // static int i = 0;
    // printf("这是回调函数，第%d次调用\n", i++);

    int num = 0;
    //每次回调，都会检索当前行有几列数据
    //然后for循环拿出
    for(; num < columns; num++)
    {
        //通过.js中的程序创建的web服务器连接，这里可以之间打印到页面上指定的位置
        printf("%s=%s",column_name[num], column_value[num]);
    }

    return 0;
}

int main()
{

    //注意printf，一定不可以有鸡肋的存在
    //这里是必须的！！与网页建立连接
    printf("content-type:text/html\n\n");
    char *db_name = "stuinfo.db";
    sqlite3 *db_hander = NULL;


    sqlite3_open(db_name, &db_hander);


    char *errmsg = NULL;
    int arg = 98;
    //执行 sql 指向的 SQL 语句，若结果集不为空，函数会调用函数指针 callback 所指向的函数
    //调用my_sqlite3_callback回调函数，查询此前插入的内容
    //内容有几行，调用几次
    sqlite3_exec(db_hander, "select max(time),len,data, addr, tep,hum from zigbee3;",\
                my_sqlite3_callback, &arg, &errmsg);    


    sqlite3_close(db_hander);

    return 0;
}
