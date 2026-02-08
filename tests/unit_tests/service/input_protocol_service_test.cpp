#include "gtest/gtest.h"

#include "modian/service/input_protocol_service.h"

TEST(input_protocol_service_test, should_return_U_message_when_encode_is_update_protocol) {
	modian::inkstone::core::protocol::input::v1::instruction instruction { modian::inkstone::core::protocol::input::v1::message_type::UPDATE, "ni" };
	const auto message = modian::inkstone::service::input_protocol_service::build_instruction_response(instruction);
	ASSERT_EQ(message, "U:ni");
}

TEST(input_protocol_service_test, should_return_empty_string_when_payload_is_null) {
	modian::inkstone::core::protocol::input::v1::instruction instruction { modian::inkstone::core::protocol::input::v1::message_type::UPDATE, "" };
	const auto message = modian::inkstone::service::input_protocol_service::build_instruction_response(instruction);
	ASSERT_EQ(message, "");
}

TEST(input_protocol_service_test, should_return_C_message_when_encode_is_commit_protocol) {
	modian::inkstone::core::protocol::input::v1::instruction instruction { modian::inkstone::core::protocol::input::v1::message_type::COMMIT, "你" };
	const auto message = modian::inkstone::service::input_protocol_service::build_instruction_response(instruction);
	ASSERT_EQ(message, "C:你");
}
