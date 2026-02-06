#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <chrono>

#include "modian/core/engine/input_engine.h"
#include "modian/core/engine/pinyin_engine.h"

#include "modian/core/logger/logger_service.h"
#include "modian/logger/spdlog_logger.h"
#include "../../inkstone/server.h"

#include "include/modian/tests/utils/test_pipe_client.h"

using namespace std::chrono_literals;

class server_integration_tests : public ::testing::Test {
protected:
	static void SetUpTestSuite() {
		modian::inkstone::core::logger_service::update_logger([](){
			return std::make_shared<modian::inkstone::infra::logger::spdlog_logger>();
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
		modian::inkstone::core::logger_service::shutdown();

		if (server_instance_) {
			server_instance_->signal_stop();
		}
	}

	std::unique_ptr<modian::inkstone::server> server_instance_;
	std::jthread server_thread_;
	const std::string PIPE_NAME = R"(\\.\pipe\modian_ipc_brush)";
};

TEST_F(server_integration_tests, should_successfully_return_candidate_when_input_is_correct) {
	modian::inkstone::tests::utils::test_pipe_client client(PIPE_NAME);
	ASSERT_TRUE(client.connect()) << "Failed to connect to server";

	const auto response_without_candidates = client.send_and_receive("n");
	EXPECT_THAT(response_without_candidates, testing::HasSubstr("U:n"));

	const auto response_with_candidates = client.send_and_receive("i");
	EXPECT_THAT(response_with_candidates, testing::HasSubstr("C:"));
	EXPECT_THAT(response_with_candidates, testing::HasSubstr("你"));
}

TEST_F(server_integration_tests, should_successfully_handle_backspace_when_user_input_is_correct) {
	modian::inkstone::tests::utils::test_pipe_client client(PIPE_NAME);
	ASSERT_TRUE(client.connect());

	client.send_and_receive("d");
	client.send_and_receive("i");
	client.send_and_receive("a");
	const auto response_after_backspace = client.send_and_receive("\b");
	EXPECT_THAT(response_after_backspace, testing::HasSubstr("U:di"));

	client.send_and_receive("a");
	const auto response_with_candidates = client.send_and_receive("n");
	EXPECT_THAT(response_with_candidates, testing::HasSubstr("点"));

	const auto empty_text = client.send_and_receive("\b");
	ASSERT_TRUE(empty_text.empty());
}