#include "gtest/gtest.h"

#include "modian/common/service/protocol/input_protocol_service.h"

TEST(input_protocol_service_test, should_return_U_message_when_build_update_response) {
	const modian::common::core::protocol::input::v1::instruction instruction { modian::common::core::protocol::input::v1::message_type::UPDATE, "ni" };
	const auto message = modian::common::service::input_protocol_service::build_instruction_response(instruction);
	ASSERT_EQ(message, "U:ni");
}

TEST(input_protocol_service_test, should_return_empty_string_when_build_response_with_null) {
	const modian::common::core::protocol::input::v1::instruction instruction { modian::common::core::protocol::input::v1::message_type::UPDATE, "" };
	const auto message = modian::common::service::input_protocol_service::build_instruction_response(instruction);
	ASSERT_EQ(message, "");
}

TEST(input_protocol_service_test, should_return_C_message_when_when_build_commit_response) {
	const modian::common::core::protocol::input::v1::instruction instruction { modian::common::core::protocol::input::v1::message_type::COMMIT, "你" };
	const auto message = modian::common::service::input_protocol_service::build_instruction_response(instruction);
	ASSERT_EQ(message, "C:你");
}
