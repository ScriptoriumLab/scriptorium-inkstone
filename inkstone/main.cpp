#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

#include "modian/core/engine/pinyin_engine.h"
#include "modian/core/utils/utils.h"
#include "modian/manager/engine_manager.h"

const std::wstring PIPE_NAME = L"\\\\.\\pipe\\modian_ipc_pipe";
const int BUFFER_SIZE = 1024;

void run_server() {
    std::wcout << L"=== Modian Inkstone (Server) Started ===" << std::endl;
    std::wcout << L"Listening on pipe: " << PIPE_NAME << std::endl;

    auto candidate_manager = std::make_shared<modian::inkstone::manager::candidate_manager>();
    auto engine_manager = std::make_shared<modian::inkstone::manager::engine_manager>(candidate_manager);
    engine_manager->add_new_engine(modian::inkstone::core::lazy_load_dictionary<modian::inkstone::core::pinyin_engine>());

    while (true) {
        HANDLE hPipe = CreateNamedPipeW(
            PIPE_NAME.c_str(),
            PIPE_ACCESS_DUPLEX,
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
                buffer[bytesRead] = '\0';
                std::string recv_str = buffer;

                for (char c : recv_str) {
                    if (c == '\b') {
                        std::cout << "[Recv] CMD: Backspace" << std::endl;
                        engine_manager->handle_backspace();
                    } else {
                        std::cout << "[Recv] Key: " << c << std::endl;
                        engine_manager->update_input_state(c);

                        auto candidates = candidate_manager->get_candidates();
                        std::string response{""};
                        if (!candidates.empty()) {
                            std::wcout << L"candidate: " << candidates[0] << std::endl;
                            response = modian::inkstone::core::utils::to_utf8(candidates[0]);
                            engine_manager->reset();
                        } else {
                            std::cout << "nothing!!!" << std::endl;
                            response = "";
                        }

                        DWORD bytesWritten;
                        WriteFile(hPipe, response.c_str(), response.size(), &bytesWritten, nullptr);
                        std::cout << "[Replied]: " << response << std::endl;
                    }
                }
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