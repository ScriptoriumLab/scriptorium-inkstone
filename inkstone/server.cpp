#include "server.h"

#include <memory>
#include <string>

#include "modian/common/core/observer/icandidate_observer.h"
#include "modian/common/core/logger/logger_service.h"
#include "modian/infra/ipc/ipc_server_factory.h"

#include "modian/common/service/protocol/input_protocol_service.h"
#include "modian/common/service/protocol/ui_protocol_service.h"

namespace modian::inkstone {
	const std::string INPUT_PROTOCOL_PIPE_NAME = R"(\\.\pipe\modian_input_protocol_pipe)";
	const std::string UI_PROTOCOL_PIPE_NAME = R"(\\.\pipe\modian_ui_protocol_pipe)";

	class ui_bridge : public common::core::icandidate_observer {
	public:
		explicit ui_bridge(std::unique_ptr<common::core::ipc::iasync_ipc_server<std::string, std::string>> pipe)
			: async_server_(std::move(pipe)) {}

		void on_candidate_update(const std::vector<std::string>& candidates, size_t highlight_index) override {
			if (!async_server_) return;

			bool visible = !candidates.empty();
			double x = 100.0;
			double y = 100.0;
			int page_index = 0;
			int total_pages = 1;

			std::string render_state = common::service::ui_protocol_service::build_render_state_request(
				visible, x, y, candidates, highlight_index, page_index, total_pages
			);

			async_server_->send(render_state);
		}

	private:
        std::unique_ptr<common::core::ipc::iasync_ipc_server<std::string, std::string>> async_server_;
	};

	server::server(const manager::EngineDetail& engine_detail) {
		input_protocol_ipc_server_ = infra::ipc::ipc_server_factory::create_sync_ipc_server(INPUT_PROTOCOL_PIPE_NAME);
		auto ui_protocol_ipc_server = infra::ipc::ipc_server_factory::create_async_ipc_server(UI_PROTOCOL_PIPE_NAME);
		ui_protocol_ipc_server->set_message_handler([this](std::string msg) {
			auto action = common::service::ui_protocol_service::parse_user_action_response(msg);

			if (action.type == common::core::protocol::ui::v1::action_type::SELECT_CANDIDATE) {
				size_t index = action.payload;

                common::core::logger_service::logger()->info("UI Selected Candidate: {}", index);

				const auto text = session_orchestrator_->select_candidate(index);
				if (!text.empty()) {
                    // TODO: need to solve async select candidates (i.e., clicking one candidate from UI need to notify inkstone and brush to replace with this candidate)
                    common::core::logger_service::logger()->info("TODO: Commit text to Brush: {}", text);
				}
			}
		});
		auto bridge = std::make_shared<ui_bridge>(std::move(ui_protocol_ipc_server));
		auto candidate_manager = std::make_unique<manager::candidate_manager>();
		candidate_manager->add_observer(bridge);

		auto engine_manager = std::make_unique<manager::engine_manager>();
		engine_manager->add_new_engine(engine_detail);

		session_orchestrator_ = std::make_shared<manager::session_orchestrator>(
			std::move(candidate_manager),
			std::move(engine_manager)
		);
	}

	void server::run() {
		input_protocol_ipc_server_->run([this](const std::string& request) {
			const auto key_event = common::service::input_protocol_service::parse_key_event_request(request);

			if (key_event.content == "cmd:shutdown") {
			   this->signal_stop();
			   return std::string("bye");
		    }

			const auto instruction = session_orchestrator_->handle_key(key_event);

			return common::service::input_protocol_service::build_instruction_response(instruction);
		});

        common::core::logger_service::logger()->info("Inkstone Server (Headless) running...");
		{
			std::unique_lock lock(exit_mutex_);
			exit_cv_.wait(lock, [this]{ return stop_requested_; });
		}
        common::core::logger_service::logger()->info("Stopping server...");
	}

	void server::signal_stop() {
		{
			std::lock_guard lock(exit_mutex_);
			stop_requested_ = true;
		}
		exit_cv_.notify_one();
	}
}
