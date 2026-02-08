#include "server.h"

#include <windows.h>
#include <string>
#include <vector>
#include <modian/core/logger/logger_service.h>
#include <modian/infra/ipc/named_pipe_server.h>

#include "modian/core/protocol/instruction.h"

namespace modian::inkstone {
	const std::string BRUSH_PIPE_NAME = R"(\\.\pipe\modian_ipc_brush)";

	server::server(const manager::EngineDetail& engine_detail) {
		candidate_manager_ = std::make_shared<manager::candidate_manager>();
		engine_manager_ = std::make_shared<manager::engine_manager>(candidate_manager_);
		engine_manager_->add_new_engine(engine_detail);
		brush_pipe_ = std::make_unique<infra::ipc::named_pipe_server>(BRUSH_PIPE_NAME);
	}

	void server::run() {
		brush_pipe_->run([this](const std::string_view request) {
			std::string response;

			if (request == "cmd:shutdown") {
			   this->signal_stop();
			   return std::string("bye");
		   }

			for (const auto& c : request) {
				if (c == '\b') {
					core::logger_service::logger()->info("[Recv] CMD: Backspace");
					engine_manager_->handle_backspace();
				} else {
					core::logger_service::logger()->info("[Recv] Key: {}", c);
					char lower_c = std::tolower(static_cast<unsigned char>(c));
					engine_manager_->update_input_state(lower_c);
				}
			}

			auto candidates = candidate_manager_->get_candidates();
			if (!candidates.empty()) {
				std::string text = candidates[0];
				response = core::protocol::input::v1::instruction::create_commit(text).encode();

				core::logger_service::logger()->info("Decision: Commit '{}'", text);

				engine_manager_->reset();
			} else {
				std::string raw_pinyin = engine_manager_->get_current_raw_pinyin();

				response = core::protocol::input::v1::instruction::create_update(raw_pinyin).encode();

				core::logger_service::logger()->info("Decision: Update '{}'", raw_pinyin);
			}

			return response;
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
