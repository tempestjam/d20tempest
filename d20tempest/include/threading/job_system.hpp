/*
    A simple Job System largely inspired by the work at : 
    https://wickedengine.net/2018/11/24/simple-job-system-using-standard-c/
*/

#pragma once

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>
#include <queue>

#include "threading/spin_lock.hpp"

namespace d20tempest::threading
{
    using Job = std::function<void()>;

    class JobSystem
    {
    public:
        // Add a job to execute asynchronously. Any idle thread will execute this job.
        static void Execute(const Job& job);
    
        // Check if any threads are working currently or not
        static bool IsBusy();
    
        // Wait until all threads become idle
        static void Wait();

    private:
        static uint32_t ms_numThreads;
        static std::queue<Job> ms_jobPool;
        static SpinLock ms_jobsLock;
        static std::condition_variable ms_wakeCondition;
        static std::mutex ms_wakeMutex;
        // tracks the state of execution of the main thread
        static uint64_t ms_currentLabel;                          
        // tracks the state of execution across background worker threads
        static std::atomic<uint64_t> ms_finishedLabel;    


        // Create the internal resources such as worker threads, etc. Call it once when initializing the application.
        static void Initialize();     

        // This little helper function will not let the system to be deadlocked while the main thread is waiting for something
        static void Poll();
    };
} // namespace d20tempest::threading
