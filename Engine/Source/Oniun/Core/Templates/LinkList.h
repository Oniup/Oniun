#pragma once

#include <initializer_list>

namespace Oniun
{
    template <typename T>
    class LinkList
    {
    public:
        struct Node
        {
            Node* Next;
            Node* Previous;
            T Value;

            Node(Node* next, Node* prev, const T* value)
                : Next(next), Previous(prev), Value(value)
            {
            }

            Node(Node* next, Node* prev, T&& value)
                : Next(next), Previous(prev), Value(Memory::Move(value))
            {
            }

            Node* InsertNext(const T& value)
            {
                Node* node = Memory::New<Node>(Next, this, value);
                Next = node;
                return node;
            }

            Node* InsertNext(T&& value)
            {
                Node* node = Memory::New<Node>(Next, this, Memory::Move(value));
                Next = node;
                return node;
            }

            Node InsertNext(const Node* node)
            {
                Node* newNode = Memory::New<Node>(Next, this, node->Value);
                Next = newNode;
                return newNode;
            }

            Node* InsertPrevious(const T& value)
            {
                Node* node = Memory::New<Node>(this, Previous, value);
                Previous = node;
                return node;
            }

            Node* InsertPrevious(T&& value)
            {
                Node* node = Memory::New<Node>(this, Previous, Memory::Move(value));
                Previous = node;
                return node;
            }

            Node* InsertPrevious(const Node* node)
            {
                Node* newNode = Memory::New<Node>(this, Previous, node->Value);
                Previous = node;
                return newNode;
            }

            void Remove()
            {
                if (Next)
                    Next->Previous = Previous;
                if (Previous)
                    Previous->Next = Next;
                Memory::Delete(this);
            }

            T Pop()
            {
                if (Next)
                    Next->Previous = Previous;
                if (Previous)
                    Previous->Next = Next;
                T value(Memory::Move(Value));
                Memory::Delete(this);
                return value;
            }
        };

        class Iterator
        {
        public:
            FORCE_INLINE Iterator()
                : m_Ptr(nullptr)
            {
            }

            FORCE_INLINE Iterator(Node* node)
                : m_Ptr(node)
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

            FORCE_INLINE Iterator& operator=(Iterator&& iter)
            {
                m_Ptr = iter.m_Ptr;
                iter.m_Ptr = nullptr;
                return *this;
            }

            FORCE_INLINE T& operator*()
            {
                return m_Ptr->Value;
            }

            FORCE_INLINE const T& operator*() const
            {
                return m_Ptr->Value;
            }

            FORCE_INLINE T* operator->()
            {
                return &m_Ptr->Value;
            }

            FORCE_INLINE const T* operator->() const
            {
                return &m_Ptr->Value;
            }

            FORCE_INLINE bool operator==(const Iterator& iter) const
            {
                return m_Ptr == iter.m_Ptr;
            }

            FORCE_INLINE bool operator!=(const Iterator& iter) const
            {
                return m_Ptr != iter.m_Ptr;
            }

            FORCE_INLINE constexpr Iterator operator-(uint64 offset) const
            {
                Iterator iter(m_Ptr);
                for (uint64 i = 0; i < offset, m_Ptr; ++i)
                    iter.m_Ptr = m_Ptr->Previous;
                return iter;
            }

            FORCE_INLINE constexpr Iterator operator+(uint64 offset) const
            {
                Iterator iter(m_Ptr);
                for (uint64 i = 0; i < offset, m_Ptr; ++i)
                    iter.m_Ptr = m_Ptr->Next;
                return iter;
            }

            friend FORCE_INLINE constexpr Iterator operator-(uint64 offset, const Iterator& self)
            {
                return self - offset;
            }

            friend FORCE_INLINE constexpr Iterator operator+(uint64 offset, const Iterator& self)
            {
                return self + offset;
            }

            FORCE_INLINE Iterator operator++()
            {
                if (m_Ptr)
                    m_Ptr = m_Ptr->Next;
                return *this;
            }

            FORCE_INLINE Iterator operator--()
            {
                if (m_Ptr)
                    m_Ptr = m_Ptr->Previous;
                return *this;
            }

        public:
            FORCE_INLINE T* Ptr()
            {
                return m_Ptr;
            }

            FORCE_INLINE const T* Ptr() const
            {
                return m_Ptr;
            }

        private:
            Node* m_Ptr;
        };

    public:
        FORCE_INLINE LinkList()
            : m_Count(0), m_First(nullptr), m_Last(nullptr)
        {
        }

        LinkList(const std::initializer_list<T>& initList)
            : m_Count(initList.size()), m_First(nullptr), m_Last(nullptr)
        {
            Copy(initList);
        }

        LinkList(const LinkList& list)
            : m_Count(list.m_Count)
        {
            Copy(list);
        }

        LinkList(LinkList&& list)
            : m_Count(list.m_Count), m_First(list.m_First), m_Last(list.m_Last)
        {
            list.m_Count = 0;
            list.m_First = nullptr;
            list.m_Last = nullptr;
        }

        ~LinkList()
        {
            Free();
        }

