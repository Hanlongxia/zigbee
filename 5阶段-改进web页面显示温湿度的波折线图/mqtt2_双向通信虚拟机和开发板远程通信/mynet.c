#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <mosquitto.h>
//这头文件里就是咱们libmosquito 这C库提供函数接口

#include <time.h>
#include "sqlite3.h"
#define ip_config_name "/mnt/hgfs/ios2021/web_boa-0.94.13/www/cgi-bin/ip_config"
char *topic1 = "topc1";
char *topic2 = "topc2";

//数据库的回调函数	
int my_sqlite3_callback(void* parg ,int columns, char**column_value, char**column_name)
{
    static int i = 0;
    printf("这是回调函数，第%d次调用\n", i++);

    int num = 0;
    //每次回调，都会检索当前行有几列数据
    //然后for循环拿出
    for(; num < columns; num++)
    {
        printf("第%d列 %s == %s\n", num, column_name[num], column_value[num]);
    }

	putchar('\n');
    return 0;
}

// MQTT的消息回调
void my_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
	char len[16] = "";
	char data[16] = "";
	char addr[16] = "";
	char tep[16] = "";
	char hum[16] = "";
	char text_buf[1024] = "";
	time_t t;
	struct tm *lt;
	time(&t);
	lt =  localtime(&t);//每次接收者只要拿到，就把当前时间，打印出来

	printf("%d.%d.%d %d:%d:%d ",lt->tm_year+1900,lt->tm_mon,lt->tm_mday,\
			lt->tm_hour,lt->tm_min,lt->tm_sec);

	//我这里数据库中对字符串的解析不准确，固，全部转换成int类型值，换算成秒，加起来
	//下面select数据库，就按照int类型值去排列，拿取最大时间的那条数据
	int time_count = lt->tm_sec + (lt->tm_min)*60 + ((lt->tm_hour) *60 )*60;
	printf("time_count :%d\n",time_count);


	//消息的主题是什么msg->topic，消息的内容是什么msg->payload
	printf("%d:topic(%s)->%s\n",__LINE__,(char *)msg->topic,(char *)msg->payload);

	//要插入到数据库中的内容，进行拆解	
	sscanf((char *)msg->payload, "%*[^=]=%[^,],data=%[^,],addr=%[^,],\
			tep=%[^,],hum=%s", len, data, addr, tep, hum);


	FILE *ip_config = NULL;
	ip_config = fopen(ip_config_name, "at+");
	if(ip_config == NULL){
		perror("!!!configure file,in main.c");
		_exit(1);
	}
	else
	{
		//以一种固定的格式写入到文件
		sprintf(text_buf, "time=%d | len=%s | data=%s | addr=%s | tep=%s | hum=%s",\
			time_count, len, data, addr, tep, hum);

		printf("text_buf = %s\n",text_buf);
		int len = strlen(text_buf);
		printf("len = %d\n",len);
		//fprintf(ip_config, "%s\n", text_buf);
		text_buf[len]='\n';//-------方法① 
		text_buf[len+1]='\0';      
		fwrite(text_buf,len+1,1,ip_config);
	}

	
	//==================【数据库操作】==============//
	char *db_name = "/mnt/hgfs/ios2021/web_boa-0.94.13/www/cgi-bin/stuinfo.db";
    sqlite3 *db_hander = NULL;

	if(sqlite3_open(db_name, &db_hander) !=  SQLITE_OK)
    {
        printf("数据库打开失败\n");
        return -1;
    }
	char *errmsg = NULL;
	
	//15:22:1 50 ct 0x2D2A 68 74--裁剪出来的样式，正确
	//printf("裁剪：%s %s %s %s %s %s\n",time, len, data, addr, tep, hum);		
	//2021.9.30 14:40:18 23:topic(topc1)->len=50,data=ct,addr=0x2D2A,tep=68,hum=74	
	//puts("==============插入内容提示================");

	//创建数据库，以及标头
    SQLITE_OK != sqlite3_exec(db_hander, "create table zigbee3\
		(time real, len text, data text, addr text, tep text, hum text);",\
                NULL, NULL, &errmsg);


	//插入数据库类型，按照字符串拼接的方式插入
	char makesql[64]=" insert into zigbee3 values(";
	char sql[128]="";
	sprintf(sql,"%s'%d','%s','%s','%s','%s','%s')",makesql,time_count, len, data, addr, tep, hum);
	if(sqlite3_exec(db_hander,sql, NULL, NULL, &errmsg))//对插入操作进行拆解，拼接   
    {
        printf("插入创建失败！ %s\n", errmsg);
        errmsg = NULL;
    }
	else
	{
		printf("插入c成功:%s\n", sql);
	}


	//数据库中的内容按照降序排序
    sqlite3_exec(db_hander," SELECT * FROM zigbee3 ORDER BY time_count DESC;",NULL,NULL,&errmsg);  

	int arg = 98;
    //执行 sql 指向的 SQL 语句，若结果集不为空，函数会调用函数指针 callback 所指向的函数
    //调用my_sqlite3_callback回调函数，查询此前插入的内容
    //内容有几行，调用几次
	//max(time),len,data, addr, tep,hum
	// max(time)：指定只显示最大时间的数据
    sqlite3_exec(db_hander, "select max(time),len,data, addr, tep,hum from zigbee3;",\
                my_sqlite3_callback, &arg, &errmsg); 

	if(sqlite3_close(db_hander) !=  SQLITE_OK)
    {
        printf("数据库关闭失败\n");
        return -1;
    }    


	fclose(ip_config);


}

