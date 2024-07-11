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
- 대부분의 네트워크 서버는 big-endian을 사용한다. 이 때문에 little endian을 쓰는 서버는 htons(), htonl()을 사용해서 big-endian으로 변환해주어야 한다.

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
//통신 소켓 연결
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
---
##
##### client create
```c++
SOCKET hSocket = ::socket(AF_INET, SOCK_STREAM, 0);
if (hSocket == INVALID_SOCKET)
{
	puts("ERROR: socket create failed");
	return 0;
}
```

##
##### client connect
- 클라이언트의 포트번호는 운영체제에서 자동으로 지정해 준다.
```c++
SOCKADDR_IN svraddr = { 0 };
svraddr.sin_family = AF_INET;
//연결할 서버의 포트 번호
svraddr.sin_port = htons(25000);
//연결할 서버 ip 주소
svraddr.sin_addr.S_un.S_addr = inet_addr("192.168.66.1"); //--> 문자열로 입력해주면 알아서 숫자로 잘 변환해준다. // 컴퓨터 한 대로 할 경우 127.0.0.1로 테스트
if (::connect(hSocket, (SOCKADDR *)&svraddr, sizeof(svraddr)) == SOCKET_ERROR)
{
	puts("ERROR: Unable to connect to server.");
	return 0;
}
```
&rarr; 처음 TCP 연결 과정에서(3-way handshaking) 여러 정보를 교환하는데, 이 때 보안상의 목적으로 **Sequence number**를 교환한다. 이것은 unsigned int 32bit의 임의의 값이 부여된다. segment 단위로 교환이 되며, 서로 SYN을 보내고, 응답할 때 마다 받은 SYN값에 1을 더해서 ACK를 보낸다. 그리고 TCP의 최대 크기의 정보가 있는 MSS의 값도 같이 교환된다. 이 외에도 여러가지 정보가 교환된다. 


##
##### client send/recv
```c++
//채팅 메세지 송/수신
char szBuffer[128] = { 0 };
while (1)
{
	gets_s(szBuffer);
	if (strcmp(szBuffer, "EXIT") == 0)
		break ;
	
	//send to server
	::send(hSocket, szBuffer, strlen(szBuffer) + 1, 0);
	//clear buffer
	memset(szBuffer, 0, sizeof(szBuffer));
	//recieve from server echo msg
	::recv(hSocket, szBuffer, sizeof(szBuffer), 0);
	printf("Form server: %s\n", szBuffer);
}
```

##
##### client close
```c++
::shutdown(hSocket, SD_BOTH);
::closesocket(hSocket);
```

- 종료할 때는 4-way handshaking이 이루어진다. 
- 그리고 반드시 종료는 **client**가 해야 종료되게끔 만들어야 한다!!
- client가 FIN + ACK를 server에게 보내면, 통신 종료과정이 진행된다.