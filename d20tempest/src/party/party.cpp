#include <entt/entt.hpp>

#include "party/party.hpp"

namespace d20tempest::party
{
    class PartyImpl
    {
    public:
        PartyImpl(const std::string& name) : m_name(name)
        {
        }

        ~PartyImpl()
        {

        }

    private:
        entt::registry m_registry;
        std::string m_name;
    };

    Party::Party(const std::string& name)
    {
        m_impl = std::make_shared<PartyImpl>(name);
    }
    
    Party::~Party()
    {
        m_impl = nullptr;
    }
}