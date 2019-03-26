//
// Copyright Jason Rice 2019
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef SC2_NBDL_WEB_VIEW_MAIN_HPP
#define SC2_NBDL_WEB_VIEW_MAIN_HPP

#include <sc2_nbdl/web/route.hpp>

#include <boost/hana/string.hpp>
#include <boost/hana/tuple.hpp>
#include <nbdl/ui_spec.hpp>
#include <nbdl/webui/html.hpp>

namespace sc2_nbdl::web::view {
  using namespace nbdl::webui::html;
  using namespace nbdl::ui_spec;
  using namespace boost::hana::literals;

  // TODO This should create an href not an onclick event
  using route_href(using auto route_name, using auto ...param) {
    return on_click(set_route(route_name), param...);
  }

  constexpr auto page_legal = div(
    div(
      text_node("Some materials such as images and sounds are owned by "
                "Blizzard Entertainment and used in accordance with their  "
                "license stated here: "),
      a(attr_href("http://us.blizzard.com/en-us/company/about/legal-faq.html"),
        text_node("http://us.blizzard.com/en-us/company/about/legal-faq.html"))
    ),
    div(text_node("These materials are subject to the following copyright: ")),
    div(text_node("Blizzard Entertainment, Inc. All rights reserved. Starcraft II is a "
        "trademark and Blizzard Entertainment is a trademark or registered "
        "trademark of Blizzard Entertainment, Inc. in the U.S. and/or other "
        "countries."))
  );

  constexpr auto main = div(
    div(
      attr_class("uk-container uk-container-center"
                 "uk-margin-top uk-margin-large-bottom"),
      div(
        attr_class("uk-navbar"),
        a(
          attr_class("uk-navbar-brand"),
          text_node("Starcraft II Nbdl Demo"),
          route_href("")
        ),
        ul(
          attr_class("uk-navbar-nav"),
          li(a(text_node("Legal"), route_href("legal")))
        )
      )
    ),
    // FIXME annoying syntax with hana::tuple and hana::string here
    div(
      attr_class("uk-grid"),
      match(
        get(hana::make_tuple("nav_route"_s)),
        when<decltype(""_s)>(text_node("Hello, world!")),
        when<decltype("legal"_s)>(page_legal),
        when<>(div(text_node("Page Not Found")))
      )
    )
  );
}

#endif
