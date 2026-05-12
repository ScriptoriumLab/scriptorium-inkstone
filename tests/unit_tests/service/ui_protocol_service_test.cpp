#include "gtest/gtest.h"

#include "modian/common/service/protocol/ui_protocol_service.h"

TEST(ui_protocol_service_test, should_successfully_build_render_state_request) {
	const std::vector<std::string> candidates{"你", "尼", "呢"};
	const std::string request = modian::common::service::ui_protocol_service::build_render_state_request(
		true, 100.0, 200.0, candidates, 0, 1, 3
	);

	const std::string expected_request = R"({"candidates":["你","尼","呢"],"highlight_index":0,"page_index":1,"total_pages":3,"visible":true,"x":100.0,"y":200.0})";
	ASSERT_EQ(request, expected_request);
}

TEST(ui_protocol_service_test, should_successfully_parse_select_candidate_action_response) {
	const std::string response = R"({"type":"SelectCandidate","payload":2})";
	const auto action = modian::common::service::ui_protocol_service::parse_user_action_response(response);

	ASSERT_EQ(action.type, modian::common::core::protocol::ui::v1::action_type::SELECT_CANDIDATE);
	ASSERT_EQ(action.payload, 2);
}
