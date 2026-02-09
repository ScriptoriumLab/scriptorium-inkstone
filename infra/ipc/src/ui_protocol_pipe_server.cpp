#include "modian/infra/ipc/ui_protocol_pipe_server.h"

#include <chrono>

#include "modian/core/logger/logger_service.h"
#include "modian/infra/utils/utils.h"

using namespace std::chrono_literals;

namespace modian::inkstone::infra::ipc {

    ui_protocol_pipe_server::ui_protocol_pipe_server(std::string_view pipe_name)
        : pipe_name_(pipe_name) {
        worker_thread_ = std::jthread([this](std::stop_token st) {
            this->connection_loop(st);
        });
    }

    ui_protocol_pipe_server::~ui_protocol_pipe_server() {
        if (const HANDLE h = pipe_handle_.exchange(INVALID_HANDLE_VALUE); h != INVALID_HANDLE_VALUE) {
            CloseHandle(h);
        }
    }

    void ui_protocol_pipe_server::send(const std::string& data) {
        std::lock_guard lock(send_mutex_);
        const HANDLE h = pipe_handle_.load();

        if (h == INVALID_HANDLE_VALUE) return;

        std::string payload = data + "\n";
        DWORD written = 0;

        const BOOL success = WriteFile(h, payload.data(), static_cast<DWORD>(payload.size()), &written, nullptr);

        if (!success) {
            core::logger_service::logger()->info("[UI-Pipe] Write failed, client disconnected.");
            DisconnectNamedPipe(h);
            CloseHandle(h);
            pipe_handle_.store(INVALID_HANDLE_VALUE);
        }
    }

    void ui_protocol_pipe_server::connection_loop(std::stop_token st) {
        while (!st.stop_requested()) {
            if (pipe_handle_.load() != INVALID_HANDLE_VALUE) {
                std::this_thread::sleep_for(100ms);
                continue;
            }

            HANDLE h = CreateNamedPipeW(
                utils::utf8_to_wstring(pipe_name_).c_str(),
                PIPE_ACCESS_DUPLEX,
                PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                1, 4096, 4096, 0, nullptr
            );

            if (h == INVALID_HANDLE_VALUE) {
                std::this_thread::sleep_for(1s);
                continue;
            }

            core::logger_service::logger()->info("[UI-Pipe] Waiting for Ink UI...");

            const bool connected = ConnectNamedPipe(h, nullptr) ? true : (GetLastError() == ERROR_PIPE_CONNECTED);

            if (connected && !st.stop_requested()) {
                core::logger_service::logger()->info("[UI-Pipe] UI Connected!");
                pipe_handle_.store(h);
            } else {
                CloseHandle(h);
            }
        }
    }
}