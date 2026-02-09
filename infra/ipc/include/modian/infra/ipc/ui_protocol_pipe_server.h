#pragma once

#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>

namespace modian::inkstone::infra::ipc {
	class ui_protocol_pipe_server {
	public:
		explicit ui_protocol_pipe_server(std::string_view pipe_name);
		~ui_protocol_pipe_server();

		void send(const std::string& data);

	private:
		void connection_loop(std::stop_token st);

		std::string pipe_name_;
		std::atomic<HANDLE> pipe_handle_{INVALID_HANDLE_VALUE};

		std::jthread worker_thread_;
		std::mutex send_mutex_;
	};

}