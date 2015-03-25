/**
*	验证Socket编程的socket属性设置.
*	只支持linux/gcc
*	+by lizhixing@2015-03-25
*/
#include <stdlib.h>	// exit

#include <stdio.h>	// printf
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>	// socket..
#include <errno.h> // errno

int main(int argc, char *argv[])
{
	printf("Test Set Sock Opt\n argc:%d\n", argc);

	if (argc < 2)
	{
		printf("Invalid param. cmd ip port\n");
		exit(-1);
		return -1;
	}

	for (int i = 0; i < argc; ++i)
	{
		printf("argv[%d]:%s\n", i, argv[i]);
	}

	int tcpLink = socket(AF_INET, SOCK_STREAM, 0);
	if (tcpLink == -1)
	{
		printf("socket fail! lasterror:%d\n", errno);
		exit(-1);
		return -1;
	}

	sockaddr_in clientaddr;
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_addr.s_addr = inet_addr( argv[1] );
	
	//clientaddr.sin_port = htons(nport);

	//int connectError = connect(tcpLink, (SOCKADDR*)&clientaddr, sizeof(clientaddr));
	//if (connectError == SOCKET_ERROR)
	//{
	//	int lastError = CU::GetLastError();
	//	LOGERR() << L"连接服务器失败 er =" << connectError
	//		<< L",WSAGetLastError=" << lastError
	//		<< L",serverIp=" << serverIp
	//		<< L",serverPort=" << serverPort;
	//	closesocket(tcpLink);

	//	WSACleanup();
	//	return false;
	//}

	return 1;
}