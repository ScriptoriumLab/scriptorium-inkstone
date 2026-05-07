#pragma once

#include <atomic>
#include <string>
#include <mutex>
#include <thread>

#include "modian/infra/ipc/isync_ipc_server.h"

namespace modian::inkstone::infra::ipc {
	class sync_named_pipe_server : public isync_ipc_server<std::string, std::string> {
	public:
		explicit sync_named_pipe_server(std::string_view pipe_name);
		sync_named_pipe_server(const sync_named_pipe_server&) = delete;
		sync_named_pipe_server& operator=(const sync_named_pipe_server&) = delete;

		~sync_named_pipe_server() override;

		void run(request_handler_t handler) override;

		void stop() override;

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
