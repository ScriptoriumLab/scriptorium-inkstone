#pragma once

#include <atomic>
#include <string>
#include <functional>

namespace modian::inkstone::infra::ipc {
	class named_pipe_server {
	public:
		using request_handler_t = std::function<std::string(const std::string&)>;

		explicit named_pipe_server(std::wstring_view pipe_name);
		~named_pipe_server();

		named_pipe_server(const named_pipe_server&) = delete;
		named_pipe_server& operator=(const named_pipe_server&) = delete;

		void run(request_handler_t handler);

		void stop();

	private:
		std::wstring pipe_name_;
		std::atomic<bool> running_{false};

		static const int BUFFER_SIZE = 1024;
	};
}