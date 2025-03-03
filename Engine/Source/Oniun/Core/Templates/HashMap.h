#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/Hash.h"
#include "Oniun/Core/Memory/Allocation.h"
#include "Oniun/Core/Templates/Pair.h"

template <typename TKey, typename TValue, typename THash, typename TAllocationType>
class HashMap
{
public:
    class Bucket
    {
        friend HashMap;

    public:
        TValue Value;

    private:
        TKey m_Key;
        bool m_IsEmpty;

    public:
        FORCE_INLINE bool IsEmpty() const
        {
            return m_IsEmpty;
        }

        FORCE_INLINE bool IsOccupied() const
        {
            return !m_IsEmpty;
        }

        FORCE_INLINE Bucket()
            : m_IsEmpty(true)
        {
        }

        FORCE_INLINE const TKey& GetKey() const
        {
            return m_Key;
        }

        FORCE_INLINE Pair<TKey*, TValue*> AsPair()
        {
            return Pair<TKey*, TValue*>(&m_Key, &Value);
        }

        FORCE_INLINE Pair<const TKey*, const TValue*> AsPair() const
        {
            return Pair<const TKey*, const TValue*>(&m_Key, &Value);
        }

    private:
        FORCE_INLINE void Free()
        {
            Memory::DestructItem(&m_Key);
            Memory::DestructItem(&Value);
            m_IsEmpty = true;
        }

        FORCE_INLINE void Destroy()
        {
            if (IsOccupied())
                Free();
        }

        FORCE_INLINE void Set(const TKey& key)
        {
            if (IsEmpty())
            {
                Memory::ConstructItem(&m_Key, key);
                Memory::ConstructItem(&Value);
                m_IsEmpty = false;
            }
        }

        FORCE_INLINE void Set(const TKey& key, const TValue& value)
        {
            if (IsEmpty())
            {
                Memory::ConstructItem(&m_Key, key);
                Memory::ConstructItem(&Value, value);
                m_IsEmpty = false;
            }
        }

        FORCE_INLINE void Set(const TKey& key, TValue&& value)
        {
            if (IsEmpty())
            {
                Memory::ConstructItem(&m_Key, key);
                Memory::MoveItems(&Value, &value, 1);
                m_IsEmpty = false;
            }
        }

        FORCE_INLINE void Set(TKey&& key, TValue&& value)
        {
            if (IsEmpty())
            {
                Memory::MoveItems(&m_Key, &key, 1);
                Memory::MoveItems(&Value, &value, 1);
                m_IsEmpty = false;
            }
        }

        FORCE_INLINE void Init()
        {
            m_IsEmpty = true;
        }
    };

    struct Iterator
    {
        friend HashMap;

    private:
        HashMap* m_Map;
        uint64 m_Index;

    public:
        Iterator(HashMap* map, const uint64 index)
            : m_Map(map), m_Index(index)
        {
        }

        Iterator(const HashMap* map, const uint64 index)
            : m_Map(const_cast<HashMap*>(map)), m_Index(index)
        {
        }

        Iterator()
            : m_Map(nullptr), m_Index(-1)
        {
        }

        Iterator(const Iterator& iterator)
            : m_Map(iterator.m_Map), m_Index(iterator.m_Index)
        {
        }

        Iterator(Iterator&& iterator)
            : m_Map(iterator.m_Map), m_Index(iterator.m_Index)
        {
        }

        FORCE_INLINE uint64 Index() const
        {
            return m_Index;
        }

        FORCE_INLINE Bucket& operator*() const
        {
            return m_Map->Data()[m_Index];
        }

        FORCE_INLINE Bucket* operator->() const
        {
            return &m_Map->m_Data[m_Index];
        }

        FORCE_INLINE bool operator==(const Iterator& iterator) const
        {
            return m_Index == iterator.m_Index && m_Map == iterator.m_Map;
        }

        FORCE_INLINE bool operator!=(const Iterator& iterator) const
        {
            return m_Index != iterator.m_Index || m_Map != iterator.m_Map;
        }

        Iterator& operator=(const Iterator& iterator)
        {
            m_Map = iterator.m_Map;
            m_Index = iterator.m_Index;
            return *this;
        }

        Iterator& operator=(Iterator&& iterator)
        {
            m_Map = iterator.m_Map;
            m_Index = iterator.m_Index;
            return *this;
        }

        Iterator& operator++()
        {
            do
            {
                ++m_Index;
            }
            while (m_Index != m_Map->Capacity() && m_Map->m_Data[m_Index].IsEmpty());
            return *this;
        }

        Iterator& operator--()
        {
            do
            {
                --m_Index;
            }
            while (m_Index > 0 && m_Map->m_Data[m_Index].IsEmpty());
            return *this;
        }
    };

    using KeyType = TKey;
    using ValueType = TValue;
    using HashFuncType = THash;
    using Allocator = typename TAllocationType::template Data<Bucket>;

private:
    uint64 m_Count;
    Allocator m_Data;

public:
    FORCE_INLINE HashMap()
        : HashMap(DEFAULT_HASH_MAP_CAPACITY_COUNT)
    {
    }

    HashMap(uint64 capacity)
        : m_Count(0)
    {
        if (capacity > 0)
            m_Data.Allocate(capacity);
        for (uint64 i = 0; i < capacity; ++i)
            m_Data[i].Init();
    }

    HashMap(HashMap&& other)
        : m_Count(other.m_Count), m_Data(Memory::Move(other.m_Data))
    {
        other.m_Count = 0;
    }

