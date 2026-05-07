#pragma once

#include <functional>

namespace modian::inkstone::infra::ipc {
    template<typename Req, typename Res>
    struct isync_ipc_server {
        using request_handler_t = std::function<Res(Req)>;

        virtual ~isync_ipc_server() = default;

        virtual void run(request_handler_t handler) = 0;
        virtual void stop() = 0;
    };
}
