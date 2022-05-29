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

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
 public:
  typedef std::shared_ptr<TcpConnection> pointer;

  static pointer create(boost::asio::io_context& io_context) {
    return pointer(new TcpConnection(io_context));
  }

  boost::asio::ip::tcp::socket& socket() { return socket_; }

  void start() {
    message_ = make_daytime_string();
    boost::asio::async_write(
        socket_, boost::asio::buffer(message_),
        boost::bind(&TcpConnection::handle_write, shared_from_this()));
  }

 private:
  TcpConnection(boost::asio::io_context& io_context) : socket_(io_context) {}

  void handle_write() {}

  boost::asio::ip::tcp::socket socket_;
  std::string message_;
};

class TcpServer {
 public:
  TcpServer(boost::asio::io_context& io_context)
      : io_context_(io_context),
        acceptor_(io_context, boost::asio::ip::tcp::endpoint(
                                  boost::asio::ip::tcp::v4(), 13)) {
    start_accept();
  }

 private:
  void start_accept() {
    TcpConnection::pointer new_connection = TcpConnection::create(io_context_);

    acceptor_.async_accept(
        new_connection->socket(),
        boost::bind(&TcpServer::handle_accept, this, new_connection,
                    boost::asio::placeholders::error));
  }

  void handle_accept(TcpConnection::pointer new_connection,
                     const boost::system::error_code& error) {
    if (!error) {
      new_connection->start();
    }

    start_accept();
  }

  boost::asio::io_context& io_context_;
  boost::asio::ip::tcp::acceptor acceptor_;
};

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

    TcpServer tcp_server(io_context);
    UdpServer udp_server(io_context);

    io_context.run();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