    HashMap(const HashMap& other)
        : m_Count(0)
    {
        Copy(other);
    }

    ~HashMap()
    {
        Free();
    }

    FORCE_INLINE TValue& operator[](const TKey& key)
    {
        TValue* result = TryGet(key);
        if (result)
            return *result;
        result = &Add(key);
        Memory::ConstructItem(result);
        return *result;
    }

    FORCE_INLINE const TValue& operator[](const TKey& key) const
    {
        return Get(GetHash(key));
    }

    FORCE_INLINE uint64 Count() const
    {
        return m_Count;
    }

    FORCE_INLINE uint64 Capacity() const
    {
        return m_Data.Capacity();
    }

    FORCE_INLINE Bucket* Data()
    {
        return m_Data.Ptr();
    }

    FORCE_INLINE const Bucket* Data() const
    {
        return m_Data.Ptr();
    }

    FORCE_INLINE Allocator& GetAllocator()
    {
        return m_Data;
    }

    FORCE_INLINE const Allocator& GetAllocator() const
    {
        return m_Data;
    }

    FORCE_INLINE bool IsEmpty()
    {
        return !m_Data.Ptr() || m_Count == 0;
    }

    FORCE_INLINE Iterator begin()
    {
        for (uint64 i = 0; i < Capacity(); ++i)
        {
            if (m_Data[i].IsOccupied())
                return Iterator(this, i);
        }
        return end();
    }

    FORCE_INLINE Iterator begin() const
    {
        for (uint64 i = 0; i < Capacity(); ++i)
        {
            if (m_Data[i].IsOccupied())
                return Iterator(this, i);
        }
        return end();
    }

    FORCE_INLINE Iterator end()
    {
        return Iterator(this, Capacity());
    }

    FORCE_INLINE Iterator end() const
    {
        return Iterator(this, Capacity());
    }

    FORCE_INLINE Iterator Begin()
    {
        return begin();
    }

    FORCE_INLINE Iterator Begin() const
    {
        return begin();
    }

    FORCE_INLINE Iterator End()
    {
        return end();
    }

    FORCE_INLINE Iterator End() const
    {
        return end();
    }

    void Clear()
    {
        if (!m_Data.Ptr())
            return;
        for (Bucket& bucket : *this)
        {
            bucket.Free();
            --m_Count;
        }
        ASSERT(!m_Count && "Not properly counting number of elements added");
    }

    void Free()
    {
        if (!m_Data.Ptr())
            return;
        for (Bucket& bucket : *this)
        {
            bucket.Free();
            --m_Count;
        }
        ASSERT(!m_Count && "Not properly counting number of elements added");
        m_Data.Free();
    }

    void Copy(const HashMap& other)
    {
        Clear();
        if (Capacity() < other.Capacity())
            Reserve(other.Capacity());
        for (const Bucket& bucket : other)
            Add(bucket.GetKey(), bucket.Value);
    }

    bool Contains(const TKey& key) const
    {
        return m_Data[GetHash(key)].IsOccupied();
    }

    TValue& Get(const TKey& key)
    {
        TValue* result = TryGet(key);
        ASSERT(result);
        return *result;
    }

    TValue& Get(const TKey& key) const
    {
        TValue* result = TryGet(key);
        ASSERT(result);
        return *result;
    }

    TValue* TryGet(const TKey& key)
    {
        uint64 hash = GetHash(key);
        if (m_Data[hash].IsOccupied())
            return &m_Data[hash].Value;
        return nullptr;
    }

    TValue* TryGet(const TKey& key) const
    {
        uint64 hash = GetHash(key);
        if (m_Data[hash].IsOccupied())
            return &m_Data[hash].Value;
        return nullptr;
    }

    TValue& Add(const TKey& key)
    {
        uint64 hash = GetHash(key);
        if (m_Data[hash].IsOccupied())
        {
            Reserve(Capacity() * 2);
            hash = GetHash(key);
        }
        m_Data[hash].Set(key);
        m_Count++;
        return m_Data[hash].Value;
    }

    TValue& Add(const TKey& key, const TValue& value)
    {
        uint64 hash = GetHash(key);
        if (m_Data[hash].IsOccupied())
        {
            Reserve(Capacity() * 2);
            hash = GetHash(key);
        }
        m_Data[hash].Set(key, value);
        m_Count++;
        return m_Data[hash].Value;
    }

    TValue& Add(const TKey& key, TValue&& value)
    {
        uint64 hash = GetHash(key);
        if (m_Data[hash].IsOccupied())
        {
            Reserve(Capacity() * 2);
            hash = GetHash(key);
        }
        m_Data[hash].Set(key, Memory::Move(value));
        m_Count++;
        return m_Data[hash].Value;
    }

    void Reserve(uint64 newCapacity)
    {
        if (newCapacity <= Capacity())
            return;
        Allocator newData;
        newData.Allocate(newCapacity);
        for (uint64 i = 0; i < newCapacity; ++i)
        {
            newData[i].Init();
        }
        if (m_Data.Ptr() && m_Count > 0)
        {
            for (Bucket& bucket : *this)
            {
                uint64 hash = THash{}.Get(bucket.m_Key) % newCapacity;
                newData[hash].Set(Memory::Move(bucket.m_Key), Memory::Move(bucket.Value));
                bucket.m_IsEmpty = true;
            }
            m_Data.Free();
        }
        m_Data.Move(Memory::Move(newData));
    }

private:
    FORCE_INLINE uint64 GetHash(const TKey& key) const
    {
        return THash{}.Get(key) % Capacity();
    }
};
