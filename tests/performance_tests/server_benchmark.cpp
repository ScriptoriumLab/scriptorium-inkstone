#include <benchmark/benchmark.h>
#include <thread>
#include <memory>

#include "../../inkstone/server.h"
#include "modian/core/logger/logger_service.h"
#include "include/modian/tests/utils/test_input_protocol_pipe_client.h"
#include "include/modian/tests/utils/test_ui_protocol_pipe_client.h"
#include "modian/core/engine/pinyin_engine.h"

using namespace modian::inkstone;

namespace modian::tests::performance_tests {
    class mock_logger : public core::base_logger {
    public:
        ~mock_logger() override = default;
        void sink_it(core::log_level level, std::string_view msg) override {}
        [[nodiscard]] std::string_view type() const override { return "mock_logger"; }
    };
}

class server_benchmark_fixture : public benchmark::Fixture {
public:
    static std::unique_ptr<server> server_instance;
    static std::jthread server_thread;
    static std::unique_ptr<tests::utils::test_input_protocol_pipe_client> brush_client;
    static std::unique_ptr<tests::utils::test_ui_protocol_pipe_client> ink_client;

    const std::string INPUT_PROTOCOL_PIPE_NAME = R"(\\.\pipe\modian_input_protocol_pipe)";
	const std::string UI_PROTOCOL_PIPE_NAME = R"(\\.\pipe\modian_ui_protocol_pipe)";

    void SetUp(const ::benchmark::State& state) override {
        if (!server_instance) {
            core::logger_service::update_logger([]{
                return std::make_shared<modian::tests::performance_tests::mock_logger>();
            });

            const auto dict_path = std::filesystem::path(PROJECT_SOURCE_DIR) / "data" / "pinyin_dictionary.txt";
            auto dict_loader = modian::inkstone::core::lazy_load_dictionary<core::pinyin_engine>(dict_path.string());
            server_instance = std::make_unique<server>(dict_loader);
            server_thread = std::jthread([this] {
                server_instance->run();
            });

            std::this_thread::sleep_for(500ms);

            brush_client = std::make_unique<tests::utils::test_input_protocol_pipe_client>(INPUT_PROTOCOL_PIPE_NAME);
            if (!brush_client->connect()) {
                throw std::runtime_error("Failed to connect brush client");
            }

            ink_client = std::make_unique<tests::utils::test_ui_protocol_pipe_client>(UI_PROTOCOL_PIPE_NAME);
            if (!ink_client->connect()) {
                throw std::runtime_error("Failed to connect ink client");
            }
        }
    }

    void TearDown(const ::benchmark::State& state) override {}
};

std::unique_ptr<server> server_benchmark_fixture::server_instance = nullptr;
std::jthread server_benchmark_fixture::server_thread;
std::unique_ptr<tests::utils::test_input_protocol_pipe_client> server_benchmark_fixture::brush_client = nullptr;
std::unique_ptr<tests::utils::test_ui_protocol_pipe_client> server_benchmark_fixture::ink_client = nullptr;

BENCHMARK_DEFINE_F(server_benchmark_fixture, BM_modian_input_method_performance)(benchmark::State& state) {
    auto n = state.range(0);

    for (auto _ : state) {
        for (int i = 0; i < n; ++i) {
            brush_client->send_and_receive("d");
            ink_client->read_next_message();

            brush_client->send_and_receive("i");
            ink_client->read_next_message();

            brush_client->send_and_receive("a");
            ink_client->read_next_message();

            brush_client->send_and_receive("n");
            ink_client->read_next_message();

            brush_client->send_and_receive("cmd:backspace");
            ink_client->read_next_message();

            brush_client->send_and_receive("cmd:backspace");
            ink_client->read_next_message();

            brush_client->send_and_receive("cmd:backspace");
            ink_client->read_next_message();

            brush_client->send_and_receive("cmd:backspace");
            ink_client->read_next_message();
        }
    }

    state.SetComplexityN(state.range(0));
}

BENCHMARK_REGISTER_F(server_benchmark_fixture, BM_modian_input_method_performance)
    ->RangeMultiplier(10)
    ->Range(1, 100000)
    ->Complexity(benchmark::oN)
    ->Unit(benchmark::kMicrosecond);

int main(int argc, char** argv) {
    ::benchmark::Initialize(&argc, argv);

    if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
    ::benchmark::RunSpecifiedBenchmarks();

    std::quick_exit(0);
}
