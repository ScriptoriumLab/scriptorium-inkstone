#include "gtest/gtest.h"

#include "modian/core/protocol/composition_protocol.h"

TEST(composition_protocol_test, should_return_U_message_when_encode_is_update_protocol) {
	const auto message = modian::inkstone::core::protocol::composition_protocol::create_update("ni").encode();
	ASSERT_EQ(message, "U:ni");
}

TEST(composition_protocol_test, should_return_empty_string_when_payload_is_null) {
	const auto message = modian::inkstone::core::protocol::composition_protocol::create_update("").encode();
	ASSERT_EQ(message, "");
}

TEST(composition_protocol_test, should_return_C_message_when_encode_is_commit_protocol) {
	const auto message = modian::inkstone::core::protocol::composition_protocol::create_commit("你").encode();
	ASSERT_EQ(message, "C:你");
}