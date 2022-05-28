#include <boost/asio.hpp>
#include <iostream>

void print(const boost::system::error_code& /*e*/) {
  std::cout << "Hello, world!" << std::endl;
}

int main() {
  std::cout << "Start of the main program." << std::endl;

  boost::asio::io_context io;

  boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));

  std::cout << "Wait for timer (~5s)" << std::endl;

  t.async_wait(&print);

  io.run();

  std::cout << "Wait for io.run() finish." << std::endl;

  return 0;
}
