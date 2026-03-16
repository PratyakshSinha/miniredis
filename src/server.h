#ifndef SERVER_H
#define SERVER_H

#include "store.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <iostream>

class Server {

public:

    /**
     * @brief Server: Constructor of the class server will open a port
     * @param portNumber: Port which needs to be opened
     */
    Server(int portNumber);

    /**
     * @brief start: Opens socket and starts accepting loop
     */
    void start();

    /**
     * @brief save: Saves data in the memory to a file
     * @param filename: name of the file to which data needs to be saved
     * @return true if data is successfully saved
     */
    bool save(const std::string& filename);

    /**
     * @brief load: Loads data from a file into the memory
     * @param filename: name of the file which needs to be loaded
     * @return true if data is loaded successfully
     */
    bool load(const std::string& filename);

private:

    /**
     * @brief acceptLoop: Waits for incoming connections and spawns a thread per client
     */
    void acceptLoop();

    /**
     * @brief handleClient: Runs in its own thread read command and sends response
     * @param clientSocket: Client's socket that will be sending commands
     */
    void handleClient(SOCKET clientSocket);

    int m_port;
    Store m_store;
    SOCKET m_serverFd;
};

#endif
