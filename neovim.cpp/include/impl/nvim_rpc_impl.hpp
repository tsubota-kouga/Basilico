#ifndef ___NVIMRPC_IMPL_H_
#define ___NVIMRPC_IMPL_H_

#include <vector>
#include <mutex>

namespace nvim {

namespace detail {
    using Packer = msgpack::packer<msgpack::sbuffer>;

    template<class ...Args>
    Packer& pack(Packer& pk, const Args& ...args)
    {
        return (pk << ... << args);
    }

    static Packer& pack(Packer& pk)
    {
        return pk;
    }

} // namespace detail

template<typename T, typename...U>
void NvimRPC::call(const String &method, T& res, const U&...u)
{
    Object v = do_call(method, u...);
    std::cout << "T NvimRPC::call " << method << std::endl;

    res = boost::get<T>(v);
}

template<typename T, typename...U>
void NvimRPC::call(const String &method, std::vector<T>& res, const U&...u)
{
    Object v = do_call(method, u...);
    std::cout << "Vector<T> NvimRPC::call " << method << std::endl;

    auto&& tmp = boost::get<Array>(v);
    if(tmp.size() == 0)
    {
        res.push_back(T{});
        return;
    }
    res.reserve(sizeof tmp);
    for(auto&& c: tmp)
    {
        res.push_back(boost::get<T>(c));
    }
}

template<typename...U>
void NvimRPC::call(const String &method, Array& res, const U&...u)
{
    Object v = do_call(method, u...);
    std::cout << "Array NvimRPC::call " << method << std::endl;

    res = boost::get<Array>(v);
    if(res.size() == 0)
    {
        res.push_back(Object{});
    }
}

template<typename...U>
void NvimRPC::call(const String& method, Integer& res, const U& ...u)
{
    Object v = do_call(method, u...);
    std::cout << "Integer NvimRPC::call " << method << std::endl;

    // int64_t is only for negative integer.
    if(v.is_int64_t())       res = v.as_int64_t();
    else if(v.is_uint64_t()) res = v.as_uint64_t();
    else std::cout << "invalid response type" << std::endl; //TODO: add error handler
}

template<typename...U>
void NvimRPC::call(const String& method, Object& res, const U& ...u)
{
    res = do_call(method, u...);
    std::cout << "Object NvimRPC::call " << method << std::endl;
}

template<typename...U>
void NvimRPC::call(const String& method, std::nullptr_t res, const U&...u)
{
    do_call(method, u...);
    std::cout << "void NvimRPC::call " << method << std::endl;
}

template<typename...U>
void NvimRPC::no_read_do_call(const String& method, const U&...u)
{
    constexpr long timeout_sec = 1000;
    constexpr int res_len = 4;
    msgpack::sbuffer sbuf;
    detail::Packer pk(&sbuf);
    pk.pack_array(res_len) << (uint64_t)REQUEST << msgid_++ << method;

    pk.pack_array(sizeof...(u));
    detail::pack(pk, u...);

    msgpack::object_handle oh = msgpack::unpack(sbuf.data(), sbuf.size());

    cObject deserialized = oh.get();

    std::cout << "sbuf = " << deserialized << std::endl;

    socket_.write(sbuf.data(), sbuf.size(), timeout_sec);
}

template<typename...U>
Object NvimRPC::do_call(const String& method, const U&...u)
{
    constexpr long read_millisec = 3000;

    no_read_do_call(method, u...);

    return read_info(read_millisec);
}

} //namespace nvim
#endif