        FORCE_INLINE LinkList& operator=(const LinkList& list)
        {
            Free();
            Copy(list);
            m_Count = list.m_Count;
            return *this;
        }

        FORCE_INLINE LinkList& operator=(const std::initializer_list<T>& initList)
        {
            Free();
            Copy(initList);
            m_Count = initList.size();
            return *this;
        }

        FORCE_INLINE LinkList& operator=(LinkList&& list)
        {
            Free();
            m_Count = list.m_Count;
            m_First = list.m_First;
            m_Last = list.m_Last;
            list.m_Count = 0;
            list.m_First = nullptr;
            list.m_Last = nullptr;
            return *this;
        }

        FORCE_INLINE uint64 Count() const
        {
            return m_Count;
        }

        FORCE_INLINE T& First()
        {
            return m_First->Value;
        }

        FORCE_INLINE const T& First() const
        {
            return m_First->Value;
        }

        FORCE_INLINE T& Last()
        {
            return m_Last->Value;
        }

        FORCE_INLINE const T& Last() const
        {
            return m_Last->Value;
        }

        FORCE_INLINE Iterator Begin()
        {
            return Iterator(m_First);
        }

        FORCE_INLINE Iterator Begin() const
        {
            return Iterator(const_cast<Node*>(m_First));
        }

        FORCE_INLINE Iterator End()
        {
            return Iterator(nullptr);
        }

        FORCE_INLINE Iterator End() const
        {
            return Iterator(const_cast<Node*>(nullptr));
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

        void AddLast(const T& value)
        {
            if (m_Last)
                m_Last = m_Last->InsertNext(value);
            else
            {
                m_First = Memory::New<Node>(nullptr, nullptr, value);
                m_Last = m_First;
            }
            ++m_Count;
        }

        void AddLast(T&& value)
        {
            if (m_Last)
                m_Last = m_Last->InsertNext(Memory::Move(value));
            else
            {
                m_First = Memory::New<Node>(nullptr, nullptr, Memory::Move(value));
                m_Last = m_First;
            }
            ++m_Count;
        }

        void AddFirst(const T& value)
        {
            if (m_First)
                m_First = m_First->InsertPrevious(value);
            else
            {
                m_First = Memory::New<Node>(nullptr, nullptr, value);
                m_Last = m_First;
            }
            ++m_Count;
        }

        void AddFirst(T&& value)
        {
            if (m_First)
                m_First = m_First->InsertPrevious(Memory::Move(value));
            else
            {
                m_First = Memory::New<Node>(nullptr, nullptr, value);
                m_Last = m_First;
            }
            ++m_Count;
        }

        void Remove(Node* node)
        {
            if (node)
            {
                if (node == m_First)
                    m_First = m_First->Next;
                else if (node == m_Last)
                    m_Last = m_Last->Previous;
                node->Remove();
                --m_Count;
            }
        }

        void RemoveAt(uint64 index)
        {
            ASSERT(index < m_Count);
            Node* node = m_First;
            for (uint64 i = 0; i < index; ++i)
                node = node->Next;
            node->Remove();
            --m_Count;
        }

        void RemoveLast()
        {
            if (m_Last)
            {
                Node* node = m_Last;
                m_Last = m_Last->Previous;
                node->Remove();
                --m_Count;
            }
        }

        void RemoveFirst()
        {
            if (m_First)
            {
                Node* node = m_First;
                m_First = m_First->Next;
                node->Remove();
                --m_Count;
            }
        }

        T PopAt(uint64 index)
        {
            ASSERT(index < m_Count);
            Node* node = m_First;
            for (uint64 i = 0; i < index; ++i)
                node = node->Next;
            --m_Count;
            return node->Pop();
        }

        void Free()
        {
            Node* node = m_First;
            while (node)
            {
                Memory::DestructItem(&node->Value);
                node = node->Next;
            }
            m_First = nullptr;
            m_Last = nullptr;
            m_Count = 0;
        }

    private:
        /// @warning Doesn't assign m_Count due the constructor using the initializer list to construct the count
        FORCE_INLINE void Copy(const LinkList& list)
        {
            Node* node = Memory::New<Node>(nullptr, nullptr, list.m_First->Value);
            m_First = node;
            m_Last = node;

            Node* copy = list.m_First->Next;
            for (uint64 i = 1; i < list.m_Count; ++i)
            {
                Node* next = Memory::New<Node>(nullptr, node, copy->Value);
                node->Next = next;
                node = next;
                copy = copy->Next;
            }
        }

        /// @warning Doesn't assign m_Count due the constructor using the initializer list to construct the count
        FORCE_INLINE void Copy(const std::initializer_list<T>& initList)
        {
            for (const T& val : initList)
            {
                Node* node = Memory::New<Node>(nullptr, m_Last, val);
                if (!m_First)
                {
                    m_First = node;
                    m_Last = node;
                    continue;
                }
                m_Last->Next = node;
                m_Last = node;
            }
        }

    private:
        uint64 m_Count;
        Node* m_First;
        Node* m_Last;
    };
}
