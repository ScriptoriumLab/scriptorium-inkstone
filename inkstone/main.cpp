#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

const std::wstring PIPE_NAME = L"\\\\.\\pipe\\modian_ipc_pipe";
const int BUFFER_SIZE = 1024;

void run_server() {
    std::wcout << L"=== Modian Inkstone (Server) Started ===" << std::endl;
    std::wcout << L"Listening on pipe: " << PIPE_NAME << std::endl;

    while (true) {
        HANDLE hPipe = CreateNamedPipeW(
            PIPE_NAME.c_str(),
            PIPE_ACCESS_INBOUND,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            1, // Max instances
            BUFFER_SIZE, // Out buffer
            BUFFER_SIZE, // In buffer
            0,           // Default timeout
            nullptr      // Security attributes
        );

        if (hPipe == INVALID_HANDLE_VALUE) {
            std::cerr << "CreateNamedPipe failed. Error: " << GetLastError() << std::endl;
            return;
        }

        std::cout << "Waiting for Brush connection..." << std::endl;

        bool connected = ConnectNamedPipe(hPipe, nullptr) ?
                         true : (GetLastError() == ERROR_PIPE_CONNECTED);

        if (connected) {
            std::cout << "Brush connected! Ready to grind ink." << std::endl;

            char buffer[BUFFER_SIZE];
            DWORD bytesRead;

            while (ReadFile(hPipe, buffer, BUFFER_SIZE - 1, &bytesRead, nullptr) != FALSE) {
                buffer[bytesRead] = '\0'; // 补上字符串结束符

                std::cout << "[Received]: " << buffer << std::endl;

                // TODO: 这里未来会连接 EngineManager 进行查词
            }
        }

        std::cout << "Brush disconnected." << std::endl;
        DisconnectNamedPipe(hPipe);
        CloseHandle(hPipe);
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    try {
        run_server();
    } catch (const std::exception& e) {
        std::cerr << "Inkstone crashed: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}