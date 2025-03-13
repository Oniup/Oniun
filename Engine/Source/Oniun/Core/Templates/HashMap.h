#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/Hash.h"
#include "Oniun/Core/Memory/Allocation.h"
#include "Oniun/Core/Templates/Pair.h"

template<typename TKey, typename TValue, typename THash, typename TAllocationType>
class HashMap
{
public:
    struct Element
    {
        bool Filled;
        Element* Next;
        Element* Previous;
        Pair<TKey, TValue> Data;
    };

    class Iterator
    {
    public:
        FORCE_INLINE Iterator()
            : m_Ptr(nullptr)
        {
        }

        FORCE_INLINE Iterator(Element* ptr)
            : m_Ptr(ptr)
        {
        }

        FORCE_INLINE Iterator(const Iterator& iter)
            : m_Ptr(iter.m_Ptr)
        {
        }

        FORCE_INLINE Iterator(Iterator&& iter)
            : m_Ptr(iter.m_Ptr)
        {
            iter.m_Ptr = nullptr;
        }

    public:
        FORCE_INLINE Iterator& operator=(const Iterator& iter)
        {
            m_Ptr = iter.m_Ptr;
            return *this;
        }

        FORCE_INLINE Iterator& operator=(Iterator& iter)
        {
            m_Ptr = iter.m_Ptr;
            iter.m_Ptr = nullptr;
            return *this;
        }

        FORCE_INLINE Pair<TKey, TValue>& operator*()
        {
            return m_Ptr->Data;
        }

        FORCE_INLINE const Pair<TKey, TValue>& operator*() const
        {
            return m_Ptr->Data;
        }

        FORCE_INLINE Pair<TKey, TValue>* operator->()
        {
            return &m_Ptr->Data;
        }

        FORCE_INLINE const Pair<TKey, TValue>* operator->() const
        {
            return &m_Ptr->Data;
        }

        FORCE_INLINE bool operator==(const Iterator& iter) const
        {
            return m_Ptr == iter.m_Ptr;
        }

        FORCE_INLINE bool operator!=(const Iterator& iter) const
        {
            return m_Ptr != iter.m_Ptr;
        }

        FORCE_INLINE Iterator operator+(uint64 offset) const
        {
            Iterator iter(*this);
            for (uint64 i = 0; i < offset, m_Ptr; ++i)
                iter.m_Ptr = iter.m_Ptr->Next;
            return iter;
        }

        FORCE_INLINE Iterator operator-(uint64 offset) const
        {
            Iterator iter(*this);
            for (uint64 i = 0; i < offset, m_Ptr; ++i)
                iter.m_Ptr = iter.m_Ptr->Previous;
            return iter;
        }

        FORCE_INLINE Iterator& operator++()
        {
            m_Ptr = m_Ptr->Next;
            return *this;
        }

        FORCE_INLINE Iterator& operator--()
        {
            m_Ptr = m_Ptr->Previous;
            return *this;
        }

    public:
        FORCE_INLINE Element* Ptr()
        {
            return m_Ptr;
        }

        FORCE_INLINE const Element* Ptr() const
        {
            return m_Ptr;
        }

    private:
        Element* m_Ptr;
    };

public:
    using KeyType = TKey;
    using ValueType = TValue;
    using Allocator = typename TAllocationType::template Data<Element>;

public:
    HashMap()
        : HashMap(DEFAULT_HASH_MAP_CAPACITY_COUNT)
    {
    }

    HashMap(uint64 capacity)
        : m_Count(0), m_Head(nullptr), m_Tail(nullptr)
    {
        Reserve(capacity);
    }

