#pragma once

#include <vector>

#include "scriptorium/core/candidate/candidate.h"

namespace scriptorium::inkstone::core {
	class icandidate_observer {
	public:
        virtual void on_candidate_update(const std::vector<candidate>& candidates, size_t highlight_index) = 0;
        virtual ~icandidate_observer() = default;
	};
}
