#pragma once

#include <atomic>
#include <string>
#include <functional>
#include <mutex>
#include <thread>

namespace modian::inkstone::infra::ipc {
    template<typename Req, typename Res>
    struct isync_ipc_server {
        using request_handler_t = std::function<Res(Req)>;

        virtual ~isync_ipc_server() = default;

        virtual void run(request_handler_t handler) = 0;
        virtual void stop() = 0;
    };

	class input_protocol_pipe_server : public isync_ipc_server<std::string, std::string> {
	public:
		explicit input_protocol_pipe_server(std::string_view pipe_name);
		input_protocol_pipe_server(const input_protocol_pipe_server&) = delete;
		input_protocol_pipe_server& operator=(const input_protocol_pipe_server&) = delete;

		~input_protocol_pipe_server() override;

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
