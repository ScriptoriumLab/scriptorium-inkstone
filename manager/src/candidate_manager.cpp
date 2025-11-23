#include "modian/manager/candidate_manager.h"

#include <algorithm>

#include "modian/core/logger/logger_service.h"
#include "modian/core/utils/utils.h"

namespace modian::inkstone::manager {

    bool candidate_manager::update_candidates(std::vector<std::wstring> candidates) {
       std::unique_lock lock(mutex_);

       candidates_ = std::move(candidates);
       bool has_candidates = !candidates_.empty();

       lock.unlock();

       if (has_candidates) {
          std::shared_lock read_lock(mutex_);
          const auto count = candidates_.size();
          const auto first_candidate = count > 0 ? candidates_[0] : L"";
          read_lock.unlock(); // 读完解锁

          core::logger_service::logger()->info("Updated candidates: count={}, first='{}'",
                count,
                core::utils::to_utf8(first_candidate));

          notify_observers();
          return true;
       } else {
          core::logger_service::logger()->info("No candidates found.");
          notify_observers();
          return false;
       }
    }

    const std::vector<std::wstring>& candidate_manager::get_candidates() const {
        std::shared_lock lock(mutex_);
        return candidates_;
    }

    void candidate_manager::add_observer(std::shared_ptr<core::candidate_observer> observer) {
       std::unique_lock lock(mutex_);

       if (std::ranges::find(observers_, observer) == observers_.end()) {
          observers_.push_back(std::move(observer));
          core::logger_service::logger()->debug("Observer added. Total: {}", observers_.size());
       }
    }

    void candidate_manager::remove_observer(const std::shared_ptr<core::candidate_observer>& observer) {
       std::unique_lock lock(mutex_);

       const auto old_size = observers_.size();
       std::erase(observers_, observer);

       if (observers_.size() < old_size) {
           core::logger_service::logger()->debug("Observer removed. Remaining: {}", observers_.size());
       }
    }

    void candidate_manager::notify_observers() {
       std::vector<std::shared_ptr<core::candidate_observer>> observers_copy;
       {
           std::shared_lock lock(mutex_);
           observers_copy = observers_;
       }

       std::vector<std::wstring> candidates_snapshot;
       {
           std::shared_lock lock(mutex_);
           candidates_snapshot = candidates_;
       }

       for (const auto& observer : observers_copy) {
          if (observer) {
              observer->on_candidate_update(candidates_snapshot);
          }
       }
    }
}