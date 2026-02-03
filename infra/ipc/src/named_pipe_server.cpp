#include "modian/infra/ipc/named_pipe_server.h"

#include <windows.h>

#include "modian/core/logger/logger_service.h"
#include "modian/infra/utils/utils.h"


namespace modian::inkstone::infra::ipc {
	named_pipe_server::named_pipe_server(std::string_view pipe_name) : pipe_name_{pipe_name} {}

	named_pipe_server::~named_pipe_server() {
		stop();
	}

	void named_pipe_server::stop() {
		bool expected{true};

		if (running_.compare_exchange_strong(expected, false)) {
			core::logger_service::logger()->info("[infra] Stopping Named Pipe Server...");
		}
	}

	void named_pipe_server::run(request_handler_t handler) {
		running_ = true;
		core::logger_service::logger()->info("[infra] Named Pipe Server starting at: {}", pipe_name_);

		while (running_) {
			HANDLE pipe_handle = CreateNamedPipeW(
				utils::utf8_to_wstring(pipe_name_).c_str(),
				PIPE_ACCESS_DUPLEX,
				PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
				1,
				BUFFER_SIZE,
				BUFFER_SIZE,
				0,
				nullptr
			);

			if (pipe_handle == INVALID_HANDLE_VALUE) {
				core::logger_service::logger()->error("[infra] CreateNamedPipe failed. Error: {}", GetLastError());
				Sleep(100);
				continue;
			}

			bool connected = ConnectNamedPipe(pipe_handle, nullptr) ? true : (GetLastError() == ERROR_PIPE_CONNECTED);

			if (connected) {
				core::logger_service::logger()->info("Brush connected! Ready to grind ink.");

				char buffer[BUFFER_SIZE];
				DWORD bytes_read;

				while (ReadFile(pipe_handle, buffer, BUFFER_SIZE - 1, &bytes_read, nullptr) != FALSE) {
					if (bytes_read > 0) {
						buffer[bytes_read] = '\0';
						std::string request = buffer;

						std::string response = handler(request);

						DWORD bytes_written;
						WriteFile(pipe_handle, response.c_str(), response.size(), &bytes_written, nullptr);
					}
				}
				core::logger_service::logger()->debug("[Infra] Client disconnected.");
			} else {
				core::logger_service::logger()->info("[Infra] Connection failed.");
			}

			core::logger_service::logger()->info("Brush disconnected.");
			DisconnectNamedPipe(pipe_handle);
			CloseHandle(pipe_handle);
		}
		core::logger_service::logger()->info("[Infra] Server loop exited.");
	}
}
