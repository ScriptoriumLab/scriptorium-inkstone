#pragma once

#include <filesystem>
#include <chrono>
#include <windows.h>

using namespace std::chrono_literals;

namespace scriptorium::inkstone::tests::utils {
	class test_input_protocol_pipe_client {
	public:
		explicit test_input_protocol_pipe_client(std::string_view pipe_name) {
			pipe_path_ = std::filesystem::path(pipe_name).wstring();
		}

		~test_input_protocol_pipe_client() {
			close();
		}

		test_input_protocol_pipe_client(const test_input_protocol_pipe_client&) = delete;
		test_input_protocol_pipe_client& operator=(const test_input_protocol_pipe_client&) = delete;

		test_input_protocol_pipe_client(test_input_protocol_pipe_client&& other) noexcept : pipe_path_(std::move(other.pipe_path_)), handle_(other.handle_) {
			other.handle_ = INVALID_HANDLE_VALUE;
		}

		test_input_protocol_pipe_client& operator=(test_input_protocol_pipe_client&& other) noexcept {
			if (this != &other) {
				close();
				pipe_path_ = std::move(other.pipe_path_);
				handle_ = other.handle_;
				other.handle_ = INVALID_HANDLE_VALUE;
			}
			return *this;
		}

		bool connect(int max_retries = 5) {
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

		[[nodiscard]] std::string send_and_receive(std::string_view message) const {
			if (handle_ == INVALID_HANDLE_VALUE) {
				throw std::runtime_error("Pipe not connected");
			}

			DWORD bytes_written;
			if (!WriteFile(handle_, message.data(), static_cast<DWORD>(message.size()), &bytes_written, nullptr)) {
				throw std::runtime_error("Failed to write to pipe");
			}

			std::vector<char> buffer(1024 * 16);
			DWORD bytes_read;
			if (!ReadFile(handle_, buffer.data(), static_cast<DWORD>(buffer.size()), &bytes_read, nullptr)) {
				throw std::runtime_error("Failed to read from pipe");
			}

			return { buffer.data(), bytes_read };
		}

		void close() {
			if (handle_ != INVALID_HANDLE_VALUE) {
				CloseHandle(handle_);
				handle_ = INVALID_HANDLE_VALUE;
			}
		}
	private:
		std::wstring pipe_path_;
		HANDLE handle_{INVALID_HANDLE_VALUE};
	};
}
