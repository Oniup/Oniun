#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/Memory/Allocation.h"
#include "Oniun/Core/Templates/Pair.h"

namespace Oniun
{
    template <typename TKey, typename TValue, typename THash, typename TAllocationType>
    class HashMap
    {
    public:
        struct Bucket
        {
            bool Empty;
            Pair<TKey, TValue> Data;

            FORCE_INLINE void Free()
            {
                Memory::DestructItem(&Data.Key);
                Memory::DestructItem(&Data.Value);
                Empty = true;
            }

            FORCE_INLINE void Destroy()
            {
                if (!Empty)
                    Free();
            }

            FORCE_INLINE void Set(const TKey& key)
            {
                Memory::ConstructItem(&Data.Key, key);
                Memory::ConstructItem(&Data.Value);
                Empty = false;
            }

            FORCE_INLINE void Set(TKey&& key)
            {
                Memory::ConstructItem(&Data.Key, Memory::Move(key));
                Memory::ConstructItem(&Data.Value);
                Empty = false;
            }

            FORCE_INLINE void Set(const TKey& key, const TValue& value)
            {
                Memory::ConstructItem(&Data.Key, key);
                Memory::ConstructItem(&Data.Value, value);
                Empty = false;
            }

            FORCE_INLINE void Set(const TKey& key, TValue&& value)
            {
                Memory::ConstructItem(&Data.Key, key);
                Memory::ConstructItem(&Data.Value, Memory::Move(value));
                Empty = false;
            }

            FORCE_INLINE void Set(TKey&& key, TValue&& value)
            {
                Memory::ConstructItem(&Data.Key, Memory::Move(key));
                Memory::ConstructItem(&Data.Value, Memory::Move(value));
                Empty = false;
            }
        };

        class Iterator
        {
            friend HashMap;

        public:
            Iterator(HashMap* map, uint64 index)
                : m_Map(map), m_Index(index)
            {
            }

            Iterator(const HashMap* map, uint64 index)
                : m_Map(const_cast<HashMap*>(map)), m_Index(index)
            {
            }

            Iterator()
                : m_Map(nullptr), m_Index(NO_POS)
            {
            }

            Iterator(const Iterator& i)
                : m_Map(i.m_Map), m_Index(i.m_Index)
            {
            }

            Iterator(Iterator&& i)
                : m_Map(i.m_Map), m_Index(i.m_Index)
            {
            }

        public:
            FORCE_INLINE Pair<TKey, TValue>& operator*() const
            {
                return m_Map->m_Data[m_Index].Data;
            }

            FORCE_INLINE Pair<TKey, TValue>* operator->() const
            {
                return &m_Map->m_Data[m_Index].Data;
            }

            Iterator& operator=(const Iterator& iter)
            {
                m_Map = iter.m_Map;
                m_Index = iter.m_Index;
                return *this;
            }

            Iterator& operator=(Iterator&& iter)
            {
                m_Map = iter.m_Map;
                m_Index = iter.m_Index;
                return *this;
            }

            Iterator& operator++()
            {
                if (m_Index < m_Map->Capacity())
                {
                    do
                    {
                        ++m_Index;
                    }
                    while (m_Map->m_Data[m_Index].Empty && m_Index < m_Map->Capacity());
                }
                return *this;
            }

            Iterator operator++(int) const
            {
                Iterator iter(*this);
                ++iter;
                return iter;
            }

            Iterator& operator--()
            {
                do
                {
                    m_Index--;
                }
                while (m_Map->m_Data[m_Index].Empty && m_Index < m_Map->Capacity());
                return *this;
            }

            Iterator operator--(int) const
            {
                Iterator i(*this);
                --i;
                return i;
            }

            FORCE_INLINE bool operator==(const Iterator& iter) const
            {
                return m_Index == iter.m_Index && m_Map == iter.m_Map;
            }

            FORCE_INLINE bool operator!=(const Iterator& iter) const
            {
                return m_Index != iter.m_Index || m_Map != iter.m_Map;
            }

            FORCE_INLINE uint64 Index() const
            {
                return m_Index;
            }

            FORCE_INLINE Bucket* Ptr()
            {
                return &m_Map->m_Data[m_Index];
            }

            FORCE_INLINE const Bucket* Ptr() const
            {
                return &m_Map->m_Data[m_Index];
            }

        private:
            HashMap* m_Map;
            uint64 m_Index;
        };

        using Allocator = typename TAllocationType::template Data<Bucket>;
        using HashType = THash;
        using KeyType = TKey;
        using ValueType = TValue;

    public:
        HashMap()
            : m_Count(0)
        {
            m_Data.Allocate(DEFAULT_HASH_MAP_CAPACITY_COUNT);
            for (uint64 i = 0; i < DEFAULT_HASH_MAP_CAPACITY_COUNT; ++i)
                m_Data[i].Empty = true;
        }

