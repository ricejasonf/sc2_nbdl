#include <sc2_nbdl/server/context.hpp>
#include <sc2_nbdl/server/server.hpp>

#include <boost/asio.hpp>
#include <nbdl.hpp>

int main() {
  boost::asio::io_service io{};
  int port = 5050;

  static auto context = sc2_nbdl::server::make_context(
    nbdl::actor("websocket_server", sc2_nbdl::server::server{io, port})
  );

  // FIXME should use nbdl::actor_init which
  // does not exist yet
  context.actor<1>().start_accepting();
}
