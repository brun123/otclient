/*
 * Copyright (c) 2010-2011 OTClient <https://github.com/edubart/otclient>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef LUABINDER_H
#define LUABINDER_H

// this file is and must be included only from luainterface.h
#include "luainterface.h"
#include "luaexception.h"

/// This namespace contains some dirty metaprogamming that uses a lot of C++0x features
/// The purpose here is to create templates that can bind any function from C++
/// and expose in lua environment. This is done combining variadic templates,
/// lambdas, tuples and some type traits features from the new C++0x standard to create
/// templates that can detect functions's arguments and then generate lambdas. These lambdas
/// pops arguments from lua stack, call the bound C++ function and then
/// pushes the result to lua.
namespace luabinder
{
    /// Removes const references, transforming 'const T&' into 'T'
    template<typename T>
    struct remove_const_ref {
        typedef typename std::remove_const<typename std::remove_reference<T>::type>::type type;
    };

    /// Pack arguments from lua stack into a tuple recursively
    template<int N>
    struct pack_values_into_tuple {
        template<typename Tuple>
        static void call(Tuple& tuple, LuaInterface* lua) {
            typedef typename std::tuple_element<N-1, Tuple>::type ValueType;
            std::get<N-1>(tuple) = lua->polymorphicPop<ValueType>();
            pack_values_into_tuple<N-1>::call(tuple, lua);
        }
    };
    template<>
    struct pack_values_into_tuple<0> {
        template<typename Tuple>
        static void call(Tuple &tuple, LuaInterface* lua) { }
    };

    /// C++ function caller that can push results to lua
    template<typename Ret, typename F, typename... Args>
    typename std::enable_if<!std::is_void<Ret>::value, int>::type
    call_fun_and_push_result(const F& f, LuaInterface* lua, const Args&... args) {
        Ret ret = f(args...);
        lua->polymorphicPush(ret);
        return 1;
    }

    /// C++ void function caller
    template<typename Ret, typename F, typename... Args>
    typename std::enable_if<std::is_void<Ret>::value, int>::type
    call_fun_and_push_result(const F& f, LuaInterface* lua, const Args&... args) {
        f(args...);
        return 0;
    }

    /// Expand arguments from tuple for later calling the C++ function
    template<int N, typename Ret>
    struct expand_fun_arguments {
        template<typename Tuple, typename F, typename... Args>
        static int call(const Tuple& tuple, const F& f, LuaInterface* lua, const Args&... args) {
            return expand_fun_arguments<N-1,Ret>::call(tuple, f, lua, std::get<N-1>(tuple), args...);
        }
    };
    template<typename Ret>
    struct expand_fun_arguments<0,Ret> {
        template<typename Tuple, typename F, typename... Args>
        static int call(const Tuple& tuple, const F& f, LuaInterface* lua, const Args&... args) {
            return call_fun_and_push_result<Ret>(f, lua, args...);
        }
    };

    /// Bind different types of functions generating a lambda
    template<typename Ret, typename F, typename Tuple>
    LuaCppFunction bind_fun_specializer(const F& f) {
        enum { N = std::tuple_size<Tuple>::value };
        return [=](LuaInterface* lua) {
            if(lua->stackSize() != N)
                throw LuaBadNumberOfArgumentsException(N, lua->stackSize());
            Tuple tuple;
            pack_values_into_tuple<N>::call(tuple, lua);
            return expand_fun_arguments<N,Ret>::call(tuple, f, lua);
        };
    }

    /// Bind a std::function
    template<typename Ret, typename... Args>
    LuaCppFunction bind_fun(const std::function<Ret(Args...)>& f) {
        typedef typename std::tuple<typename remove_const_ref<Args>::type...> Tuple;
        return bind_fun_specializer<typename remove_const_ref<Ret>::type,
                                    decltype(f),
                                    Tuple>(f);
    }

    /// Bind a customized functions
    template<>
    inline
    LuaCppFunction bind_fun(const std::function<int(LuaInterface*)>& f) {
        return f;
    }

    /// Convert to C++ functions pointers to std::function then bind
    template<typename Ret, typename... Args>
    LuaCppFunction bind_fun(Ret (*f)(Args...)) {
        return bind_fun(std::function<Ret(Args...)>(f));
    }

    // a tuple_element that works with the next algorithm
    template<std::size_t __i, typename _Tp>
        struct tuple_element;
    template<std::size_t __i, typename _Head, typename... _Tail>
        struct tuple_element<__i, std::tuple<_Head, _Tail...> >
        : tuple_element<__i - 1, std::tuple<_Tail...> > { };
    template<typename _Head, typename... _Tail>
        struct tuple_element<0, std::tuple<_Head, _Tail...> > { typedef _Head type; };
    template<typename _Head>
        struct tuple_element<-1,std::tuple<_Head>> { typedef void type; };
    template<std::size_t __i>
        struct tuple_element<__i,std::tuple<>> { typedef void type; };

    template<typename Enable, int N, typename ArgsTuple, typename HoldersTuple, typename... Args>
    struct get_holded_tuple;

    // some dirty stuff to extract arguments from std::bind holders
    template<int N, typename ArgsTuple, typename HoldersTuple, typename... Args>
    struct get_holded_tuple<
                            typename std::enable_if<
                                (N > 0 && std::is_placeholder<
                                        typename tuple_element<N-1, HoldersTuple>::type
                                        >::value > 0), void
                                 >::type, N, ArgsTuple, HoldersTuple, Args...> {
        typedef typename std::tuple_element<N-1, HoldersTuple>::type holder_type;
        typedef typename tuple_element<std::is_placeholder<holder_type>::value-1, ArgsTuple>::type _arg_type;
        typedef typename remove_const_ref<_arg_type>::type arg_type;
        typedef typename get_holded_tuple<void, N-1, ArgsTuple, HoldersTuple, arg_type, Args...>::type type;
    };
    template<int N, typename ArgsTuple, typename HoldersTuple, typename... Args>
    struct get_holded_tuple<typename std::enable_if<(N > 0 && std::is_placeholder<typename tuple_element<N-1, HoldersTuple>::type>::value == 0), void>::type, N, ArgsTuple, HoldersTuple, Args...> {
        typedef typename get_holded_tuple<void, N-1, ArgsTuple, HoldersTuple, Args...>::type type;
    };
    template<typename ArgsTuple, typename HoldersTuple, typename... Args>
    struct get_holded_tuple<void, 0, ArgsTuple, HoldersTuple, Args...> {
        typedef typename std::tuple<Args...> type;
    };

    /// Rebind functions already bound by std::bind handling it's placeholders
    template<typename Ret, typename... Args, typename... Holders>
    LuaCppFunction bind_fun(const std::_Bind<Ret (*(Holders...))(Args...)>& f) {
        typedef typename std::tuple<Args...> ArgsTuple;
        typedef typename std::tuple<Holders...> HoldersTuple;
        typedef typename get_holded_tuple<void, sizeof...(Holders), ArgsTuple, HoldersTuple>::type Tuple;
        return bind_fun_specializer<typename remove_const_ref<Ret>::type,
                                    decltype(f),
                                    Tuple>(f);
    }

    /// Rebind member functions already bound by std::bind handling it's placeholders
    template<typename Obj, typename Ret, typename... Args, typename... Holders>
    LuaCppFunction bind_fun(const std::_Bind<std::_Mem_fn<Ret (Obj::*)(Args...)>(Obj*, Holders...)>& f) {
        typedef typename std::tuple<Args...> ArgsTuple;
        typedef typename std::tuple<Holders...> HoldersTuple;
        typedef typename get_holded_tuple<void, sizeof...(Holders), ArgsTuple, HoldersTuple>::type Tuple;
        return bind_fun_specializer<typename remove_const_ref<Ret>::type,
                                    decltype(f),
                                    Tuple>(f);
    }

    template<typename Obj, typename Ret, typename... Args, typename... Holders>
    LuaCppFunction bind_fun(const std::_Bind<std::_Mem_fn<Ret (Obj::*)(Args...) const>(Obj*, Holders...)>& f) {
        typedef typename std::tuple<Args...> ArgsTuple;
        typedef typename std::tuple<Holders...> HoldersTuple;
        typedef typename get_holded_tuple<void, sizeof...(Holders), ArgsTuple, HoldersTuple>::type Tuple;
        return bind_fun_specializer<typename remove_const_ref<Ret>::type,
                                    decltype(f),
                                    Tuple>(f);
    }

    /// Bind customized functions already bound by std::bind
    template<typename Obj>
    LuaCppFunction bind_fun(const std::_Bind<std::_Mem_fn<int (Obj::*)(LuaInterface*)>(Obj*, std::_Placeholder<1>)>& f) {
        return f;
    }
    inline
    LuaCppFunction bind_fun(const std::_Bind<int (*(std::_Placeholder<1>))(LuaInterface*)>& f) {
        return f;
    }

    /// Bind member functions
    template<typename Ret, typename Obj, typename... Args>
    LuaCppFunction bind_mem_fun(Ret (Obj::*f)(Args...)) {
        auto mf = std::mem_fn(f);
        typedef typename std::tuple<std::shared_ptr<Obj>, typename remove_const_ref<Args>::type...> Tuple;
        return bind_fun_specializer<typename remove_const_ref<Ret>::type,
                                    decltype(mf),
                                    Tuple>(mf);
    }
    template<typename Ret, typename Obj, typename... Args>
    LuaCppFunction bind_mem_fun(Ret (Obj::*f)(Args...) const) {
        auto mf = std::mem_fn(f);
        typedef typename std::tuple<std::shared_ptr<Obj>, typename remove_const_ref<Args>::type...> Tuple;
        return bind_fun_specializer<typename remove_const_ref<Ret>::type,
                                    decltype(mf),
                                    Tuple>(mf);
    }

    /// Bind customized member functions
    template<typename Obj>
    LuaCppFunction bind_mem_fun(int (Obj::*f)(LuaInterface*)) {
        auto mf = std::mem_fn(f);
        return [=](LuaInterface* lua) {
            auto obj = lua->castValue<std::shared_ptr<Obj>>(1);
            lua->remove(1);
            return mf(obj, lua);
        };
    }
}

#endif