        HashMap(uint64 capacity)
            : m_Count(0)
        {
            if (capacity > 0)
            {
                m_Data.Allocate(capacity);
                for (uint64 i = 0; i < capacity; ++i)
                    m_Data[i].Empty = true;
            }
        }

        HashMap(const HashMap& map)
            : m_Count(map.m_Count)
        {
            CopyBuffer(map);
        }

        HashMap(HashMap&& map)
            : m_Count(map.m_Count), m_Data(Memory::Move(map.m_Data))
        {
            map.m_Count = 0;
        }

        ~HashMap()
        {
            Clear();
        }

    public:
        HashMap& operator=(const HashMap& map)
        {
            Free();
            CopyBuffer(map);
            m_Count = map.m_Count;
            return *this;
        }

        HashMap& operator=(HashMap&& map)
        {
            Free();
            m_Data.Move(Memory::Move(map.m_Data));
            m_Count = map.m_Count;
            map.m_Count = 0;
            return *this;
        }

        FORCE_INLINE bool operator==(const HashMap& map) const
        {
            return Compare(map);
        }

        FORCE_INLINE bool operator!=(const HashMap& map) const
        {
            return !Compare(map);
        }

        FORCE_INLINE TValue& operator[](const TKey& key)
        {
            Bucket* bucket = GetBucket(key);
            if (!bucket)
            {
                bucket = &AddBucket(key);
                bucket->Set(key);
            }
            return bucket->Data.Value;
        }

        FORCE_INLINE const TValue& operator[](const TKey& key) const
        {
            return At(key);
        }

    public:
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

        FORCE_INLINE bool IsEmpty() const
        {
            return m_Count == 0;
        }

        FORCE_INLINE Iterator Begin()
        {
            Iterator iter(this, 0);
            if (m_Data[0].Empty)
                ++iter;
            return iter;
        }

        FORCE_INLINE Iterator Begin() const
        {
            Iterator iter(const_cast<HashMap*>(this), 0);
            if (m_Data[0].Empty)
                ++iter;
            return iter;
        }

        FORCE_INLINE Iterator End()
        {
            return Iterator(this, Capacity());
        }

        FORCE_INLINE Iterator End() const
        {
            return Iterator(const_cast<HashMap*>(this), Capacity());
        }

        FORCE_INLINE Iterator begin()
        {
            return Begin();
        }

        FORCE_INLINE Iterator begin() const
        {
            return Begin();
        }

        FORCE_INLINE Iterator end()
        {
            return End();
        }

        FORCE_INLINE Iterator end() const
        {
            return End();
        }

        bool Compare(const HashMap& map) const
        {
            if (m_Count != map.m_Count)
                return false;
            for (auto&[key, value] : *this)
            {
                if (map.Contains(key))
                    continue;
                return false;
            }
            return true;
        }

        bool Contains(const TKey& key)
        {
            Bucket* bucket = GetBucket(key);
            return bucket != nullptr;
        }

        TValue& At(const TKey& key)
        {
            Bucket* bucket = GetBucket(key);
            FORCE_ASSERT(bucket);
            return bucket->Data.Value;
        }

        const TValue& At(const TKey& key) const
        {
            Bucket* bucket = GetBucket(key);
            FORCE_ASSERT(bucket);
            return bucket->Data.Value;
        }

        Pair<TKey, TValue>& GetPair(const TKey& key)
        {
            Bucket* bucket = GetBucket(key);
            FORCE_ASSERT(bucket);
            return bucket->Data;
        }

        const Pair<TKey, TValue>& GetPair(const TKey& key) const
        {
            Bucket* bucket = GetBucket(key);
            FORCE_ASSERT(bucket);
            return bucket->Data;
        }

        TValue* Try(const TKey& key)
        {
            Bucket* bucket = GetBucket(key);
            if (bucket)
                return &bucket->Data.Value;
            return nullptr;
        }

        const TValue* Try(const TKey& key) const
        {
            Bucket* bucket = GetBucket(key);
            if (bucket)
                return &bucket->Data.Value;
            return nullptr;
        }

        Pair<TKey, TValue>* TryGetPair(const TKey& key)
        {
            Bucket* bucket = GetBucket(key);
            if (bucket)
                return &bucket->Data;
            return nullptr;
        }

        const Pair<TKey, TValue>* TryGetPair(const TKey& key) const
        {
            Bucket* bucket = GetBucket(key);
            if (bucket)
                return &bucket->Data;
            return nullptr;
        }

        Pair<TKey, TValue>& Add(const TKey& key)
        {
            Bucket& bucket = AddBucket(key);
            bucket.Set(key);
            return bucket.Data;
        }

