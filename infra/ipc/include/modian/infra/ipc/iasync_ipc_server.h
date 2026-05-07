#pragma once

#include <functional>

namespace modian::inkstone::infra::ipc {
    template<typename Req, typename Res>
    struct iasync_ipc_server {
        using message_handler_t = std::function<void(Res)>;

        virtual ~iasync_ipc_server() = default;

        virtual void send(const Req& message) = 0;
        virtual void set_message_handler(message_handler_t handler) = 0;
    };
}
