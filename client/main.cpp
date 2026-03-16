#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>

void run() {
    WSAData wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);

    auto clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket == INVALID_SOCKET) {
        std::cerr << "Error while creating a socket: " << WSAGetLastError();
        return;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if(connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR){
        std::cerr << "Failed to connect to server: " << WSAGetLastError();
        return;
    }

    while(true) {
        std::string command;
        char buffer[1024] = {0};
        std::getline(std::cin, command);

        if(command == "EXIT"){
            send(clientSocket, command.data(), command.size(), 0);
            closesocket(clientSocket);
            WSACleanup();
            break;
        }

        send(clientSocket, command.data(), command.size(), 0);
        recv(clientSocket, buffer, sizeof(buffer), 0);
        std::cout << buffer << std::endl;
    }

}

int main(){

    run();

    return 0;
}
