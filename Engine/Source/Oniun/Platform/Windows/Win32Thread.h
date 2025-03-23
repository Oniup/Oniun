#pragma once

#if ONU_PLATFORM_WINDOWS

#include "Oniun/Platform/Base/IThread.h"

namespace Oniun
{
    class Thread : public IThread
    {
    public:
        Thread(IThreadJob* job);
        ~Thread() override;

    public:
        static uint64 GetCurrentThreadId();

    protected:
        bool StartImpl() override;
        void JoinImpl() override;
        void KillImpl() override;

    private:
        void* m_Thread;
    };

    class Mutex : public IMutex
    {
    public:
        Mutex();
        ~Mutex() override;

        bool Lock() override;
        bool Unlock() override;
        void Destroy() override;

        FORCE_INLINE static Mutex& GetGlobal()
        {
            return m_GlobalMutex;
        }

    private:
        static Mutex m_GlobalMutex;

        void* m_Mutex;
    };
}

#endif
