#include "modian/infra/ipc/named_pipe_server.h"

#include <windows.h>
#include <utility>
#include <chrono>

#include "modian/core/logger/logger_service.h"
#include "modian/infra/utils/utils.h"

using namespace std::chrono_literals;

namespace modian::inkstone::infra::ipc {
	named_pipe_server::named_pipe_server(std::string_view pipe_name) : pipe_name_{pipe_name} {}

	named_pipe_server::~named_pipe_server() {
		stop();
	}

	void named_pipe_server::stop() {
		bool expected{true};

		if (!running_.compare_exchange_strong(expected, false)) {
			return;
		}

		HANDLE h_pipe = CreateFileW(
			utils::utf8_to_wstring(pipe_name_).c_str(),
			GENERIC_READ | GENERIC_WRITE,
			0, nullptr, OPEN_EXISTING, 0, nullptr
		);

		if (h_pipe != INVALID_HANDLE_VALUE) {
			CloseHandle(h_pipe);
		}
	}

	void named_pipe_server::run(request_handler_t handler) {
		if (running_) return;

		handler_ = std::move(handler);
		running_ = true;

		accept_thread_ = std::jthread([this](std::stop_token st) {
			this->accept_loop(std::move(st));
		});
	}

	void named_pipe_server::accept_loop(std::stop_token st) {
		while (running_ && !st.stop_requested()) {
			HANDLE pipe_handle = CreateNamedPipeW(
			utils::utf8_to_wstring(pipe_name_).c_str(),
			PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
				PIPE_UNLIMITED_INSTANCES,
				BUFFER_SIZE,
				BUFFER_SIZE,
				0,
				nullptr
			);

			if (pipe_handle == INVALID_HANDLE_VALUE) {
				core::logger_service::logger()->error("[infra] CreateNamedPipe failed. Error: {}", GetLastError());
				std::this_thread::sleep_for(100ms);
				continue;
			}

			const bool connected = ConnectNamedPipe(pipe_handle, nullptr)
							 ? true : (GetLastError() == ERROR_PIPE_CONNECTED);

			if (connected) {
				core::logger_service::logger()->info("Brush connected! Ready to grind ink.");

				if (!running_) {
					CloseHandle(pipe_handle);
					break;
				}

				std::lock_guard lock(threads_mutex_);
				client_threads_.emplace_back(
					[this, pipe_handle](std::stop_token st) {
						this->handle_session(pipe_handle, st);
					}
				);
			} else {
				core::logger_service::logger()->info("[Infra] Connection failed.");
				CloseHandle(pipe_handle);
			}
		}
		core::logger_service::logger()->info("[Infra] Server loop exited.");
	}

	void named_pipe_server::handle_session(void* raw_handle, std::stop_token st) const {
		HANDLE pipe_handle = raw_handle;

		std::vector<char> buffer(BUFFER_SIZE);
		DWORD bytes_read = 0;

		while (!st.stop_requested()) {
			bool success = ReadFile(
				pipe_handle,
				buffer.data(),
				static_cast<DWORD>(buffer.size()),
				&bytes_read,
				nullptr
			);

			if (!success || bytes_read == 0) {
				break;
			}

			const std::string_view request(buffer.data(), bytes_read);

			std::string response = handler_(request);

			DWORD bytes_written = 0;
			WriteFile(
				pipe_handle,
				response.data(),
				static_cast<DWORD>(response.size()),
				&bytes_written,
				nullptr
			);
		}

		core::logger_service::logger()->info("[Infra] Session ended (Client disconnected).");
		FlushFileBuffers(pipe_handle);
		DisconnectNamedPipe(pipe_handle);
		CloseHandle(pipe_handle);
	}
}
