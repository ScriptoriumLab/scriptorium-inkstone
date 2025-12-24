#pragma once
#include "modian/manager/candidate_manager.h"
#include "modian/manager/engine_manager.h"

namespace modian::inkstone {
	class server {
	public:
		server();
		void run() const;

	private:
		std::shared_ptr<manager::candidate_manager> candidate_manager_;
		std::shared_ptr<manager::engine_manager> engine_manager_;

		static const int BUFFER_SIZE = 1024;
	};
}
