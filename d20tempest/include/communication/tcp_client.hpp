#pragma once

#include <memory>

#include <gsl/gsl>
#include <uvw.hpp>

#include "communication/iclient.hpp"

namespace d20tempest::communication
{
    class TCPClientImpl;
    class TCPClient : public IClient
    {
    private:
        std::shared_ptr<TCPClientImpl> m_impl;
    public:
        TCPClient(gsl::not_null<uvw::TCPHandle*> sock, const uint64_t connectionID);
        ~TCPClient();

        virtual void OnMessage(ClientMessageHandler handler) override;
        virtual void OnLeave(ClientDisconnectedHandler handler) override;

        virtual void Send(std::vector<std::byte>& msg) override;
        virtual void Send(std::vector<std::byte>&& msg) override;

        uint64_t ConnectionId() const;
    };
}