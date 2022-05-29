#include <array>
#include <boost/asio.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
  try {
    if (argc != 2) {
      std::cerr << "Usage: asio_client_udp <host>" << std::endl;
      return 1;
    }

    boost::asio::io_context io_context;

    boost::asio::ip::udp::resolver resolver(io_context);
    auto receiver_endpoint =
        *resolver.resolve(boost::asio::ip::udp::v4(), argv[1], "daytime")
             .begin();

    boost::asio::ip::udp::socket socket(io_context);
    socket.open(boost::asio::ip::udp::v4());

    std::array<char, 1> send_buf = {{0}};
    socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

    std::array<char, 128> recv_buf;
    boost::asio::ip::udp::endpoint sender_endpoint;
    size_t len =
        socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);

    std::cout.write(recv_buf.data(), len);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
