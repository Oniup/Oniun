#pragma once

#include "Oniun/Core/Templates/Array.h"
#include "Oniun/Core/Templates/HashMap.h"
#include "Oniun/Core/Templates/LinkList.h"
#include "Oniun/Serialization/TypeInfo.h"
#include <functional>

/// Registry for storing components packed in an array
///
/// Memory order:
/// │ Entity 1                                       │  Entity 2
/// ├──────────┬──────────────────┬──────────────────┼──────────┬───┄┄
/// │ EntityId │ Comp Type 1 Data │ Comp Type 2 Data │ EntityId │ ...
/// └──────────┴──────────────────┴──────────────────┴──────────┴───┄┄
class ComponentRegistry
{
public:
    struct Type
    {
        uint64 Id;
        uint64 Size;
    };
};
