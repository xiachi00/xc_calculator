#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

using namespace std;

#define IP "10.0.4.5"//这个一定要用自己服务器的IP
// #define port 8080 //监听端口，可以在范围内自由设定

string message ="";
void init(const char * file_name) {
    message="HTTP/1.1 200 OK\r\n";                                    //响应行
    message+="Content-Type:text/html\r\n";                             //响应头
    message+="\r\n";                                                   //空行
    ifstream ifile;
    ifile.open(file_name, ios::in);
    string jh;
    while(getline(ifile,jh))
    {
        message += jh + "\n";
    }
    ifile.close();
    message += "\r\n";
    cout<<"DEBUG:" << message << endl;
}


int main(int argv, char * argc[])
{
    if (argv < 2) {
        cout << "Usage: " << argc[0] << " <html>" << " <port>" << endl;
        return -1;
    }
    char * file_name = argc[1];
    int port = atoi(argc[2]);
	//1.创建一个socket套接字
	int local_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (local_fd == -1)
	{
		cout << "socket error!" << endl;
		exit(-1);
	}
	cout << "socket ready!" << endl;

	//2.sockaddr_in结构体：可以存储一套网络地址（包括IP与端口）,此处存储本机IP地址与本地的一个端口
	struct sockaddr_in local_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(port);  //绑定6666端口
	local_addr.sin_addr.s_addr = inet_addr(IP); //绑定本机IP地址

	//3.bind()： 将一个网络地址与一个套接字绑定，此处将本地地址绑定到一个套接字上
	int res = bind(local_fd, (struct sockaddr *)&local_addr, sizeof(local_addr));
	if (res == -1)
	{
		cout << "bind error!" << endl;
		exit(-1);
	}
	cout << "bind ready!" << endl;

	//4.listen()函数：监听试图连接本机的客户端
	//参数二：监听的进程数
	listen(local_fd, 10);
	cout << "等待来自客户端的连接...." << endl;

	while (true)//循环接收客户端的请求
	{
        init(file_name);
		//5.创建一个sockaddr_in结构体，用来存储客户机的地址
		struct sockaddr_in client_addr;
		socklen_t len = sizeof(client_addr);
		//6.accept()函数：阻塞运行，直到收到某一客户机的连接请求，并返回客户机的描述符
		int client_fd = accept(local_fd, (struct sockaddr *)&client_addr, &len);
		if (client_fd == -1)
		{
			cout << "accept错误\n"
				 << endl;
			exit(-1);
		}

		//7.输出客户机的信息
		char *ip = inet_ntoa(client_addr.sin_addr);
		cout << "客户机： " << ip << " 连接到本服务器成功!" << endl;

		//8.输出客户机请求的信息
		char buff[1024] = {0};
		int size = read(client_fd, buff, sizeof(buff));
		cout << "Request information:\n"
			 << buff << endl;
		cout << size << " bytes" << endl;
		
		//9.使用第6步accept()返回socket描述符，即客户机的描述符，进行通信。
		write(client_fd, message.c_str(), message.size());//返回message

		//10.关闭sockfd
		close(client_fd);
	}
	close(local_fd);
	return 0;
}

