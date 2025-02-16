#pragma once

#include "Oniun/Core/DebugLogger.h"

namespace Onu
{
    class Context
    {
    public:
        Context();
        ~Context();

        DebugLogger* GetDebugLogger() { return &m_DebugLogger; }
        
    private:
        DebugLogger m_DebugLogger;
    };
}