        Pair<TKey, TValue>& Add(TKey&& key)
        {
            Bucket& bucket = AddBucket(key);
            bucket.Set(Memory::Move(key));
            return bucket.Data;
        }

        Pair<TKey, TValue>& Add(const TKey& key, const TValue& value)
        {
            Bucket& bucket = AddBucket(key);
            bucket.Set(key, value);
            return bucket.Data;
        }

        Pair<TKey, TValue>& Add(const TKey& key, TValue&& value)
        {
            Bucket& bucket = AddBucket(key);
            bucket.Set(key, Memory::Move(value));
            return bucket.Data;
        }

        Pair<TKey, TValue>& Add(TKey&& key, TValue&& value)
        {
            Bucket& bucket = AddBucket(key);
            bucket.Set(Memory::Move(key), Memory::Move(value));
            return bucket.Data;
        }

        bool Remove(const TKey& key)
        {
            Bucket* bucket = GetBucket(key);
            if (bucket)
            {
                bucket->Free();
                return true;
            }
            return false;
        }

        void Clear()
        {
            if (m_Count > 0)
            {
                for (uint64 i = 0; i < Capacity(); ++i)
                    m_Data[i].Destroy();
                m_Count = 0;
            }
        }

        void Free()
        {
            if (m_Count > 0)
            {
                for (uint64 i = 0; i < Capacity(); ++i)
                    m_Data[i].Destroy();
                m_Count = 0;
                m_Data.Free();
            }
        }

        void Copy(const HashMap& map)
        {
            Free();
            CopyBuffer();
            m_Count = map.m_Count;
        }

        void Reserve(uint64 capacity, bool preserveContents = true)
        {
            if (capacity <= Capacity())
                return;
            HashMap map(capacity);
            if (preserveContents)
            {
                for (Iterator iter = Begin(); iter != End(); ++iter)
                {
                    Bucket* bucket = iter.Ptr();
                    map.Add(Memory::Move(bucket->Data.Key), Memory::Move(bucket->Data.Value));
                }
            }
            else
                Clear();
            m_Data.Free();
            m_Data.Move(Memory::Move(map.m_Data));
        }

    private:
        FORCE_INLINE uint64 KeyToIndex(const TKey& key) const
        {
            const uint64 mask = Capacity() - 1;
            uint64 hash = THash().Get(key);
            if ((Capacity() & (mask)) == 0)
                return hash & (mask);
            return hash % mask;
        }

        FORCE_INLINE uint64 ProbeNext(uint64 index) const
        {
            const uint64 mask = Capacity() - 1;
            return (index + 1) & mask;
        }

        /// If count goes above 75% of the capacity
        FORCE_INLINE void GrowIfNeeded(uint64 count)
        {
            uint64 newCapacity = Capacity() != 0 ? Capacity() : 1;
            if (count * 4 > newCapacity * 3)
                Reserve(newCapacity * 2);
        }

        FORCE_INLINE Bucket& AddBucket(const TKey& key)
        {
            GrowIfNeeded(m_Count + 1);
            for (uint64 i = KeyToIndex(key);; i = ProbeNext(i))
            {
                if (i >= Capacity())
                {
                    Reserve(Capacity() * 2);
                    i = KeyToIndex(key);
                }
                Bucket& bucket = m_Data[i];
                if (bucket.Empty)
                {
                    ++m_Count;
                    return m_Data[i];
                }
            }
        }

        FORCE_INLINE const Bucket* GetBucket(const TKey& key) const
        {
            for (uint64 i = KeyToIndex(key);;  i = ProbeNext(i))
            {
                Bucket* bucket = &m_Data[i];
                if (!bucket->Empty)
                {
                    if (bucket->Data.Key == key)
                        return bucket;
                }
                else
                    return nullptr;
            }
        }

        FORCE_INLINE Bucket* GetBucket(const TKey& key)
        {
            for (uint64 i = KeyToIndex(key);;  i = ProbeNext(i))
            {
                Bucket* bucket = &m_Data[i];
                if (!bucket->Empty)
                {
                    if (bucket->Data.Key == key)
                        return bucket;
                }
                else
                    return nullptr;
            }
        }

        FORCE_INLINE void CopyBuffer(const HashMap& map)
        {
            m_Data.Allocate(map.Capacity());
            for (uint64 i = 0; i < map.Capacity(); ++i)
            {
                if (map.m_Data[i].Empty)
                    m_Data[i].Empty = true;
                else
                {
                    Bucket& to = m_Data[i];
                    const Bucket& from = map.m_Data[i];
                    to.Set(from.Data.Key, from.Data.Value);
                }
            }
        }

    private:
        uint64 m_Count;
        Allocator m_Data;
    };
}
