#include <windows.h>
#include <iostream>
#include "server.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);

    try {
        const modian::inkstone::server inkstone_server;
        inkstone_server.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}