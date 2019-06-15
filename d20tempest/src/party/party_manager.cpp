#include <map>

#include "party/party.hpp" 
#include "party/party_manager.hpp"

namespace d20tempest::party
{
    class PartyManagerImpl
    {
    private:
        std::map<uint64_t, Party> m_parties;

    public:
        PartyManagerImpl()
        {

        }

        ~PartyManagerImpl()
        {

        }

        void JoinParty(uint64_t id)
        {

        }

        uint64_t CreateParty(const std::string& name)
        {
            return 0;
        }
    };
    
} // namespace d20tempest::party
