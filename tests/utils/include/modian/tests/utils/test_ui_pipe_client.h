#pragma once

#include <filesystem>
#include <chrono>
#include <windows.h>
#include <string>
#include <vector>
#include <thread>

using namespace std::chrono_literals;

namespace modian::inkstone::tests::utils {
    class test_ui_pipe_client {
    public:
        explicit test_ui_pipe_client(std::string_view pipe_name) {
            pipe_path_ = std::filesystem::path(pipe_name).wstring();
        }

        ~test_ui_pipe_client() {
            close();
        }

        void close() {
            if (handle_ != INVALID_HANDLE_VALUE) {
                CloseHandle(handle_);
                handle_ = INVALID_HANDLE_VALUE;
            }
        }

        bool connect(int max_retries = 10) {
            for (int i = 0; i < max_retries; ++i) {
                handle_ = CreateFileW(
                    pipe_path_.c_str(),
                    GENERIC_READ | GENERIC_WRITE,
                    0, nullptr, OPEN_EXISTING, 0, nullptr
                );

                if (handle_ != INVALID_HANDLE_VALUE) {
                    DWORD mode = PIPE_READMODE_MESSAGE;
                    SetNamedPipeHandleState(handle_, &mode, nullptr, nullptr);
                    return true;
                }

                if (GetLastError() == ERROR_PIPE_BUSY) {
                    WaitNamedPipeW(pipe_path_.c_str(), 200);
                } else {
                    std::this_thread::sleep_for(100ms);
                }
            }
            return false;
        }

        std::string read_next_message(int timeout_ms = 2000) const {
            if (handle_ == INVALID_HANDLE_VALUE) {
                throw std::runtime_error("UI Pipe not connected");
            }

            std::vector<char> buffer(1024 * 16);
            DWORD bytes_read = 0;

            auto start = std::chrono::steady_clock::now();
            while (std::chrono::steady_clock::now() - start < std::chrono::milliseconds(timeout_ms)) {
                DWORD available = 0;
                if (!PeekNamedPipe(handle_, nullptr, 0, nullptr, &available, nullptr)) {
                     throw std::runtime_error("PeekNamedPipe failed");
                }

                if (available > 0) {
                    if (ReadFile(handle_, buffer.data(), static_cast<DWORD>(buffer.size()), &bytes_read, nullptr)) {
                        std::string msg(buffer.data(), bytes_read);
                        if (!msg.empty() && msg.back() == '\n') msg.pop_back();
                        return msg;
                    }
                }
                std::this_thread::sleep_for(10ms);
            }

            throw std::runtime_error("Read timeout");
        }

    private:
        std::wstring pipe_path_;
        HANDLE handle_{INVALID_HANDLE_VALUE};
    };
}