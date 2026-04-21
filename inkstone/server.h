#pragma once

#include "modian/infra/ipc/input_protocol_pipe_server.h"
#include "modian/infra/ipc/ui_protocol_pipe_server.h"
#include "modian/orchistrator/session_orchestrator.h"

namespace modian::inkstone {
	class server {
	public:
		explicit server(const manager::EngineDetail& engine_detail);
		void run();

		void signal_stop();

	private:
		std::shared_ptr<manager::candidate_manager> candidate_manager_;
		std::shared_ptr<manager::session_orchestrator> session_orchestrator_;

		std::unique_ptr<infra::ipc::input_protocol_pipe_server> input_protocol_pipe_;
		std::unique_ptr<infra::ipc::ui_protocol_pipe_server> ui_protocol_pipe_;

		std::mutex exit_mutex_;
		std::condition_variable exit_cv_;
		bool stop_requested_{false};
	};
}
