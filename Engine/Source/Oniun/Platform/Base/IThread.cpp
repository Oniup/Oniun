#include "Oniun.pch.h"
#include "Oniun/Platform/Base/IThread.h"

#include "Oniun/Core/Logger.h"

namespace Oniun
{
    IThread::IThread(IThreadJob* runnable)
        : m_IsRunning(false), m_Job(runnable), m_Id(0)
    {
        DEBUG_ASSERT(m_Job);
    }

    bool IThread::Start()
    {
        if (m_Job)
            return StartImpl();
        return false;
    }

    void IThread::Join()
    {
        if (m_Job && m_IsRunning)
            JoinImpl();
    }

    void IThread::Kill()
    {
        if (m_Job && m_IsRunning)
            KillImpl();
    }

    int32 IThread::Run()
    {
        // TODO: Add to the thread registry layer
        m_IsRunning = true;
        uint32 exitCode = 1;

        LOG(Trace, "Thread \'{}\', ID: {} started", m_Job->GetName(), m_Id);
        if (m_Job->Init())
        {
            exitCode = m_Job->Run();
            m_Job->Exit();
        }
        LOG(Trace, "Thread \'{}\', ID: {} exited with code {}", m_Job->GetName(), m_Id, exitCode);
        m_IsRunning = false;

        // TODO: Remove from the thread registry layer
        return exitCode;
    }

    unsigned long IThread::ThreadProc(void* thisThread)
    {
        IThread* thread = (IThread*)thisThread;
        const int32 exitCode = thread->Run();
        return static_cast<unsigned long>(exitCode);
    }
}
