#include "server.h"

#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <modian/core/logger/logger_service.h>

#include "modian/core/engine/pinyin_engine.h"
#include "modian/core/utils/utils.h"

namespace modian::inkstone {
	const std::wstring PIPE_NAME = L"\\\\.\\pipe\\modian_ipc_pipe";

	server::server() {
		candidate_manager_ = std::make_shared<manager::candidate_manager>();
		engine_manager_ = std::make_shared<manager::engine_manager>(candidate_manager_);
		engine_manager_->add_new_engine(core::lazy_load_dictionary<core::pinyin_engine>());
	}

	void server::run() const {
		while (true) {
			HANDLE hPipe = CreateNamedPipeW(
				PIPE_NAME.c_str(),
				PIPE_ACCESS_DUPLEX,
				PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
				1, // Max instances
				BUFFER_SIZE, // Out buffer
				BUFFER_SIZE, // In buffer
				0, // Default timeout
				nullptr // Security attributes
			);

			if (hPipe == INVALID_HANDLE_VALUE) {
				core::logger_service::logger()->error("CreateNamedPipe failed. Error: {}", GetLastError());
				return;
			}

			core::logger_service::logger()->info("Waiting for Brush connection...");

			bool connected = ConnectNamedPipe(hPipe, nullptr) ? true : (GetLastError() == ERROR_PIPE_CONNECTED);

			if (connected) {
				core::logger_service::logger()->info("Brush connected! Ready to grind ink.");

				char buffer[BUFFER_SIZE];
				DWORD bytesRead;

				while (ReadFile(hPipe, buffer, BUFFER_SIZE - 1, &bytesRead, nullptr) != FALSE) {
					buffer[bytesRead] = '\0';
					std::string recv_str = buffer;

					for (char c : recv_str) {
						if (c == '\b') {
							core::logger_service::logger()->info("[Recv] CMD: Backspace");
							engine_manager_->handle_backspace();
						} else {
							core::logger_service::logger()->info("[Recv] Key: {}", c);
							engine_manager_->update_input_state(c);

							auto candidates = candidate_manager_->get_candidates();
							std::string response{""};
							if (!candidates.empty()) {
								core::logger_service::logger()->info("[candidate]: {}", core::utils::to_utf8(candidates[0]));
								response = core::utils::to_utf8(candidates[0]);
								engine_manager_->reset();
							} else {
								response = "";
							}

							DWORD bytesWritten;
							WriteFile(hPipe, response.c_str(), response.size(), &bytesWritten, nullptr);
							core::logger_service::logger()->info("[Replied]: {}", response);
						}
					}
				}
			}


			core::logger_service::logger()->info("Brush disconnected.");
			DisconnectNamedPipe(hPipe);
			CloseHandle(hPipe);
		}
	}
}
