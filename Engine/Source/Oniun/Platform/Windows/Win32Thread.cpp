#if ONU_PLATFORM_WINDOWS

#include "Oniun/Platform/Windows/Win32Thread.h"

#include <Windows.h>

#include "Oniun/Core/Logger.h"


namespace Oniun
{
    Thread::Thread(IThreadJob* job)
        : IThread(job), m_Thread(nullptr)
    {
    }

    Thread::~Thread()
    {
        if (m_Thread)
        {
            WaitForSingleObject(m_Thread, INFINITE);
            CloseHandle(m_Thread);
        }
    }

    uint64 Thread::GetCurrentThreadId()
    {
        return ::GetCurrentThreadId();
    }

    bool Thread::StartImpl()
    {
        m_Thread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ThreadProc, this, 0, (DWORD*)&m_Id);
        return m_Thread != nullptr;
    }

    void Thread::JoinImpl()
    {
        WaitForSingleObject(m_Thread, INFINITE);
        CloseHandle(m_Thread);
        m_Thread = nullptr;
    }

    void Thread::KillImpl()
    {
        TerminateThread(m_Thread, 0);
    }

    Mutex::Mutex()
        : m_Mutex(CreateMutex(nullptr, 0, nullptr))
    {
        if (!m_Mutex)
            LOG(Error, "Unable to create mutex");
    }

    Mutex::~Mutex()
    {
        if (m_Mutex)
        {
            CloseHandle(m_Mutex);
            m_Mutex = nullptr;
        }
    }

    bool Mutex::Lock()
    {
        if (!m_Mutex)
            return false;

        DWORD result = WaitForSingleObject(m_Mutex, INFINITE);
        switch (result)
        {
        case WAIT_OBJECT_0:
            return true;
        case WAIT_ABANDONED:
            LOG(Error, "Mutex lock failed");
            return false;
        default:
            return true;
        }
    }

    bool Mutex::Unlock()
    {
        if (!m_Mutex)
            return false;
        return ReleaseMutex(m_Mutex);
    }

    void Mutex::Destroy()
    {
        if (m_Mutex)
        {
            CloseHandle(m_Mutex);
            m_Mutex = nullptr;
        }
    }
}

#endif
