#include "server.h"

#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <modian/core/logger/logger_service.h>
#include <modian/infra/ipc/named_pipe_server.h>

#include "modian/core/engine/pinyin_engine.h"
#include "modian/core/protocol/composition_protocol.h"
#include "modian/core/utils/utils.h"

namespace modian::inkstone {
	const std::wstring PIPE_NAME = L"\\\\.\\pipe\\modian_ipc_pipe";

	server::server(const manager::EngineDetail& engine_detail) {
		candidate_manager_ = std::make_shared<manager::candidate_manager>();
		engine_manager_ = std::make_shared<manager::engine_manager>(candidate_manager_);
		engine_manager_->add_new_engine(engine_detail);
	}

	void server::run() const {
		infra::ipc::named_pipe_server ipc_server{PIPE_NAME};
		ipc_server.run([this](const std::string& request) {
			std::string response;

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
				response = core::protocol::composition_protocol::create_commit(text).encode();

				core::logger_service::logger()->info("Decision: Commit '{}'", text);

				engine_manager_->reset();
			} else {
				std::string raw_pinyin = engine_manager_->get_current_raw_pinyin();

				response = core::protocol::composition_protocol::create_update(raw_pinyin).encode();

				core::logger_service::logger()->info("Decision: Update '{}'", raw_pinyin);
			}

			return response;
		});
	}
}
