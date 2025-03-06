#pragma once

#include "Oniun/Core/Memory/Memory.h"

template <typename TKey, typename TValue>
struct Pair
{
    using KeyType = TKey;
    using ValueType = TValue;

    TKey Key;
    TValue Value;

    constexpr Pair()
    {
    }

    constexpr Pair(const TKey& key)
        : Key(key)
    {
    }

    constexpr Pair(const TKey& key, const TValue& value)
        : Key(key), Value(value)
    {
    }

    constexpr Pair(const TKey& key, TValue&& value)
        : Key(key), Value(Memory::Move(value))
    {
    }

    constexpr Pair(TKey&& key, const TValue& value)
        : Key(Memory::Move(key)), Value(value)
    {
    }

    constexpr Pair(TKey&& key, TValue&& value)
        : Key(Memory::Move(key)), Value(Memory::Move(value))
    {
    }

    constexpr ~Pair()
    {
    }

    constexpr bool operator==(const Pair& pair) const
    {
        return (Key == pair.Key) && (Value == pair.Value);
    }

    constexpr bool operator!=(const Pair& pair) const
    {
        return (Key != pair.Key) || (Value != pair.Value);
    }

    constexpr bool operator>=(const Pair& pair)
    {
        if (Key != pair.Key)
            return Key < pair.Key;
        return Value < pair.Value;
    }
};
