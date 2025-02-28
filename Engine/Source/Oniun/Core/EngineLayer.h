#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Serialization/TypeInfo.h"

#define API_ENGINE_LAYER() \
    public: \
    FORCE_INLINE uint64 GetId() override \
    { \
        return TypeInfo::GetId<decltype(*this)>(); \
    } \
    FORCE_INLINE TypeInfo GetTypeInfo() override \
    { \
        return TypeInfo::GetInfo<decltype(*this)>(); \
    } \
    private: \


class EngineLayer
{
public:
    virtual ~EngineLayer()
    {
    }

    virtual void OnStart()
    {
    }

    virtual void OnUpdate()
    {
    }

    virtual uint64 GetId() = 0;
    virtual TypeInfo GetTypeInfo() = 0;
};
