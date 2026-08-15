#include "scriptorium/manager/candidate_manager.h"

#include <algorithm>

#include "scriptorium/felt/core/logger/logger_service.h"

namespace scriptorium::inkstone::manager {
   void candidate_manager::update_state(const std::vector<core::candidate>& candidates, size_t highlight_index) {
       {
          std::unique_lock lock(mutex_);
          candidates_ = candidates;
          highlight_index_ = highlight_index;
       }

       felt::core::logger_service::logger()->info("UI Update: count={}, hl={}", candidates.size(), highlight_index);
       notify_observers();
   }

    const std::vector<core::candidate>& candidate_manager::get_candidates() const {
        std::shared_lock lock(mutex_);
        return candidates_;
    }

    void candidate_manager::add_observer(std::shared_ptr<core::icandidate_observer> observer) {
       std::unique_lock lock(mutex_);

       if (std::ranges::find(observers_, observer) == observers_.end()) {
          observers_.push_back(std::move(observer));
          felt::core::logger_service::logger()->debug("Observer added. Total: {}", observers_.size());
       }
    }

    void candidate_manager::remove_observer(const std::shared_ptr<core::icandidate_observer>& observer) {
       std::unique_lock lock(mutex_);

       const auto old_size = observers_.size();
       std::erase(observers_, observer);

       if (observers_.size() < old_size) {
           felt::core::logger_service::logger()->debug("Observer removed. Remaining: {}", observers_.size());
       }
    }

   void candidate_manager::notify_observers() const {
      std::vector<std::shared_ptr<core::icandidate_observer>> observers_copy;
      {
         std::shared_lock lock(mutex_);
         observers_copy = observers_;
      }

      std::vector<core::candidate> candidates_snapshot;
      {
         std::shared_lock lock(mutex_);
         candidates_snapshot = candidates_;
      }

      for (const auto& observer : observers_copy) {
         if (observer) {
            observer->on_candidate_update(candidates_snapshot, highlight_index_);
         }
      }
   }
}
