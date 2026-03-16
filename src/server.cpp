#include "server.h"
#include <sstream>

Server::Server(int portNumber)
{
    m_port = portNumber;
    m_serverFd = INVALID_SOCKET;
}

void Server::start()
{
    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    m_serverFd = socket(AF_INET, SOCK_STREAM, 0);

    if(m_serverFd == INVALID_SOCKET) {
        std::cerr << "Error while creating a socket: " << WSAGetLastError();
        return;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);
    inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);
    int bindRes = bind(m_serverFd, (struct sockaddr*)&addr, sizeof(addr));

    if(bindRes == SOCKET_ERROR) {
        std::cerr << "Error while binding port to socket: " << WSAGetLastError();
        return;
    }

    if(listen(m_serverFd, 10) == SOCKET_ERROR) {
        std::cerr << "Error while listening: " << WSAGetLastError();
        return;
    }

    acceptLoop();
}

bool Server::save(const std::string &filename)
{
    return m_store.save(filename);
}

bool Server::load(const std::string &filename)
{
    return m_store.load(filename);
}

void Server::acceptLoop()
{
    while(true){
        SOCKET clientFd = accept(m_serverFd, nullptr, nullptr);
        if(clientFd == INVALID_SOCKET){
            continue;
        }
        std::thread t(&Server::handleClient, this, clientFd);
        t.detach();
    }
}

void Server::handleClient(SOCKET clientSocket)
{

    while(true) {

        char buffer[1024] = {0};
        std::string response;

        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if(bytesReceived <= 0) {
            closesocket(clientSocket);
            return;
        }

        std::string userInput = buffer;
        std::vector<std::string> tokens;
        std::istringstream iss(userInput);
        std::string token;
        while(iss >> token) {
            tokens.push_back(token);
        }

        if(tokens.empty()){
            continue;
        }

        std::string command = tokens[0];
        if(command == "SET") {
            if(tokens.size() == 3) {
                std::string key = tokens[1];
                std::string value = tokens[2];
                m_store.set(key, value);
                response = "+OK";
            } else if(tokens.size() == 5){
                std::string key = tokens[1];
                std::string value = tokens[2];
                std::optional<int> ttlSeconds = std::make_optional(std::stoi(tokens[4]));
                m_store.set(key, value, ttlSeconds);
                response = "+OK";
            } else {
                response = "-ERR wrong number of arguments";
            }
        } else if(command == "GET") {
            if(tokens.size() == 2) {
                std::string key = tokens[1];
                auto result = m_store.get(key);
                if(result.has_value()){
                    response = "+" + result.value();
                } else {
                    response = "-ERR key not found";
                }
            } else {
                response = "-ERR wrong number of arguments";
            }
        } else if(command == "DEL") {
            if(tokens.size() == 2) {
                std::string key = tokens[1];
                if(m_store.del(key)){
                    response = ":1";
                } else {
                    response = ":0";
                }
            } else {
                response = "-ERR wrong number of arguments";
            }
        } else if(command == "EXISTS") {
            if(tokens.size() == 2) {
                std::string key = tokens[1];
                if(m_store.exists(key)){
                    response = ":1";
                } else {
                    response = ":0";
                }
            } else {
                response = "-ERR wrong number of arguments";
            }
        } else if(command == "PING"){
            if(tokens.size() == 1) {
                response = "+PONG";
            } else {
                response = "-ERR wrong number of arguments";
            }
        } else if(command == "EXIT") {
            closesocket(clientSocket);
            return;
        } else {
            response = "-ERR unknown command";
        }

        response += "\n";
        send(clientSocket, response.data(), response.size(), 0);
    }
}
