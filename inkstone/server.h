#pragma once
#include "modian/manager/candidate_manager.h"
#include "modian/manager/engine_manager.h"

namespace modian::inkstone {
	class server {
	public:
		explicit server(const manager::EngineDetail& engine_detail);
		void run();

		void signal_stop();

	private:
		std::shared_ptr<manager::candidate_manager> candidate_manager_;
		std::shared_ptr<manager::engine_manager> engine_manager_;

		std::mutex exit_mutex_;
		std::condition_variable exit_cv_;
		bool stop_requested_{false};
	};
}
