#include "socket.hpp"

#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/ip/tcp.hpp>

#define BOOST_VARIANT_USE_RELAXED_GET_BY_DEFAULT

using boost::asio::deadline_timer;
using boost::asio::ip::tcp;
using boost::lambda::bind;
using boost::lambda::var;
using boost::lambda::_1;
using boost::lambda::_2;

namespace nvim {

void Socket::connect_tcp(const std::string& host,
                     const std::string& service, long timeout_sec)
{
    tcp::resolver::query query(host, service);
    tcp::resolver::iterator iter = tcp::resolver(io_service_).resolve(query);

    deadline_.expires_from_now(boost::posix_time::seconds(timeout_sec));

    boost::system::error_code ec;

    for (; iter != tcp::resolver::iterator(); ++iter)
    {
        socket_.close();
        ec = boost::asio::error::would_block;
        socket_.async_connect(iter->endpoint(), var(ec) = _1);

        do io_service_.run_one(); while (ec == boost::asio::error::would_block);
        if (!ec && socket_.is_open()) return;
    }

    throw boost::system::system_error(
        ec ? ec : boost::asio::error::host_not_found);
}

size_t Socket::read(char *rbuf, size_t capacity, long timeout_millisec)
{
    constexpr int read_at_least = 1;
    deadline_.expires_from_now(boost::posix_time::milliseconds(timeout_millisec));
    boost::system::error_code ec = boost::asio::error::would_block;
    size_t rlen;

    async_read(socket_, boost::asio::buffer(rbuf, capacity),
        boost::asio::transfer_at_least(read_at_least),
        [&ec, &rlen](boost::system::error_code e, size_t s) { ec = e;rlen = s; });
    do { io_service_.run_one(); }
    while (ec == boost::asio::error::would_block);

    if(ec == boost::asio::error::operation_aborted)
    {
        std::cout << "operation_aborted;" << ec.message() <<  std::endl;
        exit(1);
    }
    else
    {
        deadline_.cancel_one();
        if(ec and ec != boost::asio::error::eof)
        {
            std::cout << "ERROR:" << ec.message() << std::endl;
            throw boost::system::system_error(ec);
        }
    }
    return rlen;
}

size_t Socket::ui_read(char *rbuf, size_t capacity, long timeout_millisec)
{
    constexpr int read_at_least = 1;
    deadline_.expires_from_now(boost::posix_time::milliseconds(timeout_millisec));
    boost::system::error_code ec = boost::asio::error::would_block;
    size_t rlen;

    async_read(socket_, boost::asio::buffer(rbuf, capacity),
        boost::asio::transfer_at_least(read_at_least),
        [&ec, &rlen](boost::system::error_code e, size_t s) { ec = e;rlen = s; });
    do { io_service_.run_one(); } while (ec == boost::asio::error::would_block);

    if(ec){ throw boost::system::system_error(ec); }
    return rlen;
}

void Socket::write(char *sbuf, size_t size, long timeout_sec)
{
    deadline_.expires_from_now(boost::posix_time::seconds(timeout_sec));
    boost::system::error_code ec = boost::asio::error::would_block;
    boost::asio::async_write(socket_, boost::asio::buffer(sbuf, size), var(ec) = _1);

    do io_service_.run_one(); while (ec == boost::asio::error::would_block);

    if (ec) throw boost::system::system_error(ec);
}

void Socket::check_deadline()
{
    if (deadline_.expires_at() <= deadline_timer::traits_type::now())
    {
        socket_.close();
        deadline_.expires_at(boost::posix_time::pos_infin);
        std::cout << "deadline!" << std::endl;
    }

    deadline_.async_wait(bind(&Socket::check_deadline, this));
}

} //namespace nvim

