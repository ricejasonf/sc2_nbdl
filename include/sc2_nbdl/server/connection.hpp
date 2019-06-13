//
// Copyright Jason Rice 2019
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef SC2_NBDL_SERVER_CONNECTION_HPP
#define SC2_NBDL_SERVER_CONNECTION_HPP

// The Consumer that manages messaging to socket clients

#include <full_duplex.hpp>
#include <functional>
#include <nbdl.hpp>
#include <nbdl/binder/jsoncpp.hpp>
#include <queue>
#include <string>
#include <vector>

namespace sc2_nbdl::server {
  namespace hana = boost::hana;
  using full_duplex::promise;
  using full_duplex::tap;
  using full_duplex::void_input;

  constexpr auto authenticate = promise([](auto& resolve, auto& self) {
    // we should already have a token from a cookie or something
    // check against the db or whatever
    // must resolve self
    resolve(self);
  });

  constexpr auto serialize_message = full_duplex::map([](auto&& obj) {
    return nbdl::binder::jsoncpp::to_string(obj);
  });

  constexpr auto deserialize_message = promise([](auto& resolve,
                                               auto const& msg_buf) {
    using full_duplex::make_error;
    api::upstream_variant var;

    try {
      nbdl::binder::jsoncpp::from_string(msg_buf, var);
    } catch(...) {
      resolve(make_error("JSONCPP Parse Error"));
    }

    nbdl::match(var, hana::overload_linearly(
      [&resolve](nbdl::unresolved) {
        // this should not happen unless we get
        // garbage input or something
        resolve(make_error("Unknown Error"));
      },
      [&resolve](system_message sys_msg) {
        // something bad happened?
        resolve(make_error(std::move(sys_msg)));
      },
      [&resolve](auto&& msg) {
        resolve(std::forward<decltype(msg)>(msg));
      }));
  });

  constexpr auto apply_read = promise([](auto& resolve, auto&& msg) {
    nbdl::apply_message(resolve.get_state().get().context,
                        std::forward<decltype(msg)>(msg));
    resolve(void_input);
  });

  constexpr auto log_error = tap([](auto const& /*err*/) {
    //std::cerr << '\n' << err << '\n';
  });

  constexpr auto register_conn = tap([](auto& self) {
    self.get_state().get().register_connection(self);
  });

  constexpr auto unregister_conn = tap([](auto& self) {
    self.get_state().get().unregister_connection(self);
  });

  template <typename State>
  auto connection_open(State& state) {
    using full_duplex::do_;
    using full_duplex::endpoint;
    using full_duplex::endpoint_compose;
    using full_duplex::endpoint_open;
    using full_duplex::map;
    namespace event = full_duplex::event;

    // It's fine to just unwrap the smart pointer here.
    // `unregister_conn` removes itself from the list
    // before the resource is destroyed
    //
    // This is slightly better than copying the shared_ptr
    // just to find it in the container
    // (it would have to via shared_from_this)
    return full_duplex::endpoint_open(
      state,
      std::queue<std::string>{},
      endpoint(
        event::init           = do_(authenticate, register_conn),
        event::read_message   = do_(deserialize_message, apply_read),
        event::write_message  = serialize_message,
        event::error          = log_error
      //event::terminate      = unregister_conn
      )
    ).get();
  }

#if 0 // moved into server_impl
  // connection_manager exists because we push messages
  // it owns nothing but a list of pointers to self
  // managed objects
  template <typename Context>
  class connection_manager {
    struct handle {
      tcp::socket socket;
      connection_manager& manager;

      Context get_context() {
        return manager.context;
      }

      template <typename Conn>
      void register_connection(Conn&& c) {
        manager.connections.append(std::forward<Conn>(c));
      }

      template <typename Conn>
      void unregister_connection(Conn const& c) {
        auto itr = std::find(manager.connections.begin(),
                             manager.connections.end(), std::forward<Conn>(c));
        if (itr != manager.connections.end()) {
          manager.connections.erase(itr);
        }
      }
    };

    connection_manager(Context ctx)
      : context(ctx)
      , connections()
    { }

    connection_manager(connection_manager const&) = delete;

    using Connection = decltype(connection_open(
          std::declval<handle>()));

    Context context;
    std::vector<Connection> connections;

  public:
    // This is called with the result of
    // something like async_accept.
    // The socket is already connected
    void create(tcp::socket socket) {
      connection_open(handle{std::move(socket), *this});
    };
  };
#endif
}

#endif
