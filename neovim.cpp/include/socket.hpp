#ifndef NEOVIM_CPP__SOCKET_HPP_
#define NEOVIM_CPP__SOCKET_HPP_
#define BOOST_VARIANT_USE_RELAXED_GET_BY_DEFAULT

#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/generic/stream_protocol.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/system/system_error.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

namespace nvim {

class Socket
{
public:
    Socket() :
        socket_{io_service_},
        deadline_{io_service_}
    {
        deadline_.expires_at(boost::posix_time::pos_infin);
        check_deadline();
    }

    void connect_tcp(const std::string& host,
               const std::string& service, long timeout_sec);

    size_t read(char *rbuf, size_t capacity, long timeout_microsec);

    size_t ui_read(char *rbuf, size_t capacity, long timeout_microsec);

    void write(char *sbuf, size_t size, long timeout_sec);

    size_t available(){ return socket_.available(); }

private:
    void check_deadline();

    boost::asio::io_service io_service_;
    boost::asio::generic::stream_protocol::socket socket_;
    boost::asio::deadline_timer deadline_;
    boost::asio::streambuf input_buffer_;
};

} //namespace nvim

#endif //NEOVIM_CPP__SOCKET_HPP_
