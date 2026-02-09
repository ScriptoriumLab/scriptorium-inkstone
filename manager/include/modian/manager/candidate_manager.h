#pragma once

#include <memory>
#include <vector>
#include <string>
#include <shared_mutex>

#include "modian/core/observer/candidate_observer.h"

namespace modian::inkstone::manager {
	class candidate_manager {
	public:
		candidate_manager() = default;
		~candidate_manager() = default;

		bool update_candidates(std::vector<std::string> candidates);
		void update_state(const std::vector<std::string>& candidates, size_t highlight_index);

		[[nodiscard]] const std::vector<std::string>& get_candidates() const;

		void add_observer(std::shared_ptr<core::candidate_observer> observer);
		void remove_observer(const std::shared_ptr<core::candidate_observer>& observer);
	private:
		std::vector<std::string> candidates_;
		size_t highlight_index_{0};

		std::vector<std::shared_ptr<core::candidate_observer>> observers_;

		mutable std::shared_mutex mutex_;

		void notify_observers() const;
	};
}