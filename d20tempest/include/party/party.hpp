#pragma once

#include <memory>

namespace d20tempest::party
{
    class PartyImpl;
    class Party
    {
    private:
        std::shared_ptr<PartyImpl> m_impl;
    public:
        Party(const std::string& name);
        ~Party();
    };
} // namespace name
