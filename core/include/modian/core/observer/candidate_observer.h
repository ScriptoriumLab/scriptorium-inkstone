#pragma once

#include <vector>
#include <string>

namespace modian::inkstone::core {
	class candidate_observer {
	public:
        virtual void on_candidate_update(const std::vector<std::string>& candidates, const size_t& highlight_index) = 0;
    	virtual ~candidate_observer() = default;
	};
}