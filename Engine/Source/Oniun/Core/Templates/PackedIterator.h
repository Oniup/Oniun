#pragma once

namespace Onu
{
    template <typename T>
    class PackedIterator
    {
    public:
        using Type = T;
        
    private:
        T* m_Ptr;
        
    public:
        constexpr PackedIterator(T* ptr)
            : m_Ptr(ptr)
        {
        }

        constexpr PackedIterator()
            : m_Ptr(nullptr)
        {
        }

        constexpr PackedIterator(const PackedIterator& iterator)
            : m_Ptr(iterator.m_Ptr)
        {
        }

        constexpr PackedIterator(PackedIterator&& iterator)
            : m_Ptr(iterator.m_Ptr)
        {
            iterator.m_Ptr = nullptr;
        }

        constexpr T& operator*()
        {
            return *m_Ptr;
        }

        constexpr const T& operator*() const
        {
            return *m_Ptr;
        }

        constexpr T* operator->()
        {
            return m_Ptr;
        }

        constexpr const T* operator->() const
        {
            return m_Ptr;
        }

        constexpr bool operator==(const PackedIterator& iterator)
        {
            return m_Ptr == iterator.m_Ptr;
        }

        constexpr bool operator!=(const PackedIterator& iterator)
        {
            return m_Ptr != iterator.m_Ptr;
        }

        constexpr PackedIterator& operator=(const PackedIterator& iterator)
        {
            m_Ptr = iterator.m_Ptr;
            return *this;
        }

        constexpr PackedIterator& operator=(PackedIterator&& iterator)
        {
            m_Ptr = iterator.m_Ptr;

            iterator.m_Ptr = nullptr;
            return *this;
        }

        constexpr int64 operator-(const PackedIterator& iterator) const
        {
            return m_Ptr - iterator.m_Ptr;
        }
        
        constexpr int64 operator+(const PackedIterator& iterator) const
        {
            return m_Ptr + iterator.m_Ptr;
        }
        
        constexpr PackedIterator operator-(uint64 offset) const
        {
            return PackedIterator(m_Ptr - offset);
        }
        
        constexpr PackedIterator operator+(uint64 offset) const
        {
            return PackedIterator(m_Ptr + offset);
        }
        
        friend constexpr PackedIterator operator-(uint64 offset, const PackedIterator& self)
        {
            return PackedIterator(self.m_Ptr - offset);
        }
        
        friend constexpr PackedIterator operator+(uint64 offset, const PackedIterator& self)
        {
            return PackedIterator(self.m_Ptr + offset);
        }

        constexpr PackedIterator& operator++()
        {
            ++m_Ptr;
            return *this;
        }

        constexpr PackedIterator& operator--()
        {
            --m_Ptr;
            return *this;
        }

        constexpr T* Ptr()
        {
            return m_Ptr;
        }

        constexpr const T* Ptr() const
        {
            return m_Ptr;
        }
    };
}
