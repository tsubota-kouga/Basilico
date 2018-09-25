#ifndef __NVIMRPC_IMPL_H_
#define __NVIMRPC_IMPL_H_

namespace nvim {

namespace detail {
    using Packer = msgpack::packer<msgpack::sbuffer>;

    template<class T>
    Packer& pack(Packer& pk, const T& t)
    {
          return pk << t;
    }

    template<class T1, class T2, class...T3>
    Packer& pack(Packer& pk, const T1 &t1, const T2 &t2, const T3 &...t3)
    {
          return pack(pack(pk, t1), t2, t3...);
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
    std::cout << "T NvimRPC::call" << std::endl;

    res = boost::get<T>(v);
}

template<typename...U>
void NvimRPC::call(const String& method, Integer& res, const U& ...u)
{
    Object v = do_call(method, u...);
    std::cout << "Integer NvimRPC::call" << std::endl;

    // int64_t is only for negative integer.
    if(v.is_int64_t())       res = v.as_int64_t();
    else if(v.is_uint64_t()) res = v.as_uint64_t();
    else std::cout << "invalid response type" << std::endl; //TODO: add error handler
}

template<typename...U>
void NvimRPC::call(const String& method, Object& res, const U& ...u)
{
    Object v = do_call(method, u...);
    std::cout << "Object NvimRPC::call" << std::endl;
    res = v;
}

template<typename...U>
void NvimRPC::call(const String& method, std::nullptr_t res, const U&...u)
{
    do_call(method, u...);
    std::cout << "void NvimRPC::call" << std::endl;
}

template<typename...U>
void NvimRPC::no_read_do_call(const String& method, const U&...u)
{
    constexpr double timeout_sec = 5;
    constexpr int res_len = 4;
    msgpack::sbuffer sbuf;
    detail::Packer pk(&sbuf);
    pk.pack_array(res_len) << (uint64_t)REQUEST
                           << msgid_++
                           << method;

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
    constexpr double read_millisec = 50;

    no_read_do_call(method, u...);

    return read_info(read_millisec);
}

} //namespace nvim
#endif
