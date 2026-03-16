#include "server.h"

int main() {
    Server s(8080);

    if(s.load("data.bin")){
        std::cout << "Data loaded successfully\n";
    } else {
        std::cout << "No saved data found, starting fresh\n";
    }

    s.start();

    if(s.save("data.bin")) {
        std::cout << "Data saved successfully\n";
    } else {
        std::cout << "Failed to save data\n";
    }

    return 0;
}
