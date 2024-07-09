## Socket
- Socket은 OS kernel에서 구현되어 있는 프로토콜 요소에 대한 추상화된 인터페이스
	* 응용 계층과 커널의 접점
	* 사용하는 프로토콜에 따라 소켓의 이름이 명명된다.
		+ ex) 블루투스 소켓, IRDA 소켓, TCP 소켓
- 장치 파일의 일종으로 이해할 수 있다.
- 일반 파일에 대한 개념이 대부분 적용된다.
	&rarr; 여기서는 TCP Socket 대해서 다룬다.

- 소켓은 본질적으로 **파일**이므로, 일반적으로 파일을 다루는 방법과 거의 동일하다.   
#####
- **TCP 소켓 통신에서 파일의 단위**
	* application(프로세스 관점) : stream
		+ 연속된 segment
	* L4 : segment
	* L3 : packet


- **TCP의 연결**
	* 상태(state)가 계속 변함 &rarr; 상태의 전이
		+ tcp 상태 다이어그램
	&rarr; udp에서는 상태라는 개념이 존재하지 않는다.
	* 기본적으로 사용할 수 있는 소켓의 개수가 정해져 있다고 가정해 볼 수 있다.
	* 3-way hand shake
		1. client 에서 server에 SYN 요청	
		2. listen하고 있던 서버가 SYN Recieved 그리고 ACK 전달
		3. client ESTABLISHED 그리고 ACK 전달
		4. server ESTABLISHED
	&rarr; CLOSE 할 때는 **클라이언트에서 먼저** FIN 요청을 서버에게 보내는 것이 정상적인 절차이다. (4-way handshaking)
	* wireshark 로 연결과정을 확인가능
######
![tcp socket diagram](https://upload.wikimedia.org/wikipedia/en/5/57/Tcp_state_diagram.png?20080306050740)
######
- Windows Socket 함수들 &rarr; API, SDK ... 
