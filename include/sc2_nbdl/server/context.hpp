//
// Copyright Jason Rice 2019
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef SC2_NBDL_SERVER_CONTEXT_HPP
#define SC2_NBDL_SERVER_CONTEXT_HPP

#include <sc2_nbdl/system_message.hpp>

#include <mpdef/utility.hpp>
#include <nbdl.hpp>

namespace sc2_nbdl::server {
  struct context_tag {
    // Defines all of the Actors to be used in the application
    static constexpr auto make_def() {
      using namespace nbdl_def;

      return Context(
        Producers(
          Producer(
            Name("some_service"),
            Type<nbdl::echo_producer>,
            AccessPoints(
              AccessPoint(
                //RequireAccess("access_to_counter")
                PathKey<decltype(mpdef::to_constant("counter"))>,
                Entity<int>,
                Actions(Create())
              )
            )
          )
        ),
        Consumers(
          // TODO specify websocket server here
        )
      );
    }
  };

  // make_context - Convenience function for creating the context
  //                object. It uses a named parameter interface via
  //                `nbdl::actor`.
  //                Example:
  //                ```
  //                  sc2_nbdl::web::make_context(
  //                    nbdl::actor("some_service") = init_info
  //                  );
  //                ```
  //
  constexpr auto make_context = nbdl::make_context<context_tag>;

  using api = nbdl::message_api<context_tag, system_message>;
}

#endif
