#include <iostream>
#include <string>
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")//加载ws2_32.ddl

using namespace std;

int main() {
    WORD sockVersion = MAKEWORD(2, 2);//socket版本，用2.2版本
    WSADATA wsaData;                 //WSADATA对象
    SOCKET serverSocket;             //服务器端套接字
    SOCKET clientSocket = INVALID_SOCKET;   //客户端套接字，初始化为 INVALID_SOCKET
    sockaddr_in sockAddr;            //服务器端口地址
    sockaddr_in clientAddr;          //客户端端口

    //初始化DLL
    if (WSAStartup(sockVersion, &wsaData) != 0) {
        return EXIT_FAILURE;
    }

    //创建套接字
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        cout << "created socket error." << endl;
        return EXIT_FAILURE;
    }

    //指定服务器IP地址和端口
    sockAddr.sin_family = AF_INET;                    //使用IPV4地址
    sockAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//具体的IP地址
    sockAddr.sin_port = htons(2200);                  //具体的端口号

    //绑定套接字
    if (bind(serverSocket, (sockaddr*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR) {
        cout << "bandle error!" << endl;
        return EXIT_FAILURE;
    }

    //进入监听状态，最多有10个连接
    if (listen(serverSocket, 10) == SOCKET_ERROR) {
        cout << "listen error!" << endl;
        return EXIT_FAILURE;
    }
    int length = sizeof(clientAddr);
    //cout << "Waiting for connect..." << endl;

    //实现客户端与服务器端通信
    const int datalength = 100;  //假设收到的字符长度最大为100
    char reciveClient[datalength] = { 0 };
    int client_data;
    while (1) {
        //等待客户端连接或等待客户端发送消息
        if (clientSocket == INVALID_SOCKET) {
            cout << "Waiting for connect..." << endl;
            clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &length);
            if (clientSocket == INVALID_SOCKET) {
                continue;
            }
            else {
                cout << "Created a connect line. client IP: " << inet_ntoa(clientAddr.sin_addr) << endl;
            }
        }
        else { //clientSocket 不是 INVALID_SOCKET，等待客户端发送消息                                          
            client_data = recv(clientSocket, reciveClient, datalength - 1, 0);
            if (client_data > 0) {
                cout << "Client message: " << reciveClient << endl;
                const char* sentClient = "\x6f\x70";
                send(clientSocket, sentClient, 2, 0);  //发送给客户端
                if (strcmp(reciveClient, "exit()") == 0) {            //客户端要求退出
                    closesocket(clientSocket);
                    clientSocket = INVALID_SOCKET; //重置 clientSocket 变量，等待下次连接
                    cout << "Client disconnected." << endl;
                }
            }
            else { //客户端断开连接
                cout << "Client disconnected." << endl;
                closesocket(clientSocket);
                clientSocket = INVALID_SOCKET; //重置 clientSocket 变量，等待下次连接
            }

            //设置循环出口
            if (strcmp(reciveClient, "exit()") == 0) {            //退出循环
                break;
            }
        }
    }

    //关闭工作
    closesocket(serverSocket);
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
