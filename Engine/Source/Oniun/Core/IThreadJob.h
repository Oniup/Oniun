#pragma once

#include "Oniun/Core/String/String.h"
#include "Oniun/Core/String/StringView.h"

namespace Oniun
{
    class IThreadJob
    {
    public:
        IThreadJob(const StringView& name)
            : m_Name(name)
        {
        }

        virtual ~IThreadJob()
        {
        }

    public:
        FORCE_INLINE StringView GetName() const
        {
            return m_Name;
        }

    public:
        virtual bool Init()
        {
            return true;
        }

        virtual void Stop()
        {
        }

        virtual void Exit()
        {
        }

        virtual int32 Run() = 0;

    private:
        String m_Name;
    };
}
