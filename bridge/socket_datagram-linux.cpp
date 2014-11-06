#ifdef __WAND__
target[name[socket_datagram.o] type[object] platform[;GNU/Linux]]
#endif

#include "socket_datagram.h"
#include "../exception_missing.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#include <cstring>

class SyZmO::SocketDatagram::Socket
	{
	public:
		Socket()
			{
			sock=socket(AF_INET,SOCK_DGRAM,0);
			if(sock==-1)
				{throw SyZmO::ExceptionMissing(__FILE__,__LINE__);}
			}

		void bind(uint16_t port)
			{
			sockaddr_in service;
			memset(&service,0,sizeof(sockaddr_in));

			service.sin_family=AF_INET;
			service.sin_addr.s_addr=htonl(INADDR_ANY);
			service.sin_port=htons(port);
			if(::bind(sock,(sockaddr*)&service,sizeof(service))==-1)
				{throw SyZmO::ExceptionMissing(__FILE__,__LINE__);}
			}

		void bind(uint16_t port,const char* address)
			{
			sockaddr_in service;
			memset(&service,0,sizeof(sockaddr_in));

			service.sin_family=AF_INET;
			service.sin_addr.s_addr=inet_addr(address);
			service.sin_port=htons(port);
			if(::bind(sock,(sockaddr*)&service,sizeof(service))==-1)
				{throw SyZmO::ExceptionMissing(__FILE__,__LINE__);}
			}

		uint16_t receive(void* buffer,uint16_t length)
			{
			recv_timeout=0;
			sockaddr_in from;
			socklen_t length_from=sizeof(from);
			auto ret=recvfrom(sock,(char*)buffer,length,0,(sockaddr*)&from
				,&length_from);
			if(ret==-1)
				{
				if(errno==EAGAIN)
					{recv_timeout=1;}
				}
			return ret;
			}

		uint16_t receive(void* buffer,uint16_t length
			,char* addrbuff)
			{
			recv_timeout=0;
			sockaddr_in from;
			socklen_t length_from=sizeof(from);
			auto ret=recvfrom(sock,(char*)buffer,length,0,(sockaddr*)&from
				,&length_from);
			if(ret==-1)
				{
				if(errno==EAGAIN)
					{recv_timeout=1;}
				return ret;
				}

			auto temp=inet_ntoa(from.sin_addr);
			memset(addrbuff,0,ADDRBUFF_LENGTH*sizeof(char));
			strncpy(addrbuff,temp,ADDRBUFF_LENGTH);
			addrbuff[ADDRBUFF_LENGTH-1]=0;

			return ret;
			}

		uint16_t send(const void* buffer,uint16_t length,uint16_t port
			,const char* address)
			{
			sockaddr_in to;
			to.sin_family=AF_INET;
			to.sin_port=htons(port);
			to.sin_addr.s_addr=inet_addr(address);
			return sendto(sock,(const char*)buffer,length,0,(sockaddr*)&to,sizeof(to));
			}

		void broadcastEnable()
			{
			int broadcast = 1;
			if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void*)&broadcast
					,sizeof(broadcast)) == -1)
				{throw SyZmO::ExceptionMissing(__FILE__,__LINE__);}
			}

		void broadcastDisable()
			{
			int broadcast = 0;
			if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void*)&broadcast
					,sizeof(broadcast)) == -1)
				{throw SyZmO::ExceptionMissing(__FILE__,__LINE__);}
			}

		void recvTimeoutSet(double time)
			{
			timeval t;
			t.tv_sec=(time_t)time;
			t.tv_usec=(suseconds_t)(1e6 * (time - t.tv_sec) );
			if(setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (void*)&t
					,sizeof(t)) == -1)
				{throw SyZmO::ExceptionMissing(__FILE__,__LINE__);}
			}

		bool recvTimeout() const
			{return recv_timeout;}

		~Socket()
			{close(sock);}


	private:
		int sock;
		bool recv_timeout;
	};

SyZmO::SocketDatagram::SocketDatagram()
	{impl=new Socket;}

SyZmO::SocketDatagram::~SocketDatagram()
	{delete impl;}

void SyZmO::SocketDatagram::bind(uint16_t port)
	{impl->bind(port);}

void SyZmO::SocketDatagram::bind(uint16_t port,const char* address)
	{impl->bind(port,address);}

SyZmO::uint16_t SyZmO::SocketDatagram::receive(void* buffer,uint16_t length)
	{return impl->receive(buffer,length);}

SyZmO::uint16_t SyZmO::SocketDatagram::receive(void* buffer,uint16_t length,char* addr_from)
	{return impl->receive(buffer,length,addr_from);}

SyZmO::uint16_t SyZmO::SocketDatagram::send(const void* buffer,uint16_t length
	,uint16_t port,const char* address)
	{return impl->send(buffer,length,port,address);}

void SyZmO::SocketDatagram::broadcastEnable()
	{impl->broadcastEnable();}

void SyZmO::SocketDatagram::broadcastDisable()
	{impl->broadcastDisable();}

void SyZmO::SocketDatagram::recvTimeoutSet(double time)
	{impl->recvTimeoutSet(time);}

bool SyZmO::SocketDatagram::recvTimeout() const
	{return impl->recvTimeout();}