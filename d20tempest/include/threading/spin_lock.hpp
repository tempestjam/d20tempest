#pragma once

#include <atomic>

namespace d20tempest::threading
{
    class SpinLock
    {
    private:
        std::atomic_uint32_t m_readers;
        std::atomic_uint32_t m_writer;

    public:
        SpinLock();

        SpinLock(const SpinLock& other) = delete;
        SpinLock(SpinLock&& other) = delete;

        SpinLock& operator=(const SpinLock& rhs) = delete;
        SpinLock& operator=(SpinLock&& rhs) = delete;

        void LockRead();
        void UnlockRead();

        void LockWrite();
        void UnlockWrite();
    };
} // namespace d20tempest::threading
