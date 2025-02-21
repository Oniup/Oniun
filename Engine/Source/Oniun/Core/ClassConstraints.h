#pragma once

namespace Onu
{
    class INonCopyable
    {
    public:
        INonCopyable() = default;

    private:
        INonCopyable(const INonCopyable& other) = delete;
        INonCopyable& operator=(INonCopyable& other) = delete;
    };

    struct INonMoveable
    {
    public:
        INonMoveable() = default;

    private:
        INonMoveable(INonMoveable&& other) = delete;
        INonMoveable& operator=(INonMoveable&& other) = delete;
    };

    template<typename T>
    class Singleton
    {
    private:
        Singleton(const Singleton& other) = delete;
        Singleton(Singleton&& other) = delete;

    protected:
        Singleton()
        {
        }

        ~Singleton()
        {
        }

    public:
        static T* Instance()
        {
            static T instance;
            return &instance;
        }
    };
}
