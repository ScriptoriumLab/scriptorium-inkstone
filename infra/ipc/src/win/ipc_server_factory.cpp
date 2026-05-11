#include "modian/infra/ipc/ipc_server_factory.h"

#include <memory>

#include "modian/infra/ipc/win/sync_named_pipe_server.h"
#include "modian/infra/ipc/win/async_named_pipe_server.h"

namespace modian::inkstone::infra::ipc {
    std::unique_ptr<common::core::ipc::isync_ipc_server<std::string, std::string>> ipc_server_factory::create_sync_ipc_server(const std::string& name) {
        return std::make_unique<sync_named_pipe_server>(name);
    }

    std::unique_ptr<iasync_ipc_server<std::string, std::string>> ipc_server_factory::create_async_ipc_server(const std::string& name) {
        return std::make_unique<async_named_pipe_server>(name);
    }
}
