#pragma once

#include <iostream>
#include <string>
#include <Winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

class Socket
{
public:
	Socket(std::string div);
    ~Socket();
    bool create();
	bool createNoBlock();
	bool bind(const char* ip_address, int port);
	bool listen();
	bool connect(const char* ip_address, int port);
	bool send(const char* buffer, int length);
	bool receive(char* buffer, int length);
	SOCKET& getSocket();
	void setSocket(SOCKET socket);
	bool close();
private:
    SOCKET m_socket;
};