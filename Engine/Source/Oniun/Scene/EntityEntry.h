#pragma once

#include "Oniun/Core/Hash.h"
#include "Oniun/Core/UUID.h"
#include "Oniun/Core/Templates/Array.h"

namespace Oniun
{
    struct EntityEntry
    {
        static constexpr uint64 MaxNameSize = 30;
        static constexpr uint64 MaxFullNameSize = MaxNameSize + 32;
        using NameBuffer = FixedArray<char, MaxNameSize>;

        NameBuffer Name;
        uint64 NameId;

        uint64 Parent;
        uint64 FirstChild;
        uint64 Next;

        FORCE_INLINE UUID GetId() const
        {
            return Hash_Internal::FnvHash(Name.Data(), Name.Count()) + Hash<uint64>{}.Get(NameId);
        }

        FORCE_INLINE bool operator==(const EntityEntry& name) const
        {
            return Name == name.Name && NameId == name.NameId;
        }

        FORCE_INLINE bool operator!=(const EntityEntry& name) const
        {
            return Name != name.Name && NameId != name.NameId;
        }

        FORCE_INLINE bool operator==(const StringView& name) const
        {
            return Name.Data() == name.Data();
        }

        FORCE_INLINE bool operator!=(const StringView& name) const
        {
            return Name.Data() != name.Data();
        }
    };
}