    ~HashMap()
    {
        for (auto& [key, value] : *this)
        {
            Memory::DestructItem(&key);
            Memory::DestructItem(&value);
        }
    }

public:
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
        TValue* val = TryGet(key);
        if (val)
            return *val;
        return Add(key);
    }

    FORCE_INLINE const TValue& operator[](const TKey& key) const
    {
        return Get(key);
    }

    FORCE_INLINE uint64 Count() const
    {
        return m_Count;
    }

    FORCE_INLINE uint64 Capacity() const
    {
        return m_Data.Capacity();
    }

    FORCE_INLINE Element* Data()
    {
        return m_Data.Ptr();
    }

    FORCE_INLINE const Element* Data() const
    {
        return m_Data.Ptr();
    }

    FORCE_INLINE Iterator Begin()
    {
        return Iterator(m_Head);
    }

    FORCE_INLINE Iterator Begin() const
    {
        return Iterator(const_cast<Element*>(m_Head));
    }

    FORCE_INLINE Iterator End()
    {
        return Iterator(nullptr);
    }

    FORCE_INLINE Iterator End() const
    {
        return Iterator(const_cast<Element*>(nullptr));
    }


    FORCE_INLINE Iterator Last()
    {
        return Iterator(m_Tail);
    }

    FORCE_INLINE Iterator Last() const
    {
        return Iterator(const_cast<Element*>(m_Tail));
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

    bool Contains(const TKey& key)
    {
        uint64 hash = GetHash(key);
        return m_Data[hash].Filled;
    }

    TValue& Get(const TKey& key)
    {
        uint64 hash = GetHash(key);
        Element& element = m_Data[hash];
        ASSERT(element.Filled);
        return element.Data.Value;
    }

    const TValue& Get(const TKey& key) const
    {
        uint64 hash = GetHash(key);
        Element& element = m_Data[hash];
        ASSERT(element.Filled);
        return element.Data.Value;
    }

    TValue* TryGet(const TKey& key)
    {
        uint64 hash = GetHash(key);
        Element& element = m_Data[hash];
        if (element.Filled)
            return &element.Data.Value;
        return nullptr;
    }

    const TValue* TryGet(const TKey& key) const
    {
        uint64 hash = GetHash(key);
        Element& element = m_Data[hash];
        if (element.Filled)
            return &element.Data.Value;
        return nullptr;
    }

    bool Compare(const HashMap& map) const
    {
        if (m_Count != map.m_Count)
            return false;
        for (auto&[key, value] : *this)
        {
            uint64 hash = map.GetHash(key);
            Element* compare = map.m_Data[hash];
            if (key != compare->Data.Key || value != compare->Data.Value)
                return false;
        }
        return true;
    }

    TValue& Add(const TKey& key)
    {
        uint64 hash = GetHash(key);
        while (m_Data[hash].Filled)
            Reserve(Capacity() * 2);

        Element& current = m_Data[hash];
        Memory::ConstructItem(&current.Data.Key, key);
        Memory::ConstructItem(&current.Data.Value);
        current.Filled = true;

        m_Count++;
        SetPosition(&current);
        return current.Data.Value;
    }

    TValue& Add(const TKey& key, const TValue& val)
    {
        uint64 hash = GetHash(key);
        while (m_Data[hash].Filled)
            Reserve(Capacity() * 2);

        Element& current = m_Data[hash];
        Memory::ConstructItem(&current.Data.Key, key);
        Memory::ConstructItem(&current.Data.Value, val);
        current.Filled = true;

        m_Count++;
        SetPosition(&current);
        return current.Data.Value;
    }

    TValue& Add(const TKey& key, TValue&& val)
    {
        uint64 hash = GetHash(key);
        while (m_Data[hash].Filled)
            Reserve(Capacity() * 2);

        Element& current = m_Data[hash];
        Memory::ConstructItem(&current.Data.Key, key);
        Memory::ConstructItemArgs(&current.Data.Value, Memory::Move(val));
        current.Filled = true;

        m_Count++;
        SetPosition(&current);
        return current.Data.Value;
    }

    void Remove(const TKey& key)
    {
        uint64 hash = GetHash(key);
        Element& element = m_Data[hash];
        if (element.Filled)
        {
            Memory::DestructItem(&element.Data.Key);
            Memory::DestructItem(&element.Data.Value);

            if (element.Previous)
                element.Previous->Next = element.Next;
            if (element.Next)
                element.Next->Previous = element.Previous;
            element.Next = nullptr;
            element.Previous = nullptr;
            element.Filled = false;
            m_Count--;

            if (m_Count == 0)
            {
                m_Head = nullptr;
                m_Tail = nullptr;
            }
        }
    }

    TValue PopAt(const TKey& key)
    {
        uint64 hash = GetHash(key);
        Element& element = m_Data[hash];
        ASSERT(element.Filled)

        TValue val(Memory::Move(element.Data.Value));
        Memory::DestructItem(&element.Data.Key);

        element.Previous->Next = element.Next;
        element.Next->Previous = element.Previous;
        element.Next = nullptr;
        element.Previous = nullptr;
        element.Filled = false;

        m_Count--;
        return val;
    }

    void Clear()
    {
        Element* element = m_Head;
        while (element)
        {
            Memory::DestructItem(&element->Data.Key);
            Memory::DestructItem(&element->Data.Value);
            element->Filled = false;

            Element* previous = element;
            element = element->Next;
            previous->Previous = nullptr;
            previous->Next = nullptr;
        }
        m_Count = 0;
    }

    void Copy(const HashMap& map)
    {
        Clear();
        if (Capacity() < map.Capacity())
            Reserve(map.Capacity());
        for (const auto&[key, value] : map)
            Add(key, value);
    }

    void Reserve(uint64 newCapacity)
    {
        if (newCapacity <= Capacity())
            return;

        Allocator newData;
        newData.Allocate(newCapacity);
        for (uint64 i = 0; i < newCapacity; ++i)
            newData[i].Filled = false;

        if (m_Data.Ptr() && m_Count > 0)
        {
            bool first = true;
            Element* previous = nullptr;
            Element* element = m_Head;
            while (element != nullptr)
            {
                uint64 hash = THash{}.Get(element->Data.Key) % newCapacity;
                Element* current = &newData[hash];
                if (first)
                {
                    m_Head = current;
                    first = false;
                }
                Memory::ConstructItemArgs(&current->Data.Key, Memory::Move(element->Data.Key));
                Memory::ConstructItemArgs(&current->Data.Value, Memory::Move(element->Data.Value));
                current->Filled = true;

                newData[hash].Previous = previous;
                if (previous)
                    previous->Next = current;
                previous = current;

                element = element->Next;
            }
            m_Tail = previous;
            m_Tail->Next = nullptr;
            m_Data.Free();
        }
        m_Data.Move(Memory::Move(newData));
    }

private:
    FORCE_INLINE uint64 GetHash(const TKey& key) const
    {
        return Hash<TKey>{}.Get(key) % Capacity();
    }

    FORCE_INLINE void SetPosition(Element* element)
    {
        if (!m_Head)
        {
            m_Head = element;
            m_Tail = element;
            element->Next = nullptr;
            element->Previous = nullptr;
            return;
        }
        element->Previous = m_Tail;
        element->Next = nullptr;
        m_Tail->Next = element;
        m_Tail = element;
    }

private:
    uint64 m_Count;
    Element* m_Head;
    Element* m_Tail;
    Allocator m_Data;
};
