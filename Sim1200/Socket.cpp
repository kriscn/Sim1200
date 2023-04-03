#include "Socket.h"
#include <iostream>

Socket::Socket(std::string div)
{
	std::cout << "Socket constructor called : " << div << std::endl;
}

Socket::~Socket()
{
	std::cout << "Socket destructor called" << std::endl;
}

bool Socket::create() {	
	WORD sockVersion = MAKEWORD(2,2);//socket版本，用2.2版本
	WSADATA wsadate;                 //WSADATA对象	
	//初始化DLL
	if (WSAStartup(sockVersion, &wsadate) != 0) {
		return EXIT_FAILURE;
	}

	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET) {
		return false;
	}
	return true;
}

bool Socket::createNoBlock(){
	WORD sockVersion = MAKEWORD(2,2);//socket版本，用2.2版本
	WSADATA wsadate;                 //WSADATA对象	
	//初始化DLL
	if (WSAStartup(sockVersion, &wsadate) != 0) {
		return EXIT_FAILURE;
	}

	unsigned long ul=1;
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int ret=ioctlsocket(m_socket, FIONBIO, (unsigned long *)&ul);//设置成非阻塞模式。

	return true;
}

bool Socket::bind(const char* ip_address, int port) {
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(ip_address);
	server_address.sin_port = htons(port);
	return ::bind(m_socket, (SOCKADDR*)&server_address, sizeof(server_address)) == 0;
}

bool Socket::listen(){
	return ::listen(m_socket, 1);
}

bool Socket::connect(const char* ip_address, int port) {
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(ip_address);
	server_address.sin_port = htons(port);
	return ::connect(m_socket, (SOCKADDR*)&server_address, sizeof(server_address)) == 0;
}

bool Socket::send(const char* buffer, int length) {
	try {
		// 发送心跳包
		//static const char heartbeat[] = "H";
		//::send(m_socket, heartbeat, sizeof(heartbeat), 0);
		// 发送原始数据
		return ::send(m_socket, buffer, length, 0) != SOCKET_ERROR;
	}
	catch (const std::exception& e) {
		std::cerr << "Error sending data: " << e.what() << std::endl;
		return SOCKET_ERROR;
	}
}

bool Socket::receive(char* buffer, int length) {
	int total_bytes_received = 0; // 已接收到的字节数
	int bytes_received = 0; // 每次接收到的字节数
	int idle_time = 0; // 非活动时间
	while (total_bytes_received < length) {
		// 接收数据
		bytes_received = ::recv(m_socket, buffer + total_bytes_received, length - total_bytes_received, 0);
		if (bytes_received == SOCKET_ERROR) {
			return false;
		}
		else if (bytes_received == 0) {
			return false;
		}
		// 检查接收到的数据是否是心跳包
		//if (bytes_received == 1 && buffer[total_bytes_received] == 'H') {
		//	// 接收到了心跳包
		//	idle_time = 0;
		//	continue;
		//}

		// 累计接收到的字节数
		total_bytes_received += bytes_received;

		// 更新最后活动时间（用于超时检查）
		idle_time = 0;
	}

	// 正常接收到数据，返回true
	return true;
}

SOCKET& Socket::getSocket(){
	return m_socket;
}

void Socket::setSocket(SOCKET socket){
	this->m_socket = socket;
}

bool Socket::close() {
	return ::closesocket(m_socket) != SOCKET_ERROR;
}