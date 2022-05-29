#include <array>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>

std::string make_daytime_string() {
  std::time_t now = time(0);
  return std::ctime(&now);
}

class UdpServer {
 public:
  UdpServer(boost::asio::io_context& io_context)
      : socket_(io_context, boost::asio::ip::udp::endpoint(
                                boost::asio::ip::udp::v4(), 13)) {
    start_receive();
  }

 private:
  void start_receive() {
    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_), remote_endpoint_,
        boost::bind(&UdpServer::handle_receive, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
  }

  void handle_receive(const boost::system::error_code& error,
                      std::size_t /*bytes_transferred*/) {
    if (!error) {
      std::cout << "Incomming message." << std::endl;
      std::shared_ptr<std::string> message(
          new std::string(make_daytime_string()));
      socket_.async_send_to(
          boost::asio::buffer(*message), remote_endpoint_,
          boost::bind(&UdpServer::handle_send, this, message,
                      boost::asio::placeholders::error,
                      boost::asio::placeholders::bytes_transferred));

      start_receive();
    }
  }

  void handle_send(std::shared_ptr<std::string> /*message*/,
                   const boost::system::error_code& /*error*/,
                   std::size_t /*bytes_transferred*/) {}

  boost::asio::ip::udp::socket socket_;
  std::array<char, 1> recv_buffer_;
  boost::asio::ip::udp::endpoint remote_endpoint_;
};

int main() {
  try {
    boost::asio::io_context io_context;
    UdpServer server(io_context);
    io_context.run();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
