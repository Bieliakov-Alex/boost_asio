#include <asio.hpp>
#include <iostream>

int main() {
  std::cout << "Program start. Wait (~5s)." << std::endl;
  asio::io_context io;

  asio::steady_timer t(io, asio::chrono::seconds(5));
  t.wait();

  std::cout << "Hello, world!" << std::endl;

  return 0;
}
