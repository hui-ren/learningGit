// socket_client.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h" 
//AppWizard�������ļ�stdafx.h�����ļ������˵�ǰ���̵�������ҪMFCinclude ���ļ�������һ�ļ������汻ѡ���ѡ����仯��
// AppWizard ����������ƺ�Դ���������� Windows Ӧ�ó����һ�������򵼣���װ�˳��������򵼵ĳ����������AppWizard ��������ܿ�ؿ�ʼһ���µ�Ӧ�ó���
#include<WinSock2.h> 
#include <WS2tcpip.h> // use inet_pton 
#pragma comment(lib,"ws2_32.lib")

int main()
{
	//1. Initiate
	WSADATA ws; // contains information about the Windows Sockets implementation
	if (0 != WSAStartup(MAKEWORD(2, 2), &ws)) { return 0; } // check whether we could find a usable winsock dll 
	/*
	Windows����Ҫִ��TCP/IP����ͨ�ŵ�Ӧ�ó������ö�̬���ӿ�ws2_32.dll��
���ws2_32.dll������,��ļ������������Ĳ����᲻�ȶ�(��ʹ�����ӵ��ⲿ����)��*/
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
	//�������
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //inet_addr : The inet_addr function converts a string containing an IPv4 dotted-decimal address into a proper address for the IN_ADDR structure. The in_addr structure represents an IPv4 Internet address.

    //inet_pton(AF_INET,"127.0.0.1",(void*)&addr);
	// ���ʮ����ת���ɶ���������

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
		std::cout << "�����˷������ݣ�";
		std::cin.getline(buf,128);
		int res = send(hclient, buf, strlen(buf), 0);
		if (res > 0) { printf("���͵���Ϣ�ǣ�%s\n", buf); }
		/*If no error occurs, send returns the total number of bytes sent ;  Otherwise, a value of SOCKET_ERROR (-1) is returned, and a specific error code can be retrieved by calling WSAGetLastError.
		*/
		char buf_recv[128]{ 0 };
		int rec = recv(hclient, buf_recv, 128, 0);
		if (rec > 0) { printf("���ܵ���Ϣ�ǣ�%s\n", buf_recv); }
		/*If no error occurs, recv returns the number of bytes received; Otherwise, a value of SOCKET_ERROR is returned, and a specific error code can be retrieved by calling WSAGetLastError.
		*/
	}

	//6. close the socket
	closesocket(hclient);
	WSACleanup();

	return 0;
}

