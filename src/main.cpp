#include "server.h"
#include <csignal>

Server* gServer = nullptr;

void signalHandler(int signal){
    if(gServer) {
        gServer->save("data.bin");
    }
    exit(0);
}

int main() {
    Server s(8080);
    gServer = &s;
    std::signal(SIGINT, signalHandler);

    if(s.load("data.bin")){
        std::cout << "Data loaded successfully\n";
    } else {
        std::cout << "No saved data found, starting fresh\n";
    }

    s.start();

    if(s.save("data.bin")) {
        std::cout << "Data saved successfully\n";
    } else {
        std::cerr << "Failed to save data\n";
    }

    return 0;
}
