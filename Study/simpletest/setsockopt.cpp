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
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h>	// inet_addr
#include <unistd.h>		// close

int main(int argc, char *argv[])
{
	printf("Test Set Sock Opt\n argc:%d\n", argc);

	if (argc < 4)
	{
		printf("Invalid param. cmd ip port TimeoutSecs \n");
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

	unsigned short nport = atoi(argv[2]);
	clientaddr.sin_port = htons(nport);

	// 超时时间.
	int timeoutSecs = atoi(argv[3]);

	struct timeval timeout = {0,0};
	timeout.tv_sec = timeoutSecs;

	int setoptError = setsockopt( tcpLink, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout) );
	if( setoptError == -1 )
    {
        printf("setsockopt SO_SNDTIMEO fail! er: %d\n", errno);
        exit(-1);
    }

	setoptError = setsockopt(tcpLink, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
	if (setoptError == -1)
	{
		printf("setsockopt SO_RCVTIMEO fail! er: %d\n", errno);
		exit(-1);
	}


	int connectError = connect(tcpLink, (sockaddr*)&clientaddr, sizeof(clientaddr));
	if (connectError == -1)
	{
		printf("connect fail! er: %d \n", errno);
        exit(-1);
	//	WSACleanup();
	//	return false;
	}

	close(tcpLink);


	return 1;
}