// 订阅消息回调--辅助判断当前MQTT入网没
void my_subscribe_callback(struct mosquitto *mosq, void *obj, int mid,int qos_count,const int *granted_qos)
{
	int i;
	time_t t;
	struct tm *lt;
	time(&t);
	lt =  localtime(&t);
	printf("%d: %s\n",__LINE__,(char *)obj);
	printf("%d: mid=%d\n",__LINE__,mid);
	printf("%d: qos_count=%d\n",__LINE__,qos_count);
	for(i=0;i<qos_count;i++)
		printf("%d: granted_qos[%d]=%d\n",__LINE__,i,granted_qos[i]);
}


void  *mypcbptr(void *arg)
{
	struct mosquitto *m_hMqtt1;
	m_hMqtt1 = mosquitto_new("pcbsu22222b3", true, "data");//申请客户端---主题不能一样，否者接不到也发不出去数据
	
	//注册订阅 和消息回调	
	//当订阅者收到来自服务器的订阅主题发布的消息的时候，那么就会来调用my_message_callback
    mosquitto_message_callback_set(m_hMqtt1, my_message_callback);//设置一个消息回调函数

	//订阅者订阅时候的回调函数
	mosquitto_subscribe_callback_set(m_hMqtt1, my_subscribe_callback);
	
	
	
	if(MOSQ_ERR_SUCCESS == mosquitto_connect(m_hMqtt1, "5.196.95.208", 1883, 20))
		puts("pcbsub连接服务器成功");
	else
		puts("pcbsub连接服务器失败");	
	
	mosquitto_subscribe(m_hMqtt1,NULL,"topc1",1);//这里订阅一个主题topc1--另一个MQTT服务器对主题topc1发布内容

    mosquitto_loop_start(m_hMqtt1);//这个是一个阻塞函数，只有当ctr+c才会退出
	//在这个阻塞函数，不断去判断当前订阅主题有没有来消息
	
	/* 阻塞等待 */
	mosquitto_loop_stop(m_hMqtt1, false);
	
	mosquitto_destroy(m_hMqtt1);
	
	
	 return NULL;
}

//执行这个可执行程序相当于
//手动指令测试用： mosquitto_pub  -t temp -m "hello"  -h 5.196.95.208 （主题、内容、联网MQTT服务器）
int main(int argc,char *argv[])
{
	struct mosquitto *m_hMqtt;//用来存客户端

	//----主进程中先看文件存在吗，不存在创建一下，就关掉
	//--订阅线程中在对文件追加内容
	FILE *ip_config = NULL;
    ip_config = fopen(ip_config_name,"w+");
	if(ip_config == NULL){
		perror("!!!configure file,in main.c");
		_exit(1);
	}	
	fclose(ip_config);

	char *content = "hello world";
	
	//初始化lib库函数
	mosquitto_lib_init();//初始化 libmosquit这个库
	
	
	//创建线程
	 pthread_t pcbsubptr=0;
	 pthread_create(&pcbsubptr,NULL,mypcbptr,NULL);
	 pthread_detach(pcbsubptr);
	 
	 
	
	
	
	
    // 定义一个客户端名为subtest的发布端。客户端表示订阅端的唯一性
	//如果你要连接一个MQtt服务器，你必须要求当前客户端的名称与已经连在MQTT服务器上的客户端名字必须不一样
    //保证名称的唯一性
	m_hMqtt = mosquitto_new("pcbpfxcgfcub3", true, "data");//申请客户端---主题不能一样，否者接不到也发不出去数据


	
	//开始连接服务器
	//第一个参数是客户端
	//第二个参数MQTT服务器的IP  
	//第三是MQTT服务器订阅和发布端口 1883
	//连接时间
	if(MOSQ_ERR_SUCCESS == mosquitto_connect(m_hMqtt, "5.196.95.208", 1883, 20))
		puts("pcbpub连接服务器成功");
	else
		puts("pcbpub连接服务器失败");
	
	 char buff[100]={0};
	 while(1)
	 {
		memset(buff, 0, sizeof(buff));
		fgets(buff,sizeof(buff), stdin);//从屏幕终端去拿发布内容,当没有输入会阻塞

		//buff存是什么  XXXX   XXXX\n
		buff[strlen(buff)-1]=0;//'\0'  NULL 0

		//当前的发布客户端，发布主题topc2 内容buff
		//以信号质量1 QoS1 方式交互
		mosquitto_publish(m_hMqtt,NULL,"topc2",strlen(buff)+1,buff,1,true);

		printf("当前pcbpub发布一个消息!!\n");

		 
	 }

	
	mosquitto_destroy(m_hMqtt);//销毁申请的客户端

    mosquitto_lib_cleanup();//清除这个libmosquit

	

	return 0;
}
