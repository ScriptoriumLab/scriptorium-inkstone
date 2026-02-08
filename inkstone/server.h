#pragma once

#include "modian/infra/ipc/named_pipe_server.h"
#include "modian/orchistrator/session_orchestrator.h"

namespace modian::inkstone {
	class server {
	public:
		explicit server(const manager::EngineDetail& engine_detail);
		void run();

		void signal_stop();

	private:
		std::shared_ptr<manager::session_orchestrator> session_orchestrator_;

		std::unique_ptr<infra::ipc::named_pipe_server> brush_pipe_;

		std::mutex exit_mutex_;
		std::condition_variable exit_cv_;
		bool stop_requested_{false};
	};
}
