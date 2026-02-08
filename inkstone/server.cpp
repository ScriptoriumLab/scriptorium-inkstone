#include "server.h"

#include <string>

#include "modian/core/logger/logger_service.h"
#include "modian/core/protocol/instruction.h"
#include "modian/service/input_protocol_service.h"

namespace modian::inkstone {
	const std::string BRUSH_PIPE_NAME = R"(\\.\pipe\modian_ipc_brush)";

	server::server(const manager::EngineDetail& engine_detail) {
		auto candidate_manager = std::make_shared<manager::candidate_manager>();
		auto engine_manager = std::make_shared<manager::engine_manager>(candidate_manager);
		engine_manager->add_new_engine(engine_detail);
		session_orchestrator_ = std::make_shared<manager::session_orchestrator>(candidate_manager, engine_manager);
		brush_pipe_ = std::make_unique<infra::ipc::named_pipe_server>(BRUSH_PIPE_NAME);
	}

	void server::run() {
		brush_pipe_->run([this](const std::string& request) {
			const auto key_event = service::input_protocol_service::parse_key_event_request(request);

			if (key_event.content == "cmd:shutdown") {
			   this->signal_stop();
			   return std::string("bye");
		    }

			const auto instruction = session_orchestrator_->handle_key(key_event);

			return service::input_protocol_service::build_instruction_response(instruction);
		});

		core::logger_service::logger()->info("Inkstone Server (Headless) running...");
		{
			std::unique_lock lock(exit_mutex_);
			exit_cv_.wait(lock, [this]{ return stop_requested_; });
		}
		core::logger_service::logger()->info("Stopping server...");
	}

	void server::signal_stop() {
		{
			std::lock_guard lock(exit_mutex_);
			stop_requested_ = true;
		}
		exit_cv_.notify_one();
	}
}
