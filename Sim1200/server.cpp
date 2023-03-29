#include <iostream>
#include <thread>
#include <chrono>
#include "Socket.h"
#include "DataToPc.h"
#include "DataFromPc.h"

using namespace std;

void receiveData(Socket* c_socket) {
    while (true) {
        char receiveBuff[sizeof(DataFromPc)] = { 0 };
        int len = c_socket->receive(receiveBuff, sizeof(receiveBuff));
        //int len = c_socket->receive(buff, sizeof(buff));
        if (len == SOCKET_ERROR) { // �����ݿ���
            int errCode = WSAGetLastError();
            if (errCode == WSAEWOULDBLOCK) { //û�п�������
                continue;
            }
            else { // �ش����
                break;
            }
        }
        else if (len == 0) {
            cout << "The socket has been closed by the client" << endl;
            break;
        }
        DataFromPc* dataFromPc = reinterpret_cast<DataFromPc*>(receiveBuff); //�� buffer ����Ϊ�ṹ��
        std::cout << "dataFromPc  : " << dataFromPc << std::endl;
    }
}

void sendData(Socket* c_socket) {
    while (true) {
        // ����һ��Socket* p_socket
        Socket* p_socket = c_socket;

        //�������ݸ�ʽ
        DataToPc dataToPc = {
            true, false, false, false, false, false, false, false,
            true, false, false, false, false, false, false, false,
            1.1f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f, 7.7f, 8.8f, 9.9f, 10.0f,11.1f
        };
        // ������ת��Ϊ�ַ�����
        char buffer[sizeof(DataToPc)];
        memcpy(buffer, &dataToPc, sizeof(DataToPc));
        
        // ��������
        int length = sizeof(DataToPc);
        if (p_socket->send(buffer, length)) {
            std::cout << "Data sent successfully." << std::endl;
        }
        else {
            std::cout << "Failed to send data." << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    Socket s_socket("server");
    if (!s_socket.create()) {
        std::cout << "Failed to create socket" << std::endl;
        return false;
    }

    if (!s_socket.bind("127.0.0.1", 2200)) {
        std::cout << "Failed to bind socket" << std::endl;
        return false;
    }

    if (s_socket.listen()) {
        cout << "listen error!" << endl;
        return EXIT_FAILURE;
    }

    cout << "waiting for connect......" << endl;

    while (true) {
        Socket c_socket("client");
        sockaddr_in clientAddr;
        int length = sizeof(clientAddr);
        SOCKET clientSocket = accept(s_socket.getSocket(), (sockaddr*)&clientAddr, &length);
        if (clientSocket == SOCKET_ERROR) {
            cout << "connect error!" << endl;
            s_socket.close(); // �رշ���˵�socket
            return EXIT_FAILURE;
        }
        else {
            c_socket.setSocket(clientSocket);
            cout << "created a connect line." << inet_ntoa(clientAddr.sin_addr) << endl;
            // �������պͷ����߳�
            std::thread receiveThread(receiveData, &c_socket);
            std::thread sendThread(sendData, &c_socket);

            //�ȴ����ͺͽ����߳���ֹ
            sendThread.join();
            receiveThread.join();
            c_socket.close(); // �رտͻ����׽���
        }
    }

    s_socket.close(); // �رշ���˵�socket
    return 0;
}
