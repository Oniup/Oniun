#pragma once

#include "Oniun/Core/Hash.h"
#include "Oniun/Core/Math/Math.h"

namespace Oniun
{
    class UUID
    {
    public:
        UUID(uint64 id)
            : m_Id(id)
        {
        }

        UUID()
            : m_Id(Math::RandomUInt64())
        {
        }

        FORCE_INLINE void ReGenerate()
        {
            m_Id = Math::RandomUInt64();
        }

        FORCE_INLINE operator uint64() const
        {
            return m_Id;
        }

    private:
        uint64 m_Id;
    };

    template <>
    struct Hash<UUID>
    {
        FORCE_INLINE constexpr uint64 Get(const UUID& uuid) const
        {
            return Hash<uint64>{}.Get((uint64)uuid);
        }
    };
}
