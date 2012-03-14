#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>


#ifdef WIN32
// link with Ws2_32.lib
#pragma comment(lib,"Ws2_32.lib")
#include <winsock2.h>
#else

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#endif

bool TryBindPort( int bindPort )
{
	int sockfd,new_fd;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	int sin_size;
	//建立TCP套接口
	if((sockfd = socket(AF_INET, SOCK_DGRAM,0))==-1)
	{
		printf("create socket error");
		perror("create socket error");
		
		return false;
	}
	//初始化结构体，并绑定端口
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(bindPort);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	//	bzero(&(my_addr.sin_zero),8);
	//绑定套接口
	if(bind(sockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr))==-1)
	{
		return false;
	}
	else
	{
		return true;
	}
}

int main( int argc, char **argv )
{
	if ( argc != 2 || argv[1] == 0 )
	{
		printf( "error command param!" );
		exit( 1 );
	}

#ifdef WIN32
	// Declare and initialize variables
	WSADATA wsaData = {0};
	int iResult = 0;
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		wprintf(L"WSAStartup failed: %d\n", iResult);
		return 1;
	}
#endif

	int bindPort = atoi( argv[1] );
	printf( "Will try to bind to port %d \n", bindPort );

	bool bindSuccess = TryBindPort( bindPort );
	if ( !bindSuccess )
	{
		printf( "Bind the port %d fail!!!!!!\n", bindPort );
		exit(0 );
	}
	else
	{
		printf( "Bind the port %d success!!!\n", bindPort );
	}

	while( true )
	{
		printf( "Type 'q' to exit\n" );
		int input = getchar();
		if ( 'q' == input )
		{
			break;
		}
	}
}