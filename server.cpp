#include <iostream>
#include <thread>
#include "Socket.h"

using namespace std;

void recv(Socket* recv_socket,char* recvBuff){
    recv_socket->receive(recvBuff,sizeof(recvBuff));
}

void send(Socket* send_socket,const char* sendBuff){
    cout << "sentBuffer : " << sendBuff << endl;
    send_socket->send(sendBuff,sizeof(sendBuff));            
}

void sendThread(Socket* send_socket,const char* sendBuff){
    while(1){
        //cout << "sentBuffer for thread : " << sendBuff << endl;
        send_socket->send(sendBuff,sizeof(sendBuff));
    }
}

int main(){
	Socket s_socket;
    if (!s_socket.create()) {
        std::cout << "Failed to create socket" << std::endl;
        return false;
    }
    
    if(!s_socket.bind("127.0.0.1",2200)) {
        std::cout << "Failed to bind socket" << std::endl;
        return false;
    }

    if (s_socket.listen()) {
		cout << "listen error!"<<endl;
		return EXIT_FAILURE;
	}
    
    cout << "watting for connect......"<<endl;

    Socket c_socket;
    sockaddr_in clientAddr;
    int length = sizeof(clientAddr);
    c_socket.setSocket(accept(s_socket.getSocket(),(sockaddr*)&clientAddr,&length));
    if (c_socket.getSocket() == SOCKET_ERROR) {
		cout << "connect error!"<<endl;
		return EXIT_FAILURE;
	}
	else {
		cout << "created a connect line." << inet_ntoa(clientAddr.sin_addr) << endl;
         while(1){
             const char* sentBuffer = "\x6f\x70";
             Socket* p_socket = &c_socket;
             send(p_socket,sentBuffer);
         }
        /*const char* sentBuffer = "\x6f\x70";
        Socket* p_socket = &c_socket;
        std::thread first(sendThread,p_socket,sentBuffer);
        return 0;*/
	}
}