#pragma once

#include <optional>

#include "scriptorium/felt/core/ipc/isync_ipc_server.h"
#include "scriptorium/felt/core/protocol/v1/input/key_event.h"
#include "scriptorium/orchestrator/session_orchestrator.h"

namespace scriptorium::inkstone {
    class server {
    public:
        explicit server(const manager::EngineDetail& engine_detail);
        void run();
        void signal_stop();

    private:
        std::shared_ptr<manager::session_orchestrator> session_orchestrator_;
        std::optional<felt::core::protocol::input::v1::point> ui_anchor_;

        std::unique_ptr<felt::core::ipc::isync_ipc_server<std::string, std::string>> input_protocol_ipc_server_;

        std::mutex exit_mutex_;
        std::condition_variable exit_cv_;
        bool stop_requested_{false};
    };
}
