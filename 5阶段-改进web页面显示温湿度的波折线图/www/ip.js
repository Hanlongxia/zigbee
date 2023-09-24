//AJAX的通信模板
//================AJAX 开始内容==============================
//仅仅是一个Javascript 和XML 融合一个 专门用于 网页和web服务器进行异步通信的一个 对象
//说白就是一个 对象（属性 方法）
function getXMLHttpRequest()//这个函数兼容不同浏览器 去实例化 AJAX的对象
{
    var xmlhttp = null;
    if (window.XMLHttpRequest)//自动检测当前浏览器的版本，如果是 IE5.0 以上的高版本的浏览器
    {// code for IE7+, Firefox, Chrome, Opera, Safari
        xmlhttp=new XMLHttpRequest();//创建请求对象
    }
    else////如果浏览器是底版本的
    {// code for IE6, IE5
        xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");//创建请求对象
    }
    return xmlhttp;//返回请求对象
}


//================AJAX 结束内容==============================
function btpress_temperature()
{
      var myxmlhttp=null;
      myxmlhttp=getXMLHttpRequest();


     //相当于C++  信号和槽函数 connect
      myxmlhttp.onreadystatechange=function()
      {//如果发现AJAX异步通信对象里面的一个属性，状态属性发生改变这个事情一旦发生，那么就来调用这个函数

          if(myxmlhttp.readyState==4 && myxmlhttp.status==200)
          {//进入到这函数内部可能有很多种，咱们只处理，发送数据包成功，服务器完成，返回结果这个事情发生
            //document.getElementById("idfile").innerHTML=myxmlhttp.responseText; 
            document.getElementById("1").value=myxmlhttp.responseText; 
            var mystr = myxmlhttp.responseText;
            if(mystr[28] == 'c')
            {     
              var time=mystr[10] + mystr[11]+ mystr[12] +mystr[13] +mystr[14] +mystr[15] +mystr[16]; 
              var hour =Math.floor(time/3600);
              var min=Math.floor((time-hour*3600)/60);
              var sec=time-hour*3600-min*60;
              document.getElementById("iddate_time").value=hour+":"+min+":"+sec;
              document.getElementById("iddate_data_package").value=mystr[21] + mystr[22]; 
              var str_rt = "协调器"
              document.getElementById("iddate_equipment").value=str_rt; 
              document.getElementById("iddate_addr").value=mystr[35] + mystr[36]+ mystr[37] +mystr[38] +mystr[39]+mystr[40]; 
              document.getElementById("iddate_temperature").value=mystr[45] + mystr[46]; 
              document.getElementById("iddate_humidity").value=mystr[51] + mystr[52]; 
            }
            if(mystr[28] == 'r')
            {     
              var time=mystr[10] + mystr[11]+ mystr[12] +mystr[13] +mystr[14] +mystr[15] +mystr[16]; 
              var hour =Math.floor(time/3600);
              var min=Math.floor((time-hour*3600)/60);
              var sec=time-hour*3600-min*60;
              document.getElementById("iddate_time").value=hour+":"+min+":"+sec;
              //document.getElementById("iddate_time").value=mystr[10] + mystr[11]+ mystr[12] +mystr[13] +mystr[14] +mystr[15] +mystr[16]; 
              document.getElementById("iddate_data_package").value=mystr[21] + mystr[22]; 
              var str_rt = "路由器"
              document.getElementById("iddate_equipment").value=str_rt; 
              document.getElementById("iddate_addr").value=mystr[35] + mystr[36]+ mystr[37] +mystr[38] +mystr[39]+mystr[40]; 
              document.getElementById("iddate_temperature").value=mystr[45] + mystr[46]; 
              document.getElementById("iddate_humidity").value=mystr[51] + mystr[52]; 
            }
            if(mystr[28] == 'e')
            { 
              var time=mystr[10] + mystr[11]+ mystr[12] +mystr[13] +mystr[14] +mystr[15] +mystr[16]; 
              var hour =Math.floor(time/3600);
              var min=Math.floor((time-hour*3600)/60);
              var sec=time-hour*3600-min*60; 
              document.getElementById("iddate_time").value=hour+":"+min+":"+sec;
              //document.getElementById("iddate_time").value=mystr[10] + mystr[11]+ mystr[12] +mystr[13] +mystr[14] +mystr[15] +mystr[16]; 
              document.getElementById("iddate_data_package").value=mystr[21] + mystr[22]; 
              var str_rt = "终端"
              document.getElementById("iddate_equipment").value=str_rt; 
              document.getElementById("iddate_addr").value=mystr[35] + mystr[36]+ mystr[37] +mystr[38] +mystr[39]+mystr[40]; 
              document.getElementById("iddate_temperature").value=mystr[45] + mystr[46]; 
              document.getElementById("iddate_humidity").value=mystr[51] + mystr[52]; 
            }
              
          }

      }

    //Get方式
    var url="./cgi-bin/main.cgi?";

    myxmlhttp.open("GET",url,true);
    myxmlhttp.setRequestHeader("If-Modified-Since","0");
    myxmlhttp.send();
}


