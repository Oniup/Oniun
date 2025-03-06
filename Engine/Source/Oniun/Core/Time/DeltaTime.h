#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/ClassConstraints.h"

class DeltaTime : public Singleton<DeltaTime>
{
    friend Singleton;

public:
    static void CalcDelta();
    static float GetElapsedTime();

    FORCE_INLINE static float GetTime()
    {
        return Instance()->m_Delta;
    }

private:
    DeltaTime();

private:
    float m_Last;
    float m_Delta;
};
