#pragma once

#include <atomic>
#include <string>
#include <functional>
#include <mutex>
#include <thread>

namespace modian::inkstone::infra::ipc {
	class named_pipe_server {
	public:
		using request_handler_t = std::function<std::string(std::string_view)>;

		explicit named_pipe_server(std::string_view pipe_name);
		~named_pipe_server();

		named_pipe_server(const named_pipe_server&) = delete;
		named_pipe_server& operator=(const named_pipe_server&) = delete;

		void run(request_handler_t handler);

		void stop();

	private:
		void accept_loop(std::stop_token st);
		void handle_session(void* raw_handle, std::stop_token st) const;

		std::string pipe_name_;
		request_handler_t handler_;

		std::atomic<bool> running_{false};

		std::jthread accept_thread_;

		// TODO(future): current implementation will add a new jthread everytime there's a new connection. should consider using a thread pool to limit the max number of threads and reuse them.
		std::vector<std::jthread> client_threads_;
		std::mutex threads_mutex_;

		static constexpr int BUFFER_SIZE = 1024 * 16;
	};
}
