#include <benchmark/benchmark.h>
#include <thread>
#include <memory>

#include "../../inkstone/server.h"
#include "scriptorium/felt/core/logger/logger_service.h"
#include "scriptorium/felt/core/protocol/v1/input/key_event.h"
#include "scriptorium/felt/service/protocol/input_protocol_service.h"
#include "include/scriptorium/tests/utils/test_input_protocol_pipe_client.h"
#include "include/scriptorium/tests/utils/test_ui_protocol_pipe_client.h"
#include "scriptorium/core/engine/pinyin_engine.h"

#define NOT_USED(x) (void)(x)

using namespace scriptorium::inkstone;

namespace scriptorium::tests::performance_tests {
    class mock_logger : public felt::core::base_logger {
    public:
        ~mock_logger() override = default;
        void sink_it(felt::core::log_level level, std::string_view msg) override {}
        [[nodiscard]] std::string_view type() const override { return "mock_logger"; }
    };
}

namespace {
    namespace input_protocol = scriptorium::felt::core::protocol::input::v1;
    namespace protocol_service = scriptorium::felt::service;

    std::string build_text_request(const std::string& content) {
        return protocol_service::input_protocol_service::build_key_event_request({
            .event = {
                input_protocol::key_event_type::TEXT,
                content
            },
            .context = {}
        });
    }

    std::string build_key_request(input_protocol::key_event_type type) {
        return protocol_service::input_protocol_service::build_key_event_request({
            .event = {
                type,
                std::nullopt
            },
            .context = {}
        });
    }
}

class server_benchmark_fixture : public benchmark::Fixture {
public:
    static std::unique_ptr<server> server_instance;
    static std::jthread server_thread;
    static std::unique_ptr<tests::utils::test_input_protocol_pipe_client> brush_client;
    static std::unique_ptr<tests::utils::test_ui_protocol_pipe_client> ink_client;

    const std::string INPUT_PROTOCOL_PIPE_NAME = R"(\\.\pipe\scriptorium_input_protocol_pipe)";
    const std::string UI_PROTOCOL_PIPE_NAME = R"(\\.\pipe\scriptorium_ui_protocol_pipe)";

    void SetUp(const ::benchmark::State& state) override {
        if (!server_instance) {
            scriptorium::felt::core::logger_service::update_logger([]{
                return std::make_shared<scriptorium::tests::performance_tests::mock_logger>();
            });

            const auto dict_path = std::filesystem::path(PROJECT_SOURCE_DIR) / "data" / "pinyin_dictionary.txt";
            auto dict_loader = scriptorium::inkstone::core::lazy_load_dictionary<core::pinyin_engine>(dict_path.string());
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

BENCHMARK_DEFINE_F(server_benchmark_fixture, BM_scriptorium_input_method_performance)(benchmark::State& state) {
    auto n = state.range(0);

    const auto input_f = build_text_request("f");
    const auto input_a = build_text_request("a");
    const auto input_n = build_text_request("n");
    const auto input_g = build_text_request("g");
    const auto backspace = build_key_request(input_protocol::key_event_type::BACKSPACE);

    for (auto _ : state) {
        for (int i = 0; i < n; ++i) {
            NOT_USED(brush_client->send_and_receive(input_f));
            ink_client->read_next_message();

            NOT_USED(brush_client->send_and_receive(input_a));
            ink_client->read_next_message();

            NOT_USED(brush_client->send_and_receive(input_n));
            ink_client->read_next_message();

            NOT_USED(brush_client->send_and_receive(input_g));
            ink_client->read_next_message();

            NOT_USED(brush_client->send_and_receive(backspace));
            ink_client->read_next_message();

            NOT_USED(brush_client->send_and_receive(backspace));
            ink_client->read_next_message();

            NOT_USED(brush_client->send_and_receive(backspace));
            ink_client->read_next_message();

            NOT_USED(brush_client->send_and_receive(backspace));
            ink_client->read_next_message();
        }
    }

    state.SetComplexityN(state.range(0));
}

BENCHMARK_REGISTER_F(server_benchmark_fixture, BM_scriptorium_input_method_performance)
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
