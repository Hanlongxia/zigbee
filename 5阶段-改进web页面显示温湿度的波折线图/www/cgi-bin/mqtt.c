#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <mosquitto.h>
//这头文件里就是咱们libmosquito 这C库提供函数接口

#include <time.h>

int main()
{
    char *data = NULL;
    printf("content-type:text/html\n\n");  
    data = getenv("QUERY_STRING");

    char Buf[100]={0};
    strcpy(Buf,data);

    struct mosquitto *m_hMqtt;//用来存客户端
	
	//初始化lib库函数
	mosquitto_lib_init();//初始化 libmosquit这个库	
	
    // 定义一个客户端名为subtest的发布端。客户端表示订阅端的唯一性
	//如果你要连接一个MQtt服务器，你必须要求当前客户端的名称与已经连在MQTT服务器上的客户端名字必须不一样
    //保证名称的唯一性
	m_hMqtt = mosquitto_new("pcbpfxcgfcub3", true, "data");//申请客户端

	
	//开始连接服务器
	// 第一个参数是客户端
	//第二个参数MQTT服务器的IP  
	//第三是MQTT服务器订阅和发布端口 1883
	//连接时间
	if(MOSQ_ERR_SUCCESS == mosquitto_connect(m_hMqtt, "5.196.95.208", 1883, 20))

    //当前的发布客户端，发布主题temp  内容"hello"
    //以信号质量1 QoS1 方式交互
    mosquitto_publish(m_hMqtt,NULL,"topc2",strlen(Buf)+1,Buf,1,true);
    printf("Buf = %s\n", Buf);

	
	mosquitto_destroy(m_hMqtt);//销毁申请的客户端

    mosquitto_lib_cleanup();//清除这个libmosquit
	return 0;
}