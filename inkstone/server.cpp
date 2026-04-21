#include "server.h"

#include <string>

#include "modian/core/logger/logger_service.h"
#include "modian/core/protocol/instruction.h"
#include "modian/service/input_protocol_service.h"
#include "modian/service/ui_protocol_service.h"

namespace modian::inkstone {
	const std::string INPUT_PROTOCOL_PIPE_NAME = R"(\\.\pipe\modian_input_protocol_pipe)";
	const std::string UI_PROTOCOL_PIPE_NAME = R"(\\.\pipe\modian_ui_protocol_pipe)";

	class ui_bridge : public core::candidate_observer {
	public:
		explicit ui_bridge(infra::ipc::ui_protocol_pipe_server* pipe)
			: pipe_(pipe) {}

		void on_candidate_update(const std::vector<std::string>& candidates, const size_t& highlight_index) override {
			if (!pipe_) return;

			bool visible = !candidates.empty();
			double x = 100.0;
			double y = 100.0;
			int page_index = 0;
			int total_pages = 1;

			std::string json_req = service::ui_protocol_service::build_render_state_request(
				visible, x, y, candidates, highlight_index, page_index, total_pages
			);

			pipe_->send(json_req);
		}

	private:
		infra::ipc::ui_protocol_pipe_server* pipe_;
	};

	server::server(const manager::EngineDetail& engine_detail) {
		auto engine_manager = std::make_shared<manager::engine_manager>();
		engine_manager->add_new_engine(engine_detail);
		candidate_manager_ = std::make_shared<manager::candidate_manager>();
		session_orchestrator_ = std::make_shared<manager::session_orchestrator>(
			candidate_manager_,
			engine_manager
		);
		input_protocol_pipe_ = std::make_unique<infra::ipc::input_protocol_pipe_server>(INPUT_PROTOCOL_PIPE_NAME);
		ui_protocol_pipe_ = std::make_unique<infra::ipc::ui_protocol_pipe_server>(UI_PROTOCOL_PIPE_NAME);
		ui_protocol_pipe_->set_on_message([this](std::string msg) {
			auto action = service::ui_protocol_service::parse_user_action_response(msg);

			if (action.type == core::protocol::ui::v1::action_type::SELECT_CANDIDATE) {
				size_t index = action.payload;

				core::logger_service::logger()->info("UI Selected Candidate: {}", index);

				const auto text = session_orchestrator_->select_candidate(index);
				if (!text.empty()) {
					   core::logger_service::logger()->info("TODO: Commit text to Brush: {}", text);
				}
			}
		});

		auto bridge = std::make_shared<ui_bridge>(ui_protocol_pipe_.get());
		candidate_manager_->add_observer(bridge);
	}

	void server::run() {
		input_protocol_pipe_->run([this](const std::string& request) {
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
