#pragma once

#include "Oniun/Core/BaseTypes.h"
#include "Oniun/Core/Memory/Memory.h"

template<typename TReturn, typename... TParams>
class Function<TReturn(TParams...)>
{
public:
    using Signature = TReturn(*)(TParams...);

private:
    using StubSignature = TReturn(*)(void* caller, TParams... params);

public:
    constexpr Function()
        : m_Caller(nullptr), m_Function(nullptr)
    {
    }

    constexpr Function(Signature signature)
        : m_Caller((void*)signature), m_Function(&FunctionPointer)
    {
    }

    constexpr Function(const Function& function)
        : m_Caller(function.m_Caller), m_Function(function.m_Function)
    {
    }

    constexpr Function(Function&& function)
        : m_Caller(function.m_Caller), m_Function(function.m_Function)
    {
        function.m_Caller = nullptr;
        function.m_Function = nullptr;
    }

    constexpr ~Function()
    {
    }

public:
    constexpr Function& operator=(const Function& function)
    {
        m_Caller = function.m_Caller;
        m_Function = function.m_Function;
        return *this;
    }

    constexpr Function& operator=(Function&& function)
    {
        m_Caller = function.m_Caller;
        m_Function = function.m_Function;
        function.m_Caller = nullptr;
        function.m_Function = nullptr;
        return *this;
    }

    constexpr Function& operator=(Signature signature)
    {
        m_Caller = (void*)signature;
        m_Function = &FunctionPointer;
        return *this;
    }

    FORCE_INLINE constexpr TReturn operator()(TParams... args) const
    {
        DEBUG_ASSERT(m_Function);
        return m_Function(m_Caller, Memory::Forward<TParams>(args)...);
    }

public:
    FORCE_INLINE constexpr bool IsBound() const
    {
        return m_Function != nullptr;
    }

    constexpr void Bind(Signature function)
    {
        m_Caller = function;
        m_Function = &FunctionPointer;
    }

    template <TReturn(*TMethod)(TParams...)>
    constexpr void Bind()
    {
        m_Caller = nullptr;
        m_Function = &StaticMethod<TMethod>;
    }

    template <class T, TReturn(T::*TMethod)(TParams...)>
    constexpr void Bind(T* caller)
    {
        m_Caller = caller;
        m_Function = &ClassMethod<T, TMethod>;
    }

    constexpr void Unbind()
    {
        m_Caller = nullptr;
        m_Function = nullptr;
    }

private:
    static constexpr TReturn FunctionPointer(void* caller, TParams... args)
    {
        return reinterpret_cast<Signature>(caller)(Memory::Forward<TParams>(args)...);
    }

    template <TReturn(*TMethod)(TParams...)>
    static constexpr TReturn StaticMethod(void*, TParams... args)
    {
        return (TMethod)(Memory::Forward<TParams>(args)...);
    }

    template <typename T, TReturn(T::*TMethod)(TParams...)>
    static constexpr TReturn ClassMethod(void* caller, TParams... args)
    {
        return (static_cast<T*>(caller)->*TMethod)(Memory::Forward<TParams>(args)...);
    }


    template <typename T, TReturn(T::*TMethod)(TParams...) const>
    static constexpr TReturn ClassMethod(void* caller, TParams... args)
    {
        return (static_cast<T*>(caller)->*TMethod)(Memory::Forward<TParams>(args)...);
    }

private:
    void* m_Caller;
    StubSignature m_Function;
};
