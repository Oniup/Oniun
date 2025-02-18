#pragma once

#include "Oniun/Core/LogContext.h"

namespace Onu
{
    class Context
    {
    public:
        static LogContext* GetLogContext() { return &m_Instance->m_LogContext; }

        Context();
        ~Context();

    private:
        static Context* m_Instance;

        LogContext m_LogContext;
    };
}
