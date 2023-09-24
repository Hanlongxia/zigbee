OBJ += main.o #编译文件
OBJ += sqlite3.o
OBJ1 += mqtt.o
OBJ2 += text.o
OBJ3 += setEart.o
OBJ3 += sqlite3.o
FLAGS = -Wall

#编译方式
CC = gcc 

#$(FLAGS) ：编译文件需要到的环境变量
FLAGS += -I/home/edu/mqtt_pc/mqtt_install/home/edu/share/mqtt_pc/mqtt_install/include
FLAGS += -L/home/edu/mqtt_pc/mqtt_install/home/edu/share/mqtt_pc/mqtt_install/lib
FLAGS += -L/home/edu/mqtt_pc/ssl_install/lib

main:$(OBJ) $(OBJ1) $(OBJ2) $(OBJ3)  #两个不同的cgi文件，需要不同的连接CC去编译
	$(CC) $(OBJ)  -o main.cgi  $(FLAGS)  -lpthread -ldl -lmosquitto -lrt
	$(CC) $(OBJ1) -o mqtt.cgi  $(FLAGS)  -ldl -lmosquitto -lrt
	$(CC) $(OBJ2) -o text.cgi  $(FLAGS)  -lrt
	$(CC) $(OBJ3) -o setEart.cgi   $(FLAGS)  -lpthread   -lrt  -ldl
%.o:%.c
	$(CC) -c $^ -o $@ $(FLAGS)
.PHONY:clean
clean:
	rm example *.o -rfv	
