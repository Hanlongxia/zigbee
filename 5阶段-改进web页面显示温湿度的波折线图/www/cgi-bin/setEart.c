#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <string.h>
#include <time.h>

int main()
{
    //不加这句话会报错：[05/Nov/2021:07:34:13 +0000] cgi_header: unable to find LFLF
    printf("content-type:text/html\n\n");  



    char *errmsg = NULL; //用来存储错误信息字符串
    char ret = 0;
    int nRow = 0, nColumn = 0; //nRow 查找出的总行数,nColumn 存储列
    char **dbResult;           //指针数组的地址，其记录了结果集的数据。
                               //内存布局：先依次存放各列的列名，然后是每 一行各列的值。

    char *db_name = "./stuinfo.db";
    sqlite3 *db = NULL;
    char *sql = NULL;

    sqlite3_open(db_name, &db);


    sql = "select time,tep,hum from zigbee3;";
    ret = sqlite3_get_table(db, sql, &dbResult, &nRow, &nColumn, &errmsg);
    if (ret != SQLITE_OK)
    {
        sqlite3_close(db);
        printf("%s\n", errmsg);
        sqlite3_free(errmsg);
        return 0;
    }
    char tep[1024];
    char hum[1024];
    char timess[1024];
    memset(tep, 0, sizeof(tep));
    memset(hum, 0, sizeof(hum));
    memset(timess, 0, sizeof(timess));

    int i = 0;
    //行+1，拿取到最底层那一列的数据
    for (; i < (nRow + 1) * (nColumn + 0); i++)
    {
        // printf("result[%d] = %s\n",nColumn, dbResult[i]);
        char teps[128] = "";
        char hums[128] = "";
        char times[128] = "";
        sprintf(times, "%s,", dbResult[i]);
        strcat(timess, times);
        i++;
        sprintf(teps, "%s,", dbResult[i]);
        strcat(tep, teps);
        i++;
        sprintf(hums, "%s,", dbResult[i]);
        strcat(hum, hums);
    }
    printf("%s|%s|%s", timess, tep, hum);

    // //非回调方式----最后释放一下
    // sqlite3_free_table(db);
    // sqlite3_close(db);
    return 0;
}