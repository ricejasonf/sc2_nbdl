//
// Copyright Jason Rice 2019
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef SC2_NBDL_WEB_ROUTE_HPP
#define SC2_NBDL_WEB_ROUTE_HPP

#include <nbdl/webui/route_map.hpp>
#include <nbdl/webui/set_route.hpp>

namespace sc2_nbdl::web::route {
  // Any routes that have state would be defined here

  constexpr auto route_map = nbdl::webui::make_route_map(
    nbdl::webui::route_pair<>(""), // root
    nbdl::webui::route_pair<>("sign-in"),
    nbdl::webui::route_pair<>("legal")
  );

  using variant_t = typename decltype(route_map)::variant;

  constexpr auto make = [](auto&& x) {
    return variant_t(std::forward<decltype(x)>(x));
  };
}

namespace sc2_nbdl::web {
  constexpr auto set_route = nbdl::webui::make_set_route(route::route_map);
}

#endif
