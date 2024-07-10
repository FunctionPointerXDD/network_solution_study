#### [ 간단한 TCP echo service 만들기 ]
---
- TCP Echo service
	* server API
		1. socket( ) : file open &rarr; bind()가 끝나면 열림 (server socket)
		2. bind( ) : tcp/ip (port / ip addr)
		3. listen( ) : 1, 2가 충족되면 open되고 대기상태로 전환
		4. accept( ) : client에서 connect()하면 호출되서, **통신 socket**을 반환
		5. recv( )
		6. send( )
		7. shutdown( )
		8. closesocket( ) : client 요청에 의한 정상적인 종료의 경우, 먼저 통신 socket이 닫히고, 잠시 listen 상태로 변하고, 그 다음 server socket이 닫혀서 종료된다. 
	* client API
		1. socket( )
		2. connect( )
		3. send( )
		4. recv( )
		5. shutdown( ) : 연결 끊기 요청
		6. closesocket( )
- port
	* 할당가능한 포트 번호: 1 ~ 65535
	* 포트 번호는 프로세스 마다 unique하다.
		+ 만약 프로세스 a가 tcp 80번을 사용하고 있으면, 프로세스 b는 해당 소켓을 열 수 없다.

##### server socket create
```c++
/** 1. create listen socket 
* microsoft msdn socket function 참조할 것
*/
SOCKET hSocket = ::socket(AF_INET, SOCK_STREAM, 0); // open socket
			//L3 :IPv4 , L4: TCP  , protocol
if (hSocket == INVALID_SOCKET)
{
	puts("ERROR: listen socket create failed");
	return 0;
}
```
##
##### server port bind
```c++
// struct SOCKADDR_IN
// USHORT sin_port;  : unsigned short (16bit) 
// IN_ADDR sin_addr; : union (32bit)
// CHAR sin_zero[8];
SOCKADDR_IN svraddr = { 0 };

svraddr.sin_family = AF_INET; // IPv4
svraddr.sin_port = htons(25000); //tcp port number--> tcp:25000
// INADDR_ANY -> default addr  (0.0.0.0/0)
svraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // ip addr

if (::bind(hSocket, (SOCKADDR *)&svraddr, sizeof(svraddr)) == SOCKET_ERROR)
{
	puts("ERROR: ip or port bind failed");
	return 0;
}
```
- port 번호는 프로세스에 대한 식별자라고 볼 수 있다. (프로세스 마다 독립적으로 부여되므로)
- 대부분의 네트워크 서버는 big-endian을 사용한다. 이 때문에 little endian을 쓰는 client는 htons(), htonl()을 사용해서 big-endian으로 변환해주어야 한다.

##
##### server listen 
```c++
//서버가 closed에서 접속대기(Listen) 상태로 전환
if (::listen(hSocket, SOMAXCONN) == SOCKET_ERROR)
{
	puts("ERROR: listen failed");
	return 0;
}
```
- 현재는 운영체제가 listen()에서 연결될 여러 클라이언트를 동기적으로 처리해준다. (그래서 SOMAXCONN 파라미터는 크게 신경 안써도 됨..)

##
##### server listening and client socket setting
```c++
//client 접속 처리 및 대응
SOCKADDR_IN clientaddr = { 0 };
int nAddrLen = sizeof(clienaddr);
SOCKET hClient = 0;
char szBuffer[128] = { 0 };
int nReceive = 0;
```

##
##### server accept
```c++
//client connect -> accept, and new socket create(open)
while (hClient = ::accept((hSocket, (SOCKADDR *)&clientaddr, &nAddrLen)) != INVALID_SOCKET)
{
	puts("new client connect");
}
```

##
##### server receive and echo
```c++
//receive string from client 
// 버퍼 크기만큼 tcp buffer를 읽어드림(read == recv)
// receive 될 때까지 wait (만약 0이 반환되면 client의 종료 요청으로 받아드림)
while (nReceive = ::recv(hClient, szBuffer, sizeof(szBuffer), 0))
{
	//수신한 버퍼 크기만큼을 그대로 copy후 client에게 내용 그대로 전송 (write == send)
	::send(hClient, szBuffer, sizeof(szBuffer), 0);
	puts(szBuffer);
	memset(szBuffer, 0, sizeof(szBuffer));
}
```

##
##### server close
```c++

	::shutdown(hSocket, SD_BOTH); // 클라이언트 연결 종료
	::closesocket(hClient); //통신 소켓 종료
	puts("Client connection has been lost.");
}

//listen socket close
::closesocket(hSocket);

```
