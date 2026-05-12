#include "modian/infra/ipc/win/async_named_pipe_server.h"

#include <chrono>

#include "modian/common/core/logger/logger_service.h"
#include "modian/common/infra/utils/string_utils.h"

using namespace std::chrono_literals;

namespace modian::inkstone::infra::ipc {

    async_named_pipe_server::async_named_pipe_server(std::string_view pipe_name)
        : pipe_name_(pipe_name) {
        worker_thread_ = std::jthread([this](std::stop_token st) {
            this->connection_loop(st);
        });
    }

    async_named_pipe_server::~async_named_pipe_server() {
        if (const HANDLE h = pipe_handle_.exchange(INVALID_HANDLE_VALUE); h != INVALID_HANDLE_VALUE) {
            CloseHandle(h);
        }
    }

    void async_named_pipe_server::send(const std::string& data) {
        std::lock_guard lock(send_mutex_);
        const HANDLE h = pipe_handle_.load();

        if (h == INVALID_HANDLE_VALUE) return;

        std::string payload = data + "\n";
        DWORD written = 0;

        const BOOL success = WriteFile(h, payload.data(), static_cast<DWORD>(payload.size()), &written, nullptr);

        if (!success) {
            common::core::logger_service::logger()->info("[UI-Pipe] Write failed, client disconnected.");
            DisconnectNamedPipe(h);
            CloseHandle(h);
            pipe_handle_.store(INVALID_HANDLE_VALUE);
        }
    }

    void async_named_pipe_server::connection_loop(std::stop_token st) {
        while (!st.stop_requested()) {
            if (pipe_handle_.load() != INVALID_HANDLE_VALUE) {
                std::this_thread::sleep_for(100ms);
                continue;
            }

            HANDLE h = CreateNamedPipeW(
                common::infra::utils::utf8_to_wstring(pipe_name_).c_str(),
                PIPE_ACCESS_DUPLEX,
                PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                1, 4096, 4096, 0, nullptr
            );

            if (h == INVALID_HANDLE_VALUE) {
                std::this_thread::sleep_for(1s);
                continue;
            }

            common::core::logger_service::logger()->info("[UI-Pipe] Waiting for Ink UI...");

            const bool connected = ConnectNamedPipe(h, nullptr) ? true : (GetLastError() == ERROR_PIPE_CONNECTED);

            if (connected && !st.stop_requested()) {
                common::core::logger_service::logger()->info("[UI-Pipe] UI Connected!");
                pipe_handle_.store(h);

                read_loop(h, st);

                pipe_handle_.store(INVALID_HANDLE_VALUE);
            }

            CloseHandle(h);
        }
    }

    void async_named_pipe_server::read_loop(HANDLE h, const std::stop_token& st) const {
        std::vector<char> buffer(4096);
        DWORD bytes_read = 0;
        DWORD bytes_avail = 0;

        while (!st.stop_requested()) {
            BOOL peek_success = PeekNamedPipe(
                h,
                nullptr,
                0,
                nullptr,
                &bytes_avail,
                nullptr
            );

            if (!peek_success) {
                common::core::logger_service::logger()->info("[UI-Pipe] Peek failed (Client disconnected?)");
                break;
            }

            if (bytes_avail == 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }

            BOOL success = ReadFile(
                h,
                buffer.data(),
                static_cast<DWORD>(buffer.size()),
                &bytes_read,
                nullptr
            );

            if (!success || bytes_read == 0) {
                break;
            }

            std::string message(buffer.data(), bytes_read);

            if (!message.empty()) {
                if (message_handler_) {
                    message_handler_(message);
                }
            }
        }
    }
}
