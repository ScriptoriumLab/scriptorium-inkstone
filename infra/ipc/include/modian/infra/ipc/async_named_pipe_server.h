#pragma once

#include <functional>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>

namespace modian::inkstone::infra::ipc {
    template<typename Req, typename Res>
    struct iasync_ipc_server {
        using message_handler_t = std::function<void(Res)>;

        virtual ~iasync_ipc_server() = default;

        virtual void send(const Req& message) = 0;
        virtual void set_message_handler(message_handler_t handler) = 0;
    };

	class async_named_pipe_server : public iasync_ipc_server<std::string, std::string> {
	public:
		using message_handler_t = std::function<void(std::string)>;

		explicit async_named_pipe_server(std::string_view pipe_name);
		~async_named_pipe_server() override;

		void send(const std::string& data) override;

		void set_message_handler(message_handler_t handler) override {
			message_handler_ = std::move(handler);
		}
	private:
		void connection_loop(std::stop_token st);
		void read_loop(HANDLE h, const std::stop_token& st) const;

		std::string pipe_name_;
		std::atomic<HANDLE> pipe_handle_{INVALID_HANDLE_VALUE};

		std::jthread worker_thread_;
		std::mutex send_mutex_;

		message_handler_t message_handler_;
	};

}
