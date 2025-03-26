#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Serialization/TypeInfo.h"

#define API_ENGINE_LAYER() \
    public: \
    FORCE_INLINE Oniun::uint64 GetId() override \
    { \
        return Oniun::TypeInfo::GetId<std::remove_reference_t<decltype(*this)>>(); \
    } \
    FORCE_INLINE Oniun::uint64 GetFastId() override \
    { \
        return Oniun::TypeInfo::GetFastId<std::remove_reference_t<decltype(*this)>>(); \
    } \
    FORCE_INLINE Oniun::TypeInfo GetTypeInfo() override \
    { \
        return Oniun::TypeInfo::GetInfo<std::remove_reference_t<decltype(*this)>>(); \
    } \
    private:

namespace Oniun
{
    class EngineLayer
    {
    public:
        virtual uint64 GetId() = 0;
        virtual uint64 GetFastId() = 0;
        virtual TypeInfo GetTypeInfo() = 0;

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
    };
}
