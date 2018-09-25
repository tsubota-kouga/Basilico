#ifndef NEOVIM_CPP__NVIM_RPC_HPP__
#define NEOVIM_CPP__NVIM_RPC_HPP__
#define BOOST_VARIANT_USE_RELAXED_GET_BY_DEFAULT

#include "msgpack.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <tuple>

#include "socket.hpp"

namespace nvim {

using Integer = int64_t;
using Window = Integer;
using Buffer = Integer;
using Tabpage = Integer;
using String = std::string;
using Object = msgpack::type::variant;
using cObject = msgpack::object;
using Map = std::multimap<Object, Object>;
using Dictionary = Map;
using Array = std::vector<Object>;

class NvimRPC
{

    uint64_t msgid_;
    Socket socket_;

    enum {
        REQUEST  = 0,
        RESPONSE = 1,
        NOTIFY   = 2
    };

public:
    NvimRPC() :msgid_(0) {}

    void connect_tcp(const String &host,
            const String &service, double timeout_sec)
    {
        socket_.connect_tcp(host, service, timeout_sec);
    }

    Object read_info(double timeout_millisec)
    {
        msgpack::unpacker unpacker;
        unpacker.reserve_buffer(32*1024ul);
        size_t rlen;
        try
        {
            rlen = socket_.read(unpacker.buffer(), unpacker.buffer_capacity(), timeout_millisec);
        }
        catch(boost::system::system_error& e)
        {
            std::cout << e.what() << std::endl;
        }
        msgpack::unpacked result;
        unpacker.buffer_consumed(rlen);
        unpacker.next(result);
        const cObject &obj = result.get();
        std::cout << "res = " << obj << std::endl;
        msgpack::type::tuple<Integer, Object, Object, Object> dst;
        obj.convert(dst);
        Object ret = dst.get<3>();
        if(!ret.is_nil()){ return ret; }
        else{ return dst.get<2>(); }
    }

    Array ui_read_info(double timeout_millisec)
    {
        msgpack::unpacker unpacker;
        unpacker.reserve_buffer(32*1024ul);
        Array ret;
        size_t rlen;
        try
        {
            rlen = socket_.ui_read(unpacker.buffer(), unpacker.buffer_capacity(), timeout_millisec);
        }
        catch(boost::system::system_error& e)
        {
            std::cout << e.what() << std::endl;
        }
        msgpack::unpacked result;
        unpacker.buffer_consumed(rlen);

        msgpack::type::tuple<Integer, Object, Object> dst;
        while(unpacker.next(result))
        {
            const cObject& obj = result.get();
            std::cout << "res = " << obj << std::endl;
            obj.convert(dst);
            ret.push_back(dst.get<2>());
        }
        return ret;
    }

    std::size_t available(){ return socket_.available(); }

    template<typename T, typename...U>
    void call(const String &method, T& res, const U&...u);

    template<typename...U>
    void call(const String &method, Integer& res, const U&...u);

    template<typename...U>
    void call(const String &method, Object& res, const U&...u);

    template<typename...U>
    void call(const String &method, std::nullptr_t res, const U&...u);

    template<typename...U>
    void no_read_do_call(const String &method, const U&...u);

private:
    template<typename...U>
    Object do_call(const String &method, const U&...u);
};

} //namespace nvim

#include "impl/nvim_rpc_impl.hpp"

#endif //NEOVIM_CPP__NVIM_RPC_HPP_

