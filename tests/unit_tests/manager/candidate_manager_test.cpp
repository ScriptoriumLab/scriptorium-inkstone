#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "modian/manager/candidate_manager.h"

using namespace modian::inkstone;

class mock_observer : public core::candidate_observer {
public:
	MOCK_METHOD(void, on_candidate_update, (const std::vector<std::string>&), (override));
};

TEST(candidate_manager_test, ShouldNotifyObserverOnUpdate) {
	const auto manager = std::make_shared<manager::candidate_manager>();
	const auto observer = std::make_shared<mock_observer>();

	manager->add_observer(observer);

	EXPECT_CALL(*observer, on_candidate_update(testing::ElementsAre("ni", "hao")))
		.Times(1);

	manager->update_candidates({"ni", "hao"});
}

class suicidal_observer : public core::candidate_observer, public std::enable_shared_from_this<suicidal_observer> {
public:
	std::weak_ptr<manager::candidate_manager> manager_weak_;
	int call_count_ = 0;

	void set_manager(const std::shared_ptr<manager::candidate_manager>& mgr) {
		manager_weak_ = mgr;
	}

	void on_candidate_update(const std::vector<std::string>& candidates) override {
		call_count_++;

		if (auto mgr = manager_weak_.lock()) {
			mgr->remove_observer(shared_from_this());
		}
	}
};

TEST(candidate_manager_test, ShouldNotCrashWhenObserverRemovesItself) {
	auto manager = std::make_shared<manager::candidate_manager>();
	auto suicide_observer = std::make_shared<suicidal_observer>();

	suicide_observer->set_manager(manager);
	manager->add_observer(suicide_observer);

	ASSERT_NO_THROW({
		manager->update_candidates({"ni", "hao"});
	});

	EXPECT_EQ(suicide_observer->call_count_, 1) << "Observer should be called once.";


	manager->update_candidates({"ceshi"});

	EXPECT_EQ(suicide_observer->call_count_, 1) << "Observer should NOT be called again after removal.";
}