#include <iostream>
#include "Socket.cpp"

using namespace std;

void recv(Socket* recv_socket,char* recvBuff){
    recv_socket->receive(recvBuff,sizeof(recvBuff));
}

void send(Socket* send_socket,const char* sendBuff){
    cout << "sentBuffer : " << sendBuff << endl;
    send_socket->send(sendBuff,sizeof(sendBuff));            
}

int main(){
    Socket c_socket;
    if (!c_socket.create()) {
        std::cout << "Failed to create socket" << std::endl;
        return false;
    }

    if(!c_socket.connect("127.0.0.1",2200)){
        cout << "connect error!" << endl;
		return EXIT_FAILURE;
	}
	else {
		cout << "connect success!" << endl;
        while(1){
            char receiveBuffer;
            char* receiveBuff = &receiveBuffer;
            Socket* p_socket = &c_socket;
            recv(p_socket,receiveBuff);

            cout<< "receiveBuff : " << receiveBuff << endl;
        }
	}
}