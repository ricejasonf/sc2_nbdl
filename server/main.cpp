  #include <iostream>

#include <sc2_nbdl/server/context.hpp>
#include <sc2_nbdl/server/server.hpp>

#include <boost/asio.hpp>
#include <nbdl.hpp>

int main() {
  boost::asio::io_service io{};
  unsigned short port = 5050;

  auto context = sc2_nbdl::server::make_context(
    nbdl::actor("websocket_server", sc2_nbdl::server::server{io, port})
  );

  io.run();
}
