#include <iostream>
#include <string>
#include <WinSock2.h>
#include <thread> //添加多线程头文件
#include "DataToPc.h"
#include "DataFromPc.h"

#pragma comment(lib,"ws2_32.lib")//加载ws2_32.dll

using namespace std;

bool shouldSend = true; //标记是否发送数据
SOCKET clientSocket = INVALID_SOCKET; //客户端套接字，初始化为 INVALID_SOCKET

//发送数据线程函数
void sendData(SOCKET clientSock)
{
    while (1) {
        //每隔一秒给客户端发送数据
        if (shouldSend) {
            //发送数据格式
            DataToPc dataToPc = {
                true, false, false, false, false, false, false, false,
                true, false, false, false, false, false, false, false,
                1.1f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f, 7.7f, 8.8f, 9.9f, 10.0f,11.1f
            };
            char sendData[sizeof(dataToPc)] = { 0 };
            memcpy(sendData, &dataToPc, sizeof(dataToPc));
            if (!send(clientSocket, sendData, sizeof(sendData), 0)) {
                std::cerr << "Failed to send scan command." << std::endl;
            }
        }
        Sleep(1000); //睡眠1秒
    }
}

//接收数据线程函数
void recvData(SOCKET clientSock)
{
    char reciveClient[sizeof(DataFromPc)] = { 0 };
    int client_data;
    while (1) {
        //接收客户端数据
        client_data = recv(clientSocket, reciveClient, sizeof(DataFromPc) - 1, 0);
        if (client_data > 0) {
            cout << "Client message: " << reciveClient << endl;
            if (strcmp(reciveClient, "exit()") == 0) { //客户端要求退出
                closesocket(clientSocket);
                cout << "Client disconnected." << endl;
                break; //退出线程
            }
        }
        else { //客户端断开连接
            cout << "Client disconnected." << endl;
            //closesocket(clientSocket);
            break; //退出线程
        }
        cout << "Received message: " << reciveClient << endl;
        memset(reciveClient, 0, sizeof(reciveClient)); // 每次接收后需要清空缓冲区
    }
}

//int main() {
//    WSADATA wsaData; //WSADATA对象
//    WORD sockVersion = MAKEWORD(2, 2);//socket版本，用2.2版本
//    //初始化DLL
//    int result = WSAStartup(sockVersion, &wsaData);
//    if (result != 0) {
//        cout << "WSAStartup failed: " << result << endl;
//        return EXIT_FAILURE;
//    }
//
//    //创建套接字
//    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);; //服务器端套接字
//    if (serverSocket == INVALID_SOCKET) {
//        cout << "Socket creation failed: " << WSAGetLastError() << endl;
//        WSACleanup();
//        return EXIT_FAILURE;
//    }
//
//    //指定服务器IP地址和端口
//    sockaddr_in sockAddr;
//    sockAddr.sin_family = AF_INET;
//    sockAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
//    sockAddr.sin_port = htons(2200);
//
//    sockaddr_in clientAddr; //客户端端口
//    int length = sizeof(clientAddr);
//
//    //绑定套接字
//    result = bind(serverSocket, (sockaddr*)&sockAddr, sizeof(sockAddr));
//    if (result == SOCKET_ERROR) {
//        cout << "Bind failed: " << WSAGetLastError() << endl;
//        closesocket(serverSocket);
//        WSACleanup();
//        return EXIT_FAILURE;
//    }
//
//    //进入监听状态
//    result = listen(serverSocket, SOMAXCONN);
//    if (result == SOCKET_ERROR) {
//        cout << "Listen failed: " << WSAGetLastError() << endl;
//        closesocket(serverSocket);
//        WSACleanup();
//        return EXIT_FAILURE;
//    }
//
//    cout << "Waiting for connection..." << endl;
//
//    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &length);
//    if (clientSocket == INVALID_SOCKET) {
//        cout << "Accept failed: " << WSAGetLastError() << endl;
//        closesocket(serverSocket);
//        WSACleanup();
//        return 1;
//    }
//    else {
//        cout << "Connection established." << endl;
//
//        // 设置非阻塞模式
//        u_long iMode = 1;
//        ioctlsocket(clientSocket, FIONBIO, &iMode);
//
//        // 创建发送和接收线程
//        thread sender(sendData, clientSocket);
//        thread receiver(recvData, clientSocket);
//
//        sender.join();
//        receiver.join();
//    }
//
//    //关闭工作
//    closesocket(serverSocket);
//    closesocket(clientSocket);
//    WSACleanup();
//    return 0;
//}
