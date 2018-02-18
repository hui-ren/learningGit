// socket_client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h" 
//AppWizard建立了文件stdafx.h，该文件包含了当前工程的所有需要MFCinclude 的文件。且这一文件可以随被选择的选项而变化。
// AppWizard 是用类别名称和源程序名产生 Windows 应用程序的一个工作向导，整装了程序并作成向导的程序产生器。AppWizard 将帮助你很快地开始一个新的应用程序。
#include<WinSock2.h> 
#include <WS2tcpip.h> // use inet_pton 
#pragma comment(lib,"ws2_32.lib")

int main()
{
	//1. Initiate
	WSADATA ws; // contains information about the Windows Sockets implementation
	if (0 != WSAStartup(MAKEWORD(2, 2), &ws)) { return 0; } // check whether we could find a usable winsock dll 
	/*
	Windows和需要执行TCP/IP网络通信的应用程序会调用动态链接库ws2_32.dll。
如果ws2_32.dll不可用,你的计算机连接网络的操作会不稳定(即使不连接到外部网络)。*/
	if (2 != LOBYTE(ws.wVersion) || 2 != HIBYTE(ws.wVersion))
	{
		WSACleanup(); // terminates use of the Winsock 2 DLL (Ws2_32.dll)
		return 0;
	}// check the version

	//2. establish the socket
	SOCKET hclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	/*The addressFamily parameter
	specifies the addressing scheme that theSocket class uses,
	the socketType parameter specifies the type of the Socket class,
	and the protocolType parameter specifies the protocol used by Socket.*/
	if (hclient == INVALID_SOCKET) { return 0; } //check

	//3. prepare the  addr struct 
	SOCKADDR_IN addr{ 0 }; //initiate addr 
	addr.sin_family = AF_INET;
	addr.sin_port = htons(short(4999)); //converts a u_short from host to TCP/IP network byte order (which is big-endian)
	//坑在这里！
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //inet_addr : The inet_addr function converts a string containing an IPv4 dotted-decimal address into a proper address for the IN_ADDR structure. The in_addr structure represents an IPv4 Internet address.

    //inet_pton(AF_INET,"127.0.0.1",(void*)&addr);
	// 点分十进制转换成二进制整数

	//4. get linked to the serve
	if (SOCKET_ERROR == connect(hclient, (sockaddr*)&addr, sizeof(addr)))
	{
		printf("fail! \n");
		closesocket(hclient);
		return 0;
	}
	while (true) 
	{
		//5.send && receive
		char buf[128]{0};
		std::cout << "向服务端发送数据：";
		std::cin.getline(buf,128);
		int res = send(hclient, buf, strlen(buf), 0);
		if (res > 0) { printf("发送的消息是：%s\n", buf); }
		/*If no error occurs, send returns the total number of bytes sent ;  Otherwise, a value of SOCKET_ERROR (-1) is returned, and a specific error code can be retrieved by calling WSAGetLastError.
		*/
		char buf_recv[128]{ 0 };
		int rec = recv(hclient, buf_recv, 128, 0);
		if (rec > 0) { printf("接受的消息是：%s\n", buf_recv); }
		/*If no error occurs, recv returns the number of bytes received; Otherwise, a value of SOCKET_ERROR is returned, and a specific error code can be retrieved by calling WSAGetLastError.
		*/
	}

	//6. close the socket
	closesocket(hclient);
	WSACleanup();

	return 0;
}

