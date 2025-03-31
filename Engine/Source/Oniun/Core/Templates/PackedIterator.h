#pragma once

namespace Oniun
{
    template <typename T>
    class PackedIterator
    {
    public:
        using Type = T;

    public:
        FORCE_INLINE constexpr PackedIterator()
            : m_Ptr(nullptr)
        {
        }

        FORCE_INLINE constexpr PackedIterator(T* ptr)
            : m_Ptr(ptr)
        {
        }

        FORCE_INLINE constexpr PackedIterator(const PackedIterator& iter)
            : m_Ptr(iter.m_Ptr)
        {
        }

        FORCE_INLINE constexpr PackedIterator(PackedIterator&& iter)
            : m_Ptr(iter.m_Ptr)
        {
            iter.m_Ptr = nullptr;
        }

    public:
        FORCE_INLINE constexpr PackedIterator& operator=(const PackedIterator& iter)
        {
            m_Ptr = iter.m_Ptr;
            return *this;
        }

        FORCE_INLINE constexpr PackedIterator& operator=(PackedIterator&& iter)
        {
            m_Ptr = iter.m_Ptr;

            iter.m_Ptr = nullptr;
            return *this;
        }


        FORCE_INLINE constexpr T& operator*()
        {
            return *m_Ptr;
        }

        FORCE_INLINE constexpr const T& operator*() const
        {
            return *m_Ptr;
        }

        FORCE_INLINE constexpr T* operator->()
        {
            return m_Ptr;
        }

        FORCE_INLINE constexpr const T* operator->() const
        {
            return m_Ptr;
        }

        FORCE_INLINE constexpr bool operator==(const PackedIterator& iter) const
        {
            return m_Ptr == iter.m_Ptr;
        }

        FORCE_INLINE constexpr bool operator!=(const PackedIterator& iter) const
        {
            return m_Ptr != iter.m_Ptr;
        }

        FORCE_INLINE constexpr int64 operator-(const PackedIterator& iter) const
        {
            return m_Ptr - iter.m_Ptr;
        }

        FORCE_INLINE constexpr int64 operator+(const PackedIterator& iter) const
        {
            return m_Ptr + iter.m_Ptr;
        }

        FORCE_INLINE constexpr PackedIterator operator-(uint64 offset) const
        {
            return PackedIterator(m_Ptr - offset);
        }

        FORCE_INLINE constexpr PackedIterator operator+(uint64 offset) const
        {
            return PackedIterator(m_Ptr + offset);
        }

        friend FORCE_INLINE constexpr PackedIterator operator-(uint64 offset, const PackedIterator& self)
        {
            return PackedIterator(self.m_Ptr - offset);
        }

        friend FORCE_INLINE constexpr PackedIterator operator+(uint64 offset, const PackedIterator& self)
        {
            return PackedIterator(self.m_Ptr + offset);
        }

        FORCE_INLINE constexpr PackedIterator& operator++()
        {
            ++m_Ptr;
            return *this;
        }

        FORCE_INLINE constexpr PackedIterator& operator--()
        {
            --m_Ptr;
            return *this;
        }

    public:
        FORCE_INLINE constexpr T* Ptr()
        {
            return m_Ptr;
        }

        FORCE_INLINE constexpr const T* Ptr() const
        {
            return m_Ptr;
        }

    private:
        T* m_Ptr;
    };
}
