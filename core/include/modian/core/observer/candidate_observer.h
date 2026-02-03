#pragma once

#include <vector>
#include <string>

namespace modian::inkstone::core {
	class candidate_observer {
	public:
        virtual void on_candidate_update(const std::vector<std::string>& candidates) = 0;
    	virtual ~candidate_observer() = default;
	};
}