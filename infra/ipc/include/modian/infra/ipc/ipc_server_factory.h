#pragma once

#include <string>
#include <memory>

#include "modian/infra/ipc/isync_ipc_server.h"
#include "modian/infra/ipc/iasync_ipc_server.h"

namespace modian::inkstone::infra::ipc {
    class ipc_server_factory {
    public:
        static std::unique_ptr<isync_ipc_server<std::string, std::string>> create_sync_ipc_server(const std::string& name);

        static std::unique_ptr<iasync_ipc_server<std::string, std::string>> create_async_ipc_server(const std::string& name);
    };
}
