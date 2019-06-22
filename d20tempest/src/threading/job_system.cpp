#include <thread>

#include "threading/job_system.hpp"

namespace d20tempest::threading
{
    uint32_t JobSystem::ms_numThreads = 0;                        
    std::queue<Job> JobSystem::ms_jobPool;  
    SpinLock JobSystem::ms_jobsLock;       
    std::condition_variable JobSystem::ms_wakeCondition;          
    std::mutex JobSystem::ms_wakeMutex;                           
    uint64_t JobSystem::ms_currentLabel = 0;                      
    std::atomic<uint64_t> JobSystem::ms_finishedLabel;      

    void JobSystem::Initialize()
    {
        // Initialize the worker execution state to 0:
        ms_finishedLabel.store(0);
    
        // Retrieve the number of hardware threads in this system:
        auto numCores = std::thread::hardware_concurrency();
    
        // Calculate the actual number of worker threads we want:
        ms_numThreads = std::max(1u, numCores);
    
        // Create all our worker threads while immediately starting them:
        for (uint32_t threadID = 0; threadID < ms_numThreads; ++threadID)
        {
            std::thread worker([] 
            {
                std::function<void()> job;

                while (true)
                {
                    if (JobSystem::ms_jobPool.size() > 0) // if there is jobs
                    {
                        JobSystem::ms_jobsLock.LockWrite();
                        if(JobSystem::ms_jobPool.size() == 0)
                        {
                            JobSystem::ms_jobsLock.UnlockWrite();
                            continue;
                        }
                        job = JobSystem::ms_jobPool.front();
                        JobSystem::ms_jobPool.pop();
                        JobSystem::ms_jobsLock.UnlockWrite();

                        job();
                        JobSystem::ms_finishedLabel.fetch_add(1); // update worker label state
                    }
                    else
                    {
                        // no job, put thread to sleep
                        std::unique_lock<std::mutex> lock(JobSystem::ms_wakeMutex);
                        JobSystem::ms_wakeCondition.wait(lock);
                    }
                }
    
            });
            
            // *****Here we could do platform specific thread setup...
            
            worker.detach(); // forget about this thread, let it do it's job in the infinite loop that we created above
        }
    }
    
    void JobSystem::Poll()
    {
        ms_wakeCondition.notify_one(); // wake one worker thread
        std::this_thread::yield(); // allow this thread to be rescheduled
    }   

    void JobSystem::Execute(const Job& job)
    {
        if(ms_numThreads == 0)
        {
            Initialize();
        }
        
        // The main thread label state is updated:
        ms_currentLabel += 1;
    
        // Try to push a new job until it is pushed successfully:
        //while (!ms_jobPool.push(job)) { Poll(); }
        JobSystem::ms_jobsLock.LockWrite();
        ms_jobPool.push(job);
        JobSystem::ms_jobsLock.UnlockWrite();
    
        ms_wakeCondition.notify_one(); // wake one thread
    }
    
    bool JobSystem::IsBusy()
    {
        // Whenever the main thread label is not reached by the workers, it indicates that some worker is still alive
        return ms_finishedLabel.load() < ms_currentLabel;
    }

    void JobSystem::Wait()
    {
        while (IsBusy()) { Poll(); }
    }
} // namespace d20tempest::threading
