//
// Copyright Jason Rice 2019
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef SC2_NBDL_WEB_VIEW_MAIN_HPP
#define SC2_NBDL_WEB_VIEW_MAIN_HPP

#include <nbdl/ui_spec.hpp>
#include <nbdl/webui/html.hpp>

namespace sc2_nbdl::web::view {
  using namespace nbdl::webui::html;
  using namespace nbdl::ui_spec;

  constexpr auto main = div(
    text_node("Hello, world!")
  );
}

#endif
