#include <array>
#include <boost/asio.hpp>
#include <ctime>
#include <iostream>
#include <string>

std::string make_daytime_string() {
  std::time_t now = time(0);
  return std::ctime(&now);
}

int main() {
  try {
    boost::asio::io_context io_context;

    boost::asio::ip::udp::socket socket(
        io_context,
        boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 13));

    for (;;) {
      std::array<char, 1> recv_buf;
      boost::asio::ip::udp::endpoint remote_endpoint;
      socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);
      std::cout << "Incomming connection" << std::endl;

      auto message = make_daytime_string();

      boost::system::error_code ignored_error;
      socket.send_to(boost::asio::buffer(message), remote_endpoint, 0,
                     ignored_error);
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
