#include "server.h"

#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <modian/core/logger/logger_service.h>
#include <modian/infra/ipc/named_pipe_server.h>

#include "modian/core/engine/pinyin_engine.h"
#include "modian/core/utils/utils.h"

namespace modian::inkstone {
	const std::wstring PIPE_NAME = L"\\\\.\\pipe\\modian_ipc_pipe";

	server::server() {
		candidate_manager_ = std::make_shared<manager::candidate_manager>();
		engine_manager_ = std::make_shared<manager::engine_manager>(candidate_manager_);
		engine_manager_->add_new_engine(core::lazy_load_dictionary<core::pinyin_engine>());
	}

	void server::run() const {
		infra::ipc::named_pipe_server ipc_server{PIPE_NAME};
		ipc_server.run([this](const std::string& request) {
		   std::string response{""};

		   for (const auto& c : request) {
			  if (c == '\b') {
				 core::logger_service::logger()->info("[Recv] CMD: Backspace");
				 engine_manager_->handle_backspace();
			  } else {
				 core::logger_service::logger()->info("[Recv] Key: {}", c);
				 engine_manager_->update_input_state(c);
			  }
		   }

		   auto candidates = candidate_manager_->get_candidates();
		   if (!candidates.empty()) {
			   std::string candidate_text = core::utils::to_utf8(candidates[0]);

			   core::logger_service::logger()->info("[Commit Candidate]: {}", candidate_text);
			   response = candidate_text;

			   engine_manager_->reset();
		   }

		   return response;
		});
	}
}
