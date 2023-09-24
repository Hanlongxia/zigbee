#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<string.h>
#define ip_config_name "/mnt/hgfs/ios2021/web_boa-0.94.13/www/cgi-bin/ip_config"

int main()
{
    printf("content-type:text/html\n\n");

    FILE *fp=fopen("./ip_config","r+");
    if(fp==NULL)
    {
        printf("文件不存在\n");
        return 0;
    }

    int count = 0;
    int ch;
    
  
    while((ch = fgetc(fp)) != EOF)
    {
        //碰到'\n'表示一行结束，开始下一行
        if(ch == '\n')
        {
            count++;
        }
    }
    //printf("行号统计count = %d\n", count); //输出查看


    char *str = "time=36258 | len=40 | data=et | addr=0x2D4A | tep=28 | hum=73";
    //printf("单独一行的字符串长度 strlen(str) = %d\n", strlen(str));
    int num = strlen(str) + 1;
    char buf[62] = "";
    int i = 62;
    
    fseek(fp, 0, SEEK_END);//定位到文件末尾        
    int j = 0;

    while(num--)
    {
        i--;
        j = j - 1;
        buf[i] = fgetc(fp);//获得当前的字符给数组
        fseek(fp, j, SEEK_END); //不停的向左（前）移动一位
    }
    buf[strlen(str)] = '\0';
    printf("%s\n", buf);

    fclose(fp);

    return 0;
}