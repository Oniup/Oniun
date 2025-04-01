#pragma once

#include "Oniun/Core/Hash.h"
#include "Oniun/Core/Math.h"

namespace Oniun
{
    class UUID
    {
    public:
        FORCE_INLINE static UUID Generate()
        {
            return UUID(Math::RandomUInt64());
        }

    public:
        UUID()
            : m_Id(0)
        {
        }

        UUID(uint64 id)
            : m_Id(id)
        {
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
