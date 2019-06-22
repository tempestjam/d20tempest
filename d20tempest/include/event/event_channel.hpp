#pragma once

#include <any>
#include <atomic>
#include <cstdint>
#include <functional>
#include <map>

#include "threading/spin_lock.hpp"
#include "threading/job_system.hpp"

namespace d20tempest::event
{
    enum class Events : uint64_t
    {
        TestEvent
    };

    using EventKey = uint64_t;

    template<Events event, typename Arg>
    class EventChannel
    {
    public:
        using EventListener = std::function<void(const Arg& arg)>;

        EventKey Register(const EventListener& listener)
        {
            if(listener != nullptr)
            {
                m_lock.LockWrite();
                ms_listeners.insert(std::make_pair(++ms_latest, std::make_any<EventListener>(listener)));
                m_lock.UnlockWrite();
                return ms_latest;
            }

            throw std::invalid_argument("The listenenr cannot be null");
        }

        void Unregister(EventKey listenerKey)
        {
            if(auto it = ms_listeners.find(listenerKey); it != ms_listeners.end())
            {
                m_lock.LockWrite();
                ms_listeners.erase(it);
                m_lock.UnlockWrite();   
            }
        }

        void Emit(const Arg& arg)
        {
            m_lock.LockRead();
            for (auto&[key, value] : ms_listeners)
            {
                if(!value.has_value())
                {
                    continue;
                }

                try
                {
                    EventListener listener = std::any_cast<EventListener>(value);
                    threading::JobSystem::Execute([listener, arg]()
                    {
                        listener(arg);  
                    }); 
                }
                catch(const std::exception& /*ex*/)
                {
                    //Silent the exception
                }
            }
            m_lock.UnlockRead();
        }

    private:
        static std::map<EventKey, std::any> ms_listeners;
        static threading::SpinLock m_lock;
        static EventKey ms_latest;
    };

    template<Events event, typename Arg> std::map<EventKey, std::any> EventChannel<event,Arg>::ms_listeners;
    template<Events event, typename Arg> threading::SpinLock EventChannel<event,Arg>::m_lock;
    template<Events event, typename Arg> EventKey EventChannel<event,Arg>::ms_latest = 0;
} // namespace d20tempest::event
