//
// Copyright Jason Rice 2019
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef SC2_NBDL_SERVER_SERVER_HPP
#define SC2_NBDL_SERVER_SERVER_HPP

// The Consumer that manages messaging to socket clients

#include <sc2_nbdl/server/connection.hpp>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <full_duplex.hpp>
#include <nbdl.hpp>
#include <string_view>
#include <vector>

namespace sc2_nbdl::server {
  namespace beast_ws = nbdl::ext::beast_ws;
  namespace net = boost::asio;
  using tcp = net::ip::tcp;

  // server - The construction arg for make_context
  struct server {
    net::io_service& io;
    int port;
  };

  void error_log(std::string_view) { }

  template <typename Context>
  struct server_impl {
    using hana_tag = server;

    tcp::acceptor acceptor_;
    beast_ws::stream_t stream_;
    boost::system::error_code ec;

    server_impl(server_impl const&) = delete;

    server_impl(nbdl::actor_initializer<Context, server> a)
      : connections(a.context)
      , acceptor_(a.io)
    {
      tcp::endpoint endpoint{tcp::v4(), a.port};

      // open the acceptor syncronously
      acceptor_.open(endpoint.protocol(), ec);
      if (ec) {
        error_log("open");
        return;
      }
      acceptor_.set_option(net::socket_base::reuse_address(true), ec);
      if (ec) {
        error_log("set_option");
        return;
      }
      acceptor_.bind(endpoint, ec);
      if (ec) {
        error_log("bind");
        return;
      }
      acceptor_.listen(net::socket_base::max_listen_connections, ec);
      if (ec) {
        error_log("listen");
        return;
      }
    }

    auto& acceptor() { return acceptor_; }
    auto& stream()   { return stream_; }
    auto& socket()   { return stream_.next_layer(); }

    struct connection_state {
      // eventually replace tcp::socket with something that includes
      // http request information to get the session cookie
      beast_ws::stream_t stream_;
      server_impl& serv_;

      Context get_context() {
        return serv_.context;
      }

      template <typename Conn>
      void register_connection(Conn&& c) {
        serv_.connections.append(std::forward<Conn>(c));
      }

      template <typename Conn>
      void unregister_connection(Conn const& c) {
        auto itr = std::find(serv_.connections.begin(),
                             serv_.connections.end(), std::forward<Conn>(c));
        if (itr != serv_.connections.end()) {
          serv_.connections.erase(itr);
        }
      }
    };

    using Connection = decltype(connection_open(
          std::declval<connection_state>()));

    std::vector<Connection> connections;


    void start_accepting() {
      using full_duplex::promise;
      using full_duplex::map;
      using full_duplex::void_input;

      if (ec) return;

      full_duplex::run_async_loop_with_state(
        *this,
        beast_ws::accept,
#if 0
        full_duplex::tap([&](auto& stream ) {
          // TODO check session token
        }),
#endif
        map([this](auto stream) {
          // create connection object add to connections list
          connection_open(connection_state{std::move(stream), *this});
          return void_input;
        }),
        map_any([](auto&& val) {
          if constexpr(full_duplex::is_error<decltype(val)>) {
            boost::system::error_code code = val;
            (void) code;
            // TODO log error message
          }
          return void_input;
        })
      );
    }

    template <typename Message>
    void send_message(Message const& m) {
      for (auto& conn : connections) {
        conn.send_message(m);
      }
    }
  };
}

namespace nbdl {
  template <>
  struct consumer_init_impl<sc2_nbdl::server::server> {
    template <typename Consumer>
    static void apply(Consumer& c) {
      c.start_accepting();
    }
  };

  template <typename Context>
  struct actor_type<sc2_nbdl::server::server, Context> {
    using type = sc2_nbdl::server::server_impl<Context>;
  };

  template <>
  struct send_downstream_message_impl<sc2_nbdl::server::server> {
    template <typename Consumer, typename Message>
    static void apply(Consumer& c, Message&& m) {
      // send message to connections?
      c.send_message(std::forward<Message>(m));
    }
  };
}

#endif
