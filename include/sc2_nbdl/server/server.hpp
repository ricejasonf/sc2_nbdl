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

#include <full_duplex.hpp>
#include <nbdl.hpp>
#include <queue>
#include <string>
#include <vector>

namespace sc2_nbdl::server {
  namespace beast_ws = nbdl::ext::beast_ws;

  // server - The construction arg for make_context
  struct server {
    asio::io_service& io;
    int port;
  };

  template <typename Context>
  struct server_impl {
    using hana_tag = server;

    server_impl(server_impl const&) = delete;

    server_impl(nbdl::actor_initializer<Context, server> a)
      : server_(a.value)
      , connections(a.context)
    { }

    struct handle {
      // eventually replace tcp::socket with something that includes
      // http request information to get the session cookie
      tcp::socket socket_;
      server_impl& serv_;

      Context get_context() {
        return serv_.context;
      }

      tcp::socket& get_socket() {
        return socket_;
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
          std::declval<handle>()));

    server server_;
    std::vector<Connection> connections;


    void start_accepting() {
      using full_duplex::promise;
      using full_duplex::map;
      using full_duplex::void_input;

      full_duplex::run_async_loop(
        beast_ws::accept_socket(tcp::acceptor{server_.io},
                                tcp::endpoint{tcp::v4(), server_.port}),

        map([this](tcp::socket socket) {
          // create connection object add to connections list
          connection_open(handle{std::move(socket), *this});
          return void_input{};
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
#if 0 // there is currently no way to do this
      // it is called in `main()` for now
  template <>
  struct consumer_init<sc2_nbdl::server> {
    static void apply(Consumer& c) {
      c.start_accepting();
    }
  };
#endif
  template <typename Context>
  struct actor_type<server_impl<sc2_nbdl::server, Context>> {
    using type = sc2_nbdl::server_impl<Context>;
  };

  template <>
  struct send_downstream_message_impl<sc2_nbdl::server> {
    template <typename Consumer, typename Message>
    static void apply(Consumer& c, Message&& m) {
      // send message to connections?
      c.send_message(std::forward<Message>(m));
    }
  };
}

#endif
