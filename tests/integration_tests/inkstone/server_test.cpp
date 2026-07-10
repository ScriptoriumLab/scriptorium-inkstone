#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <chrono>

#include "modian/core/engine/input_engine.h"
#include "modian/core/engine/pinyin_engine.h"

#include "modian/common/core/logger/logger_service.h"
#include "modian/common/infra/logger/spdlog_logger.h"
#include "../../inkstone/server.h"

#include "include/modian/tests/utils/test_input_protocol_pipe_client.h"
#include "include/modian/tests/utils/test_ui_protocol_pipe_client.h"

using namespace std::chrono_literals;

class server_integration_tests : public ::testing::Test {
protected:
	static void SetUpTestSuite() {
		modian::common::core::logger_service::update_logger([](){
			return std::make_shared<modian::common::infra::logger::spdlog_logger>("inkstone");
		});
	}

	void SetUp() override {
		auto dict_path = std::filesystem::path(PROJECT_SOURCE_DIR) / "data" / "pinyin_dictionary.txt";
		auto dict_loader = modian::inkstone::core::lazy_load_dictionary<modian::inkstone::core::pinyin_engine>(dict_path.string());

		server_instance_ = std::make_unique<modian::inkstone::server>(dict_loader);

		server_thread_ = std::jthread([this]() {
			server_instance_->run();
		});

		std::this_thread::sleep_for(200ms);
	}

	void TearDown() override {
		modian::common::core::logger_service::shutdown();

		if (server_instance_) {
			server_instance_->signal_stop();
		}
	}

	std::unique_ptr<modian::inkstone::server> server_instance_;
	std::jthread server_thread_;
	const std::string INPUT_PROTOCOL_PIPE_NAME = R"(\\.\pipe\modian_input_protocol_pipe)";
	const std::string UI_PROTOCOL_PIPE_NAME = R"(\\.\pipe\modian_ui_protocol_pipe)";
};

TEST_F(server_integration_tests, should_successfully_return_candidate_when_input_is_correct) {
	modian::inkstone::tests::utils::test_input_protocol_pipe_client brush_client(INPUT_PROTOCOL_PIPE_NAME);
	ASSERT_TRUE(brush_client.connect()) << "Failed to connect to server";

	modian::inkstone::tests::utils::test_ui_protocol_pipe_client ink_client(UI_PROTOCOL_PIPE_NAME);
	ASSERT_TRUE(ink_client.connect()) << "UI failed to connect";

	const auto response_without_candidates = brush_client.send_and_receive("n");
	EXPECT_THAT(response_without_candidates, testing::HasSubstr(R"({"candidate_info":{"payload":"n"},"type":"U"})"));
	ink_client.read_next_message();

	const auto response_with_candidates = brush_client.send_and_receive("i");
	EXPECT_THAT(response_with_candidates, testing::HasSubstr(R"({"candidate_info":{"payload":"ni"},"type":"U"})"));

	const auto json_msg = ink_client.read_next_message();
	EXPECT_THAT(json_msg, testing::HasSubstr("\"candidates\""));
	EXPECT_THAT(json_msg, testing::HasSubstr("visible\":true"));
	EXPECT_THAT(json_msg, testing::HasSubstr("你"));
}

TEST_F(server_integration_tests, should_successfully_handle_backspace_when_user_input_is_correct) {
	modian::inkstone::tests::utils::test_input_protocol_pipe_client brush_client(INPUT_PROTOCOL_PIPE_NAME);
	modian::inkstone::tests::utils::test_ui_protocol_pipe_client ink_client(UI_PROTOCOL_PIPE_NAME);
	ASSERT_TRUE(brush_client.connect());
	ASSERT_TRUE(ink_client.connect());

	brush_client.send_and_receive("d");
	ink_client.read_next_message();

	brush_client.send_and_receive("i");
	ink_client.read_next_message();

	brush_client.send_and_receive("a");
	ink_client.read_next_message();

	const auto response_after_backspace = brush_client.send_and_receive("cmd:backspace");
	ink_client.read_next_message();
	EXPECT_THAT(response_after_backspace, testing::HasSubstr(R"({"candidate_info":{"payload":"di"},"type":"U"})"));

	brush_client.send_and_receive("a");
	ink_client.read_next_message();

	const auto response_with_candidates = brush_client.send_and_receive("n");
	EXPECT_THAT(response_with_candidates, testing::HasSubstr(R"({"candidate_info":{"payload":"dian"},"type":"U"})"));

	const auto json_msg = ink_client.read_next_message();
	EXPECT_THAT(json_msg, testing::HasSubstr("\"candidates\""));
	EXPECT_THAT(json_msg, testing::HasSubstr("visible\":true"));
	EXPECT_THAT(json_msg, testing::HasSubstr("点"));
}
