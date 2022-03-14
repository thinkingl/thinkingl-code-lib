#include <iostream>
#include <asio.hpp>

using namespace std;

char g_szData[1024] = {0};

void doReceive(asio::ip::udp::socket& s)
{
	s.async_receive( asio::buffer( g_szData, 1024 ), [&s]( asio::error_code ec, size_t len ){
		cout << "recv data ec:[" << ec << "] len:[" << len << "] data:[" << g_szData << "]" << endl;
		if( !ec )
		{
			doReceive( s );
		}
	});
}

void doSend( asio::ip::udp::socket& s )
{
	asio::ip::udp::endpoint sendToEndpoint( asio::ip::make_address_v4( "127.0.0.1" ), 7200 );
	//s.async_send_to( )
}

int main(int argc, char *argv[])
{
	asio::io_context ctx;


	asio::ip::udp::endpoint localEndpoint( asio::ip::udp::v4(), 7200 );

	#if 0
	asio::ip::udp::socket s(ctx);
	asio::error_code ec;
	s.bind( localEndpoint, ec );
	cout << "bind local port ec:" << ec << endl;
	#endif

	asio::ip::udp::socket s(ctx, localEndpoint );

	doReceive(s);

	

	ctx.run();

}