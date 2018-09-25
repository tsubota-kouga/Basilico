
#include <iostream>
#include <boost/asio.hpp>

namespace asio = boost::asio;
using asio::ip::tcp;

int main(int argc, char const* argv[])
{
    asio::io_service io_service;
    tcp::socket socket(io_service);

    boost::system::error_code error;
    socket.connect(tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), 6666), error);

    if(error)
    {
        std::cout << "connect failed" << error.message() << std::endl;
    }
    else
    {
        std::cout << "connected" << std::endl;
    }
    return 0;
}
