#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Serialization/TypeInfo.h"

#define API_ENGINE_LAYER() \
    public: \
    FORCE_INLINE uint64 GetId() override \
    { \
        return TypeInfo::GetId<std::remove_reference_t<decltype(*this)>>(); \
    } \
    FORCE_INLINE uint64 GetFastId() override \
    { \
        return TypeInfo::GetFastId<std::remove_reference_t<decltype(*this)>>(); \
    } \
    FORCE_INLINE TypeInfo GetTypeInfo() override \
    { \
        return TypeInfo::GetInfo<std::remove_reference_t<decltype(*this)>>(); \
    } \
    private:

class EngineLayer
{
public:
    virtual uint64 GetId() = 0;
    virtual uint64 GetFastId() = 0;
    virtual TypeInfo GetTypeInfo() = 0;

    virtual ~EngineLayer()
    {
    }

    virtual void OnStart()
    {
    }

    virtual void OnUpdate()
    {
    }
};
