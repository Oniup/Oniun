#pragma once

#include "Oniun/Core/ClassConstraints.h"
#include "Oniun/Core/IThreadJob.h"

namespace Oniun
{
    class IThread : public INonCopyable
    {
    public:
        IThread(IThreadJob* runnable);

        virtual ~IThread()
        {
        }

    public:
        FORCE_INLINE uint64 GetId() const
        {
            return m_Id;
        }

        FORCE_INLINE bool IsRunning() const
        {
            return m_IsRunning;
        }

    public:
        bool Start();
        void Join();
        void Kill();

        static uint64 GetCurrentThreadId() = delete;

    protected:
        virtual bool StartImpl() = 0;
        virtual void JoinImpl() = 0;
        virtual void KillImpl() = 0;

        int32 Run();
        static unsigned long ThreadProc(void* thisThread);

    protected:
        bool m_IsRunning;
        IThreadJob* m_Job;
        uint64 m_Id;
    };

    class IMutex
    {
    public:
        virtual ~IMutex()
        {
        }

        virtual bool Lock() = 0;
        virtual bool Unlock() = 0;
        virtual void Destroy() = 0;
    };
}
