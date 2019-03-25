//
// Copyright Jason Rice 2019
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef SC2_NBDL_WEB_CONTEXT_HPP
#define SC2_NBDL_WEB_CONTEXT_HPP

#include <sc2_nbdl/web/view/main.hpp>
#include <sc2_nbdl/web/route.hpp>

#include <nbdl.hpp>
#include <nbdl/webui/nav_route.hpp>
#include <nbdl/webui/renderer.hpp>

namespace sc2_nbdl::web {
  struct context_tag {
    // Defines all of the Actors to be used in the application
    static constexpr auto make_def() {
      using namespace nbdl_def;
      using web_main = nbdl::webui::renderer<
        decltype(::sc2_nbdl::web::view::main)>;

      return Context(
        Producers(
          NavRouteProducer(route::route_map)
        ),
        Consumers(
          Consumer(Name("web_main"), Type<web_main>)
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
  //                    nbdl::actor("web_main") = dom_node
  //                  );
  //                ```
  //
  constexpr auto make_context = nbdl::make_context<context_tag>;
}

#endif
