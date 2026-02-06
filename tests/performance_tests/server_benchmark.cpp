#include <benchmark/benchmark.h>
#include <thread>
#include <memory>

#include "../../inkstone/server.h"
#include "modian/core/logger/logger_service.h"
#include "modian/logger/spdlog_logger.h"
#include "include/modian/tests/utils/test_pipe_client.h"
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
    const std::string PIPE_NAME = R"(\\.\pipe\modian_ipc_brush)";
    std::unique_ptr<tests::utils::test_pipe_client> client;

    void SetUp(const ::benchmark::State& state) override {
        if (!server_instance) {
            core::logger_service::update_logger([]{
                return std::make_shared<modian::tests::performance_tests::mock_logger>();
            });

            const auto dict_path = std::filesystem::path(PROJECT_SOURCE_DIR) / "data" / "pinyin_dictionary.txt";
            auto dict_loader = modian::inkstone::core::lazy_load_dictionary<modian::inkstone::core::pinyin_engine>(dict_path.string());
            server_instance = std::make_unique<server>(dict_loader);
            server_thread = std::jthread([this] {
                server_instance->run();
            });

            std::this_thread::sleep_for(500ms);
        }

        client = std::make_unique<tests::utils::test_pipe_client>(PIPE_NAME);
        client->connect();
    }

    void TearDown(const ::benchmark::State& state) override {
        if (client) {
            client->close();
            client.reset();
        }
    }
};

std::unique_ptr<server> server_benchmark_fixture::server_instance = nullptr;
std::jthread server_benchmark_fixture::server_thread;

BENCHMARK_DEFINE_F(server_benchmark_fixture, BM_modian_input_method_performance)(benchmark::State& state) {
    if (!client->connect()) {
        state.SkipWithError("Failed to connect to server");
        return;
    }

    auto n = state.range(0);

    for (auto _ : state) {
        for (int i = 0; i < n; ++i) {
            client->send_and_receive("d");
            client->send_and_receive("i");
            client->send_and_receive("a");
            client->send_and_receive("n");

            client->send_and_receive("\b");
            client->send_and_receive("\b");
            client->send_and_receive("\b");
            client->send_and_receive("\b");
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