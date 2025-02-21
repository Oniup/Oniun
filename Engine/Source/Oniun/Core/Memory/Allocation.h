#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/Compiler.h"
#include "Oniun/Core/Memory/Crt.h"
#include "Oniun/Core/Memory/Memory.h"

namespace Onu
{
    struct HeapAllocation
    {
        template<typename T>
        class Data
        {
        public:
            using Type = T;

        private:
            uint64 m_Capacity;
            T* m_Data;

        public:
            FORCE_INLINE Data()
                : m_Capacity(0), m_Data(nullptr)
            {
            }

            FORCE_INLINE Data(Data&& allocator)
                : m_Capacity(allocator.m_Capacity), m_Data(allocator.m_Data)
            {
                allocator.m_Capacity = 0;
                allocator.m_Data = nullptr;
            }

            FORCE_INLINE ~Data()
            {
                if (m_Data)
                    Free();
            }

            FORCE_INLINE T& operator[](uint64 index)
            {
                return m_Data[index];
            }

            FORCE_INLINE const T& operator[](uint64 index) const
            {
                return m_Data[index];
            }

            FORCE_INLINE T* Ptr()
            {
                return m_Data;
            }

            FORCE_INLINE const T* Ptr() const
            {
                return m_Data;
            }

            FORCE_INLINE uint64 Capacity() const
            {
                return m_Capacity;
            }

            FORCE_INLINE void Allocate(uint64 capacity)
            {
                ASSERT(!m_Data);
                m_Data = static_cast<T*>(Crt::Allocate(capacity * sizeof(T)));
                m_Capacity = capacity;
                ASSERT(m_Data && "Out of memory");
            }

            FORCE_INLINE void Relocate(uint64 newCapacity, uint64 oldCount, uint64 newCount)
            {
                if (m_Data)
                {
                    T* newData = newCapacity != 0 ? static_cast<T*>(Crt::Allocate(newCapacity * sizeof(T))) : nullptr;
                    if (newCapacity != 0)
                        ASSERT(m_Data);

                    if (oldCount)
                    {
                        if (newCount > 0)
                            Memory::MoveItems(newData, m_Data, newCount);
                        Memory::DestructItems(m_Data, oldCount);
                    }

                    Free();
                    m_Data = newData;
                    m_Capacity = newCapacity;
                }
                else
                    Allocate(newCapacity);
            }

            FORCE_INLINE void Move(Data&& data)
            {
                m_Data = data.m_Data;
                m_Capacity = data.m_Capacity;

                data.m_Data = nullptr;
                data.m_Capacity = 0;
            }

            FORCE_INLINE void Free()
            {
                ASSERT(m_Data);
                Crt::Free(m_Data);
                m_Data = nullptr;
                m_Capacity = 0;
            }

            FORCE_INLINE void Swap(Data& data)
            {
                std::swap(m_Data, data.m_Data);
                std::swap(m_Capacity, data.m_Capacity);
            }
        };
    };

    template<uint64 TCapacity>
    struct FixedAllocation
    {
        template<typename T>
        class Data
        {
        public:
            using Type = T;

        private:
            T m_Data[TCapacity];

        public:
            FORCE_INLINE constexpr Data()
            {
            }

            FORCE_INLINE constexpr Data(Data&& allocator)
            {
            }

            FORCE_INLINE constexpr ~Data()
            {
            }

            FORCE_INLINE constexpr T& operator[](uint64 index)
            {
                return m_Data[index];
            }

            FORCE_INLINE constexpr const T& operator[](uint64 index) const
            {
                return m_Data[index];
            }

            FORCE_INLINE constexpr T* Ptr()
            {
                return m_Data;
            }

            FORCE_INLINE constexpr const T* Ptr() const
            {
                return m_Data;
            }

            FORCE_INLINE constexpr uint64 Capacity() const
            {
                return TCapacity;
            }

            FORCE_INLINE constexpr void Allocate(uint64 capacity)
            {
                ASSERT(capacity <= TCapacity);
            }

            FORCE_INLINE constexpr void Relocate(uint64 newCapacity, uint64 oldCount, uint64 newCount)
            {
                ASSERT(newCapacity <= TCapacity);
            }

            FORCE_INLINE constexpr void Move(Data&& data)
            {
            }

            FORCE_INLINE constexpr void Free()
            {
            }

            FORCE_INLINE constexpr void Swap(Data& data)
            {
                ASSERT(false && "Swapping fixed allocation is not supported");
            }
        };
    };
}
