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

  // TODO This should create an href and an onclick event
  using route_href(using auto route_name, using auto ...param) {
    return on_click(set_route(route_name), param...);
  }

  constexpr auto page_legal = div(
    attr_class("article"),
    h1(text_node("Legal Information")),
    div(
      text_node("Some materials such as images and sounds are owned by "
                "Blizzard Entertainment and used in accordance with their  "
                "license stated here: "),
      a(attr_href("http://us.blizzard.com/en-us/company/about/legal-faq.html"),
        text_node("http://us.blizzard.com/en-us/company/about/legal-faq.html"))
    ),
    div(
      attr_class("ui segment"),
      text_node("Blizzard Entertainment, Inc. All rights reserved. Starcraft II is a "
        "trademark and Blizzard Entertainment is a trademark or registered "
        "trademark of Blizzard Entertainment, Inc. in the U.S. and/or other "
        "countries."))
  );

  constexpr auto page_auth = div(
    h1(text_node("Authentication")),
    div(
      attr_class("ui placeholder segment"),
      text_node("Authentication stuffs goes here.")
    )
  );

  constexpr auto main = div(
    attr_class("pushable"),
    div(
      attr_class("pusher"),
      attribute("style", "height:100%"),
      div(
        attr_class("ui padded grid"),
        attribute("style", "flex-direction:column; height:100%"),
        div(
          attr_class("ui inverted attached menu"),
          attribute("style", "padding:0"),
          a(
            attr_class("header item"),
            text_node("Starcraft II Nbdl Demo"),
            route_href("")
          ),
          div(
            attr_class("right menu"),
            a(attr_class("item"), text_node("Sign In"),  route_href("sign-in"))
          )
        ),
        div(attr_class("ui hidden divider")), // add section if menu is fixed
        div(attr_class("ui container"),
          // FIXME annoying syntax with hana::tuple and hana::string here
          match(
            get(hana::make_tuple("nav_route"_s)),
            when<decltype(""_s)>      (text_node("Hello, world!")),
            when<decltype("legal"_s)> (page_legal),
            when<decltype("sign-in"_s)>  (page_auth),
            when<>(div(text_node("Page Not Found")))
          )
        ),
        div(attr_class("ui hidden divider"), attribute("style", "flex-grow:1")),
        div(
          attr_class("ui inverted attached footer menu"),
          attribute("style", "padding:0"),
          a(attr_class("item right"), text_node("Legal"), route_href("legal"))
        )
      )
    )
  );
}

#endif
