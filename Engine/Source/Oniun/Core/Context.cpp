#include "Oniun/Core/Context.h"

namespace Onu
{
    Context* Context::m_Instance = nullptr;

    Context::Context()
    {
        m_Instance = this;
    }

    Context::~Context()
    {
        m_Instance = nullptr;
    }
}