function btpress_led()
{
  //指定某一个下拉框
  var myselect=document.getElementById("equipment");//获取select的id
  var index_equipment=myselect.selectedIndex; //获取此时下拉框option的index索引
  var value_equipment=myselect.options[index_equipment].value; //获取此时下拉框option的index索引对应的value值
  var text_equipment=myselect.options[index_equipment].text;//获取此时下拉框option的index索引对应的文本

  var myselect=document.getElementById("led");//获取 select的id
  var index_led=myselect.selectedIndex; 
  var value_led=myselect.options[index_led].value; 
  var text_led=myselect.options[index_led].text;

  var myselect=document.getElementById("higeht");//获取 select的id
  var index_higeht=myselect.selectedIndex; 
  var value_higeht=myselect.options[index_higeht].value; 
  var text_higeht=myselect.options[index_higeht].text;


  //value_equipment + "," +value_led + "," + value_higeht;
  //仅显示value的整数int值
  //document.getElementById("iddate2").value=value_equipment + "," +value_led + "," + value_higeht;
  //仅显示text文本
  document.getElementById("iddate3").value=text_equipment + "--" +text_led + "--" + text_higeht;
  
  
  var myxmlhttp=null;
  myxmlhttp=getXMLHttpRequest();


   
  var  midstr="./cgi-bin/mqtt.cgi"; //从文本框拿到就是字符串
   
 //相当于C++  信号和槽函数 connect
  myxmlhttp.onreadystatechange=function()
  {//如果发现AJAX异步通信对象里面的一个属性，状态属性发生改变这个事情一旦发生，那么就来调用这个函数

       if(myxmlhttp.readyState==4 && myxmlhttp.status==200)
       {//进入到这函数内部可能有很多种，咱们只处理，发送数据包成功，服务器完成，返回结果这个事情发生
        //myxmlhttp.responseText= value_equipment + "," +value_led + "," + value_higeht;
          document.getElementById("iddate2").value=myxmlhttp.responseText;   
       }

  }
  
  //Get方式
  var url="./cgi-bin/mqtt.cgi?" + "CTR=" + value_equipment + "," +value_led + "," + value_higeht;

  myxmlhttp.open("GET",url,true);
  myxmlhttp.setRequestHeader("If-Modified-Since","0");
  myxmlhttp.send();

}

/*
第一个参数：code 	必需。要调用的函数或要执行的代码串。
第二个参数：millisec 	必须。周期性执行或调用 code 之间的时间间隔，以毫秒计。
setInterval() 方法可按照指定的周期（以毫秒计）来调用函数或计算表达式
setInterval() 方法会不停地调用函数，直到 clearInterval() 被调用或窗口被关闭
*/
setInterval(function()
{
  var myxmlhttp=null;
  myxmlhttp=getXMLHttpRequest();

   
 //相当于C++  信号和槽函数 connect
  myxmlhttp.onreadystatechange=function()
  {//如果发现AJAX异步通信对象里面的一个属性，状态属性发生改变这个事情一旦发生，那么就来调用这个函数

       if(myxmlhttp.readyState==4 && myxmlhttp.status==200)
       {//进入到这函数内部可能有很多种，咱们只处理，发送数据包成功，服务器完成，返回结果这个事情发生
        //myxmlhttp.responseText= value_equipment + "," +value_led + "," + value_higeht;
          document.getElementById("iddate_condition").value=myxmlhttp.responseText;   
       }

  }
  
  //Get方式
  var url="./cgi-bin/text.cgi?";

  myxmlhttp.open("GET",url,true);
  myxmlhttp.setRequestHeader("If-Modified-Since","0");
  myxmlhttp.send();

  
},1000);


$(document).ready(function ()
{
    // 基于准备好的dom，初始化echarts实例
    var myChart = echarts.init(document.getElementById('ECharts'));
    var humChart = echarts.init(document.getElementById('humECharts'));
    //点击刷新按钮，触发函数
    $("#qwsure").click(function () 
    {
        $.get("./cgi-bin/setEart.cgi", {
            name: "状况显示图",
            url: "asdasd",
            data: "来源数据库"
        },function (data2) {

              //使用'|'将三个数据时间、温度、湿度进行大范围切割
              var data = data2.split("|");
              //切割到的三部分数据分别存放
              // alert(data[0]);
              // alert(data[1]);
              // alert(data[2]);

              // 指定图表的配置项和数据
              var option = {
                title: {
                  text: '第一个 ECharts 实例'
                },  
                /*配置提示信息*/
                tooltip: {},

                //需要和series中的name一致
                legend: {
                  data:['温度']
                },
                /*xAxis：配置要在 X 轴显示的项*/
                xAxis: {
                  //时间
                  data: data[0].split(",")
                },

                /*
                yAxis：配置要在 Y 轴显示的项
                系列列表
                每个系列通过 type 决定自己的图表类型
                type: 'line'：折线/面积图*/
                yAxis: {},
                    series: [{
                        name: '温度',
                        type: 'line',
                        //拿到','分割后的数据
                        data: data[1].split(",")
                    }]
              };

              var humoption = {
                title: {
                  text: '第二个 ECharts 实例'
                },  
                tooltip: {},

                legend: {
                  data:['湿度']
                },
                xAxis: {
                  data: data[0].split(",")
                },

                yAxis: {},
                    series: [{
                        name: '湿度',
                        type: 'line',
                        data: data[2].split(",")
                    }]
              };  


              //使用刚指定的配置项和数据显示图表。  
              myChart.setOption(option);
              humChart.setOption(humoption);
        });
    });
    
    
});


