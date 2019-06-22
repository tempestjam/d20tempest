#include <cassert>

#include "threading/spin_lock.hpp"

namespace d20tempest::threading
{
    SpinLock::SpinLock() : m_readers(0), m_writer(0)
    {
    }

    void SpinLock::LockRead()
    {
        bool ended = false;
        while(!ended)
        {
            uint32_t required = 0;
            //Wait for possible writer to finish
            while (!m_writer.compare_exchange_strong(required, 0))
            {
                required = 0;
            }

            //Say we want to read
            m_readers.fetch_add(1);

            //Recheck in case the write flag has been set to 1 between the while and the add
            required = 0;
            ended = m_writer.compare_exchange_strong(required, 0);

            if(!ended)
            {
                //Revert reading flag
                m_readers.fetch_sub(1);
            }
        }
    }

    void SpinLock::UnlockRead()
    {
        assert(m_readers > 0);
        m_readers.fetch_sub(1);
    }

    void SpinLock::LockWrite()
    {
        uint32_t required = 0;
        while (!m_writer.compare_exchange_strong(required, 1))
        {
            required = 0;
        }

        //Wait for all the readers to finish reading
        while(m_readers.load() != 0);
    }

    void SpinLock::UnlockWrite()
    {
        if(m_writer == 0)
        {
            return;
        }

        assert(m_writer > 0);
        m_writer.store(0);
    }
} // namespace d20tempest::threading
