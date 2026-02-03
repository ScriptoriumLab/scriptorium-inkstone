#pragma once
#include "modian/manager/candidate_manager.h"
#include "modian/manager/engine_manager.h"

namespace modian::inkstone {
	class server {
	public:
		explicit server(const manager::EngineDetail& engine_detail);
		void run() const;

	private:
		std::shared_ptr<manager::candidate_manager> candidate_manager_;
		std::shared_ptr<manager::engine_manager> engine_manager_;
	};
}
